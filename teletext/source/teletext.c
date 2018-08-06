#include "headersdl2.h"

int main (int argc, char**argv)
{
  SDL_Simplewin sw;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  FILE *fp;
  unsigned int ** array;
  Data data;

  fp=file_open(argc, argv);
  array = array_creation(fp);
  Neill_SDL_ReadFont(fontdata, "m7fixed.fnt");
  Neill_SDL_Init(&sw);
  SDL_Delay(MILLISECONDDELAY);
  create_window(&data, array, &sw, fontdata);
  update_screen(&sw);

  return 0;
}

FILE* file_open(int argc, char**argv)
{
  FILE*fp;
  if (argc!=2){
    fprintf(stderr, "Error: Please enter a file!\n");
    exit(EXIT_FAILURE);
  }
  fp = fopen(argv[1], "rb");
  return fp;
}
unsigned int ** array_creation(FILE * fp)
{
  int i,j;
  unsigned int value;
  unsigned int ** array;

  array=(unsigned int**)calloc(HEIGHT, sizeof(unsigned int*));
  for (i=0;i<HEIGHT;i++){
  array[i]=(unsigned int*)calloc(WIDTH,sizeof(unsigned int));
  }
  if (!array){
    fprintf(stderr, "Error: Memory Allocation Failed!\n");
    fclose(fp);
    exit(EXIT_FAILURE);
  }
  value=0;
  for (i=0; i<HEIGHT; i++){
      for(j=0;j<WIDTH;j++){
        if (fread(&value,1, 1, fp)<=FILELEN){
          if (value<CONSTANT){
            value = CONSTANT+value;
          }
          if (value>CONTROLOWERBOUND&&value<CHARSUPPER){
            array[i][j]=value;
          }
          else{
          fprintf(stderr, "Error: Values Not Within Bounds!\n");
          exit(EXIT_FAILURE);
          }

        }
      }
  }
  fclose(fp);
  return array;
}
void free_array(unsigned int** array)
{
  int i;
  for (i=0;i<HEIGHT;i++){
    free(array[i]);
  }
  free(array);
}
void initialize_location(Data*data)
{
  data->location=DEFAULT;
}
void reset_data(Data*data)
{
  data->background.red=0;
  data->background.green=0;
  data->background.blue=0;
  data->foreground.red=255;
  data->foreground.green=255;
  data->foreground.blue=255;
  data->mode=ALPHA;
  data->hold=OFF;
  data->doubleheight=OFF;
  data->blast=OFF;
  data->holdmem=0;
  data->doublemem=0;
}
void update_data(Data*data, unsigned int code, int i, int j)
{
  data->code=code;
  data->coords.i=i;
  data->coords.j=j;
}
void background_create(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data)
{
  DrawChar_Normal(sw, fontdata, SPACE, data);
}
void mode_select(Data*data)
{
  unsigned int temp;
  temp=data->code;
  if (temp>=REDALPHA&&temp<=WHITEALPHA){
    data->mode=ALPHA;
  }
  if (temp==SINGLEH){
    data->doubleheight=OFF;
  }
  if (temp==DOUBLEH){
    data->doubleheight=ON;

    if (data->doublemem==OFF){
      if (data->location==TOP){
        data->location=BOTTOM;
        data->doublemem=ON;
      }
      else{
        data->location=TOP;
        data->doublemem=ON;
      }
    }
  }
  if (temp>=REDGRAPHICS&&temp<=WHITEGRAPHICS){
    if (data->hold==OFF){
    data->mode=CONTIGUOUS;
    }
  }
  if (temp==CONTIGUOUSGRAPHICS){
    data->mode=CONTIGUOUS;
  }
  if (temp==SEPARATEDGRAPHICS){
    data->mode=SEPARATED;
  }
  if (temp==HOLDGRAPHICS){
    data->hold=ON;
  }
  if (temp==RELEASEGRAPHICS){
    data->hold=OFF;
  }
}
void char_colour(Data*data)
{
  int temp;
  if (data->code>=REDGRAPHICS&&data->code<=WHITEGRAPHICS){
  temp=data->code-GRAPHICSCOLOURCONSTANT;
  }
  else{
  temp=data->code;
  }
  if (temp==REDALPHA){
    data->foreground.red=255;
    data->foreground.green=0;
    data->foreground.blue=0;
  }
  if (temp==GREENALPHA){
    data->foreground.red=0;
    data->foreground.green=255;
    data->foreground.blue=0;
  }
  if (temp==YELLOWALPHA){
    data->foreground.red=255;
    data->foreground.green=255;
    data->foreground.blue=0;
  }
  if (temp==BLUEALPHA){
    data->foreground.red=0;
    data->foreground.green=0;
    data->foreground.blue=255;
  }
  if (temp==MAGALPHA){
    data->foreground.red=255;
    data->foreground.green=0;
    data->foreground.blue=255;
  }
  if (temp==CYANALPHA){
    data->foreground.red=0;
    data->foreground.green=255;
    data->foreground.blue=255;
  }
  if (temp==WHITEALPHA){
    data->foreground.red=255;
    data->foreground.green=255;
    data->foreground.blue=255;
  }
  if (temp==BLACKBACKGROUND){
    data->background.red=0;
    data->background.green=0;
    data->background.blue=0;
  }
  if (temp==NEWBACKGROUND){
    data-> background.red=data->foreground.red;
    data->background.green=data->foreground.green;
    data->background.blue=data->foreground.blue;
  }
}
void alpha_mode(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data)
{
  unsigned int temp;
  temp=data->code;

  if ((data->mode==ALPHA && (temp>CHARSLOWER&&temp<CHARSUPPER))||data->blast==ON){
    temp-=CONSTANT;
    if (data->doubleheight==OFF){
      DrawChar_Normal(sw, fontdata, temp, data);
    }
    if (data->doubleheight==ON && data->location==TOP){
      DrawChar_DoubleTop(sw, fontdata, temp, data);
    }
    if (data->doubleheight==ON && data->location==BOTTOM){
      DrawChar_DoubleBottom(sw, fontdata, temp, data);
    }
  }
}
void DrawChar_Normal(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data)
{
  unsigned y,x,oy,ox;
  oy=(data->coords.i)*FNTHEIGHT;
  ox=(data->coords.j)*FNTWIDTH;
  for(y = 0; y < FNTHEIGHT; y++){
    for(x = 0; x < FNTWIDTH; x++){
       if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
          Neill_SDL_SetDrawColour(sw, data->foreground.red, data->foreground.green, data->foreground.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
       }
       else{
          Neill_SDL_SetDrawColour(sw, data->background.red, data->background.green, data->background.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
       }
    }
  }
}
void DrawChar_DoubleTop(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data)
{
  unsigned y,x,oy,ox;
  oy=(data->coords.i)*FNTHEIGHT;
  ox=(data->coords.j)*FNTWIDTH;

  for(y = 0; y < FNTHEIGHT/2; y++){
    for(x = 0; x < FNTWIDTH; x++){
       if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
          Neill_SDL_SetDrawColour(sw, data->foreground.red, data->foreground.green, data->foreground.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+oy);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+oy+1);
       }
       else{
          Neill_SDL_SetDrawColour(sw, data->background.red, data->background.green, data->background.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+oy);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+oy+1);
        }
    }
  }
}
void DrawChar_DoubleBottom(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data)
{
  unsigned y,x,oy,ox;
  oy=(data->coords.i)*FNTHEIGHT;
  ox=(data->coords.j)*FNTWIDTH;

  for(y = FNTHEIGHT/2; y < FNTHEIGHT; y++){
    for(x = 0; x < FNTWIDTH; x++){
       if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
          Neill_SDL_SetDrawColour(sw, data->foreground.red, data->foreground.green, data->foreground.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+(oy-FNTHEIGHT));
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+(oy-FNTHEIGHT+1));

       }
       else{
          Neill_SDL_SetDrawColour(sw, data->background.red, data->background.green, data->background.blue);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+(oy-FNTHEIGHT));
          SDL_RenderDrawPoint(sw->renderer, x + ox, y*2+(oy-FNTHEIGHT+1));
        }
    }
  }
}
void graphics_mode(SDL_Rect *rectangle, SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data)
{
  unsigned int temp;
  int i, j;
  i=data->coords.i;
  j=data->coords.j;

  temp=0;
  if(data->mode!=ALPHA){
    if (data->mode==CONTIGUOUS){
      rectangle->w = FNTWIDTH/WIDTHCONSTANT;
      rectangle->h = FNTHEIGHT/HEIGHTCONSTANT;
    }
    else{
      rectangle->w = (FNTWIDTH-WIDTHCONSTANT)/WIDTHCONSTANT;
      rectangle->h = (FNTHEIGHT-HEIGHTCONSTANT)/HEIGHTCONSTANT;
    }
    if (data->hold==ON){
      temp=data->holdmem;
    }
    else{
      temp=data->code;
    }
    Neill_SDL_SetDrawColour(sw, data->foreground.red, data->foreground.green, data->foreground.blue);
    if ((temp>CHARSLOWER&&temp<CAPSLOWER)||(temp>CAPSUPPER&&temp<CHARSUPPER)){
       data->holdmem=temp;
       while(temp>0){
           temp-=GRAPHICSCONSTANT;
           if (temp/DIVIDENDONE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT));
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=DIVIDENDONE;
           }
           if (temp/DIVIDENDTWO>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT));
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=DIVIDENDTWO;
           }
           if (temp/DIVIDENDTHREE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT+FNTHEIGHT/HEIGHTCONSTANT;
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=DIVIDENDTHREE;
           }
           if(temp/DIVIDENDFOUR>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT+FNTHEIGHT/HEIGHTCONSTANT;
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=DIVIDENDFOUR;
           }
           if(temp/DIVIDENDFIVE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT;
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=DIVIDENDFIVE;
           }
           if(temp/1>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT;
             SDL_RenderFillRect(sw->renderer, rectangle);
             temp-=1;
           }
        }
    }
    else if(temp>=CAPSLOWER&&temp<=CAPSUPPER){
      blast_mode(sw, fontdata, data);
    }
  }
}
void blast_mode(SDL_Simplewin*sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data)
{
  data->blast=ON;
  alpha_mode(sw, fontdata, data);
  data->blast=OFF;
}

void create_window(Data*data, unsigned int ** array, SDL_Simplewin* sw, fntrow fontdata[FNTCHARS][FNTHEIGHT])
{
  int i,j;
  SDL_Rect rectangle;

  initialize_location(data);
  for (i=0;i<HEIGHT;i++){
    reset_data(data);
    for (j=0;j<WIDTH; j++){
      update_data(data, array[i][j], i, j);
      background_create(sw, fontdata, data);
      mode_select(data);
      char_colour(data);
      alpha_mode(sw, fontdata, data);
      graphics_mode(&rectangle, sw, fontdata, data);
    }
  }
  free_array(array);
}
void update_screen(SDL_Simplewin*sw)
{
  do{
    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
  }while(!sw->finished);
}
