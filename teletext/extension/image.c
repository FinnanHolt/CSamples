#include <wand/magick_wand.h>

#define WIDTH 40
#define HEIGHT 25

MagickWand* RGB_create(int argc, char**argv);

int main(int argc, char**argv)
{
  MagickWand* wand;
  if (argc!=2)
  {
    fprintf(stderr, "Please Enter A Filename\n");
    exit(EXIT_FAILURE);
  }
  RGB_create(argc, argv);
  if(wand)wand = DestroyMagickWand(wand);
  MagickWandTerminus();
  return 0;
}


MagickWand* RGB_create(int argc, char**argv)
{
	MagickWand *wand;
	int width,height;

  wand=NULL;
	MagickWandGenesis();
	wand = NewMagickWand();
	MagickReadImage(wand,argv[1]);

	width = MagickGetImageWidth(wand);
	height = MagickGetImageHeight(wand);

	if((width /= 2) < 1)width = 1;
	if((height /= 2) < 1)height = 1;
  while(width>WIDTH&&height>HEIGHT){
    width=width/2;
    height=height/2;

  int i,j;
  char*pixelcolour;
  PixelWand*colour;
  char array[HEIGHT][WIDTH];
  colour=NewPixelWand();
  for(i=0;i<HEIGHT;i++){
    for(j=0;j<WIDTH;j++){
      array[i][j]=MagickGetImagePixelColor(wand, i, j, colour);
      pixelcolour=PixelGetColorAsString(colour);
      printf("%s", pixelcolour);
    }
  }
  printf("\n");
  }
}
