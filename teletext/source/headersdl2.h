#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

/*Width and height of SDL window*/
#define WWIDTH 640
#define WHEIGHT 450

/* Font stuff */
typedef unsigned short fntrow;
#define FNTWIDTH 16
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32
#define SDL_8BITCOLOUR 256


/*Long list of various #defines to avoid magic numbers*/
#define MILLISECONDDELAY 20
#define WIDTH 40
#define HEIGHT 25
#define CONSTANT 128
#define FILELEN 1000
  #define ALPHA 0
#define CONTIGUOUS 1
#define SEPARATED 2
#define OFF 0
#define ON 1
#define TOP 1
#define BOTTOM 0
#define DEFAULT 2
#define SPACE 32
#define REDALPHA 129
#define GREENALPHA 130
#define YELLOWALPHA 131
#define BLUEALPHA 132
#define MAGALPHA 133
#define CYANALPHA 134
#define WHITEALPHA 135
#define CONTROLOWERBOUND 127
#define CONTROLUPPERBOUND 159
#define SINGLEH 140
#define DOUBLEH 141
#define REDGRAPHICS 145
#define WHITEGRAPHICS 151
#define CONTIGUOUSGRAPHICS 153
#define SEPARATEDGRAPHICS 154
#define BLACKBACKGROUND 156
#define NEWBACKGROUND 157
#define HOLDGRAPHICS 158
#define RELEASEGRAPHICS 159
#define CHARSLOWER 159
#define CHARSUPPER 256
#define CAPSLOWER 192
#define CAPSUPPER 223
#define GRAPHICSCONSTANT 160
#define GRAPHICSCOLOURCONSTANT 16
#define WIDTHCONSTANT 2
#define HEIGHTCONSTANT 3
#define DIVIDENDONE 64
#define DIVIDENDTWO 16
#define DIVIDENDTHREE 8
#define DIVIDENDFOUR 4
#define DIVIDENDFIVE 2

/* All info required for windows / renderer & event loop */
struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
   SDL_Texture *display;
};
typedef struct SDL_Simplewin SDL_Simplewin;

typedef struct Coords{
    int i;
    int j;
}Coords;

typedef struct Colours{
    int red;
    int green;
    int blue;
}Colours;

/*Struct containing all data needed to draw each cell */
typedef struct Data{
  Colours background;
  Colours foreground;
  int mode;
  int hold;
  int doubleheight;
  int location;
  unsigned int code;
  Coords coords;
  unsigned int blast;
  unsigned int holdmem;
  int doublemem;
}Data;


/*Functions to change data within data struct and pass to SDL*/
FILE* file_open(int argc, char**argv);
unsigned int ** array_creation(FILE * fp);
void free_array(unsigned int** array);
void initialize_location(Data*data);
void reset_data(Data*data);
void update_data(Data*data, unsigned int code, int i, int j);
void background_create(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data);
void mode_select(Data*data);
void char_colour(Data*data);
void alpha_mode(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data);
void graphics_mode(SDL_Rect *rectangle, SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data);
void blast_mode(SDL_Simplewin*sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Data*data);
void create_window(Data*data, unsigned int ** array, SDL_Simplewin* sw, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
void update_screen(SDL_Simplewin*sw);


/*The following three functions are copies of Neill Campbell's function for drawing chars in SDL. I have made only minor modifications*/
void DrawChar_Normal(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data);
void DrawChar_DoubleTop(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data);
void DrawChar_DoubleBottom(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, Data*data);

/*Neill Campbell's functions for operation of the SDL environment*/
void Neill_SDL_Init(SDL_Simplewin *sw);
void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);
void Neill_SDL_UpdateScreen(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy, Colours *foreground, Colours *background);
void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);
void Neill_SDL_UpdateScreen(SDL_Simplewin *sw);
