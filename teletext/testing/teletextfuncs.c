#include "testheadersdl2.h"

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
  return array;
}
void free_array(unsigned int** array)
{
  int i;
  for (i=0; i<HEIGHT; i++){
    free(array[i]);
  }

  free(array);
}
void print_array(unsigned int ** array)
{
  int i,j;
  for (i=0;i<HEIGHT;i++)
  {
    for(j=0;j<WIDTH;j++)
    {
      if (j==WIDTH)
      {
      printf("\n");
      }
      printf("%3u ", array[i][j]);
    }
  printf("\n");
  }
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
  data->holdmem=OFF;
  data->doublemem=OFF;
}
void update_data(Data*data, unsigned int code, int i, int j)
{
  data->code=code;
  data->coords.i=i;
  data->coords.j=j;
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
    data->mode=CONTIGUOUS;
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
int alpha_mode(Data*data)
{
  unsigned int temp;
  temp=data->code;

  if ((data->mode==ALPHA && (temp>CHARSLOWER&&temp<CHARSUPPER))||data->blast==ON){
    temp-=CONSTANT;
    if (data->doubleheight==OFF){
      return 0;
    }
    if (data->doubleheight==ON && data->location==TOP){
      return 1;
    }
    if (data->doubleheight==ON && data->location==BOTTOM){
      return 2;
    }
  }
  return 4;
}
void graphics_mode(SDL_Rect *rectangle , Data*data)
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
    if ((temp>CHARSLOWER&&temp<CAPSLOWER)||(temp>CAPSUPPER&&temp<CHARSUPPER)){
       data->holdmem=temp;
       while(temp>0){
           temp-=GRAPHICSCONSTANT;
           if (temp/DIVIDENDONE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT));
             temp-=DIVIDENDONE;
           }
           if (temp/DIVIDENDTWO>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT));
             temp-=DIVIDENDTWO;
           }
           if (temp/DIVIDENDTHREE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT+FNTHEIGHT/HEIGHTCONSTANT;
             temp-=DIVIDENDTHREE;
           }
           if(temp/DIVIDENDFOUR>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT+FNTHEIGHT/HEIGHTCONSTANT;
             temp-=DIVIDENDFOUR;
           }
           if(temp/DIVIDENDFIVE>=1){
             rectangle->x=j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT;
             rectangle->y=i*FNTHEIGHT;
             temp-=DIVIDENDFIVE;
           }
           if(temp/1>=1){
             rectangle->x=j*FNTWIDTH;
             rectangle->y=i*FNTHEIGHT;
             temp-=1;
           }
        }
      }
      else if(temp>=CAPSLOWER&&temp<=CAPSUPPER){
        blast_mode(data);
      }
    }
}
void blast_mode(Data*data)
{
  data->blast=ON;
}
