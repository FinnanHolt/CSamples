#include "testheadersdl2.h"
#include <assert.h>

int main (void)
{
  unsigned int ** array;
  FILE*fp;
  fp = fopen("test.m7", "rb");

  printf("Starting Teletext Tests...\n");

  array=test_array_creation(fp);
  test_reset_data(array);
  test_mode_select(array);
  test_char_colour(array);
  free_array(array);
  test_graphics_mode();
  printf("All Teletext Tests Passed!\n");

  fclose(fp);

  return 0;
}
unsigned int ** test_array_creation(FILE*fp)
{
  int i,j;
  unsigned int**array;

  array = array_creation(fp);

  /*Check first and last values are as expected*/
  assert(array[0][0]==160);
  assert(array[24][39]==181);

  /*Check values are within the correct range*/
  for (i=0; i<HEIGHT; i++){
      for(j=0;j<WIDTH;j++){
        assert(array[i][j]>127&&array[i][j]<256);
        assert(array!=NULL);
      }
    }
    return array;
}
void test_reset_data(unsigned int**array)
{
  int i,j;
  Data data;

  for (i=0; i<HEIGHT; i++){
    reset_data(&data);
    assert(data.background.red==0);
    assert(data.background.green==0);
    assert(data.background.blue==0);
    assert(data.foreground.red==255);
    assert(data.foreground.green==255);
    assert(data.foreground.blue==255);
    assert(data.mode==ALPHA);
    assert(data.hold==OFF);
    assert(data.doubleheight==OFF);
    assert(data.blast==OFF);
    assert(data.holdmem==OFF);
    assert(data.doublemem==OFF);
    for(j=0;j<WIDTH;j++){
    }
  }
  i=0;
  j=0;
  update_data(&data, array[i][j], i, j);
  assert(data.code==160);
  assert(data.coords.i==i);
  assert(data.coords.j==j);

  initialize_location(&data);
  assert(data.location==DEFAULT);

}
void test_mode_select(unsigned int**array)
{
  int i,j;
  Data data;

  for (i=0; i<HEIGHT; i++){
      for(j=0;j<WIDTH;j++){

        data.code=array[i][j];
        data.location=DEFAULT;
        data.doublemem=OFF;
        mode_select(&data);
        if (data.code>=REDALPHA&&data.code<=WHITEALPHA){
          assert(data.mode==ALPHA);
        }
        if (data.code==SINGLEH){
          assert(data.doubleheight==OFF);
        }
        if (data.code==DOUBLEH){
          assert(data.doubleheight==ON);
        }
        if (data.code>=REDGRAPHICS&&data.code<=WHITEGRAPHICS){
          assert(data.mode==CONTIGUOUS);
        }
        if (data.code==CONTIGUOUSGRAPHICS){
          assert(data.mode=CONTIGUOUS);
        }
        if (data.code==SEPARATEDGRAPHICS){
          assert(data.mode==SEPARATED);
        }
        if (data.code==HOLDGRAPHICS){
          assert(data.hold==ON);
        }
        if (data.code==RELEASEGRAPHICS){
          assert(data.hold==OFF);
        }
      }
    }
}
void test_char_colour(unsigned int**array)
{
  int i,j;
  Data data;

  for (i=0;i<HEIGHT;i++){
      for(j=0;j<WIDTH;j++){
        data.code=array[i][j];
        char_colour(&data);
        if (data.code==REDALPHA){
          assert(data.foreground.red==255&&
                data.foreground.green==0&&
                data.foreground.blue==0);
        }
        if (data.code==MAGALPHA){
          assert(data.foreground.red==255&&
                data.foreground.green==0&&
                data.foreground.blue==255);
        }
        if (data.code==WHITEGRAPHICS){
          assert(data.foreground.red==255&&
                data.foreground.green==255&&
                data.foreground.blue==255);
        }
        if (data.code==BLACKBACKGROUND){
          assert(data.background.red==0&&
                data.background.green==0&&
                data.background.blue==0);
        }
        if (data.code==NEWBACKGROUND){
          assert(data.background.red==data.foreground.red&&
                data.background.green==data.foreground.green&&
                data.foreground.blue==data.foreground.blue);
        }
      }
  }
}
void test_alpha_mode(unsigned int**array)
{
  int i,j;
  Data data;

  initialize_location(&data);
  for (i=0;i<HEIGHT;i++){
    reset_data(&data);
    for (j=0;j<WIDTH; j++){
      update_data(&data, array[i][j], i, j);
      mode_select(&data);
      if (data.mode==ALPHA&&data.doubleheight==OFF){
        assert(alpha_mode(&data));
      }
      if (data.mode==ALPHA&&data.doubleheight==ON&&data.location==TOP){
        assert(alpha_mode(&data)==1);
      }
      if (data.mode==ALPHA&&data.doubleheight==ON&&data.location==BOTTOM){
        assert(alpha_mode(&data)==2);
      }
    }
  }
}
void test_graphics_mode(void)
{
  Data data;
  SDL_Rect rectangle;

  data.hold=OFF;
  data.mode=CONTIGUOUS;
  data.code=160;
  graphics_mode(&rectangle, &data);
  assert(rectangle.w = FNTWIDTH/WIDTHCONSTANT);
  assert(rectangle.h = FNTHEIGHT/HEIGHTCONSTANT);

  data.code=224;
  data.coords.i=8;
  data.coords.j=10;
  graphics_mode(&rectangle, &data);
  assert(rectangle.x==data.coords.j*FNTWIDTH+FNTWIDTH/WIDTHCONSTANT);
  assert(rectangle.y==data.coords.i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT)));

  data.code=176;
  graphics_mode(&rectangle, &data);
  assert(rectangle.x==data.coords.j*FNTWIDTH);
  assert(rectangle.y==data.coords.i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT)));

  data.code=158;
  graphics_mode(&rectangle, &data);
  assert(rectangle.x==data.coords.j*FNTWIDTH);
  assert(rectangle.y==data.coords.i*FNTHEIGHT+(WIDTHCONSTANT*(FNTHEIGHT/HEIGHTCONSTANT)));

  data.code=196;
  graphics_mode(&rectangle, &data);
  assert(data.blast==ON);
}
