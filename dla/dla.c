#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neillncurses.h"

#define MIDPOINT 24
#define NORTH 101
#define EAST 102
#define SOUTH 103
#define WEST 104
#define SIZE 50
#define PARTICLECOUNT 250

/* coordinates.i is equivalent to the 'y' axis and
coordinates.j is equivalent to the 'x' axis*/
struct coordinates
{
    int i;
    int j;
};


struct coordinates spawn(void);
int hashtest (char grid[SIZE][SIZE], struct coordinates particle);
int direction (void);
struct coordinates targeting(struct coordinates particle, int a);
void swap(char grid[SIZE][SIZE], struct coordinates particle, struct coordinates target);

int main (void)
{
    NCURS_Simplewin sw;

    /* Grid initialisation */
    int i, j, a;
    char grid[SIZE][SIZE];
    struct coordinates particle;
    struct coordinates target;
    srand(time(NULL));

    for (i=0; i<SIZE; i++)
    {
        for (j=0; j<SIZE; j++)
        {
            if (i==MIDPOINT && j==MIDPOINT)
            {
                grid[i][j]='#';
            }
            else
            {
                grid[i][j]=' ';
            }
        }
    }
    
Neill_NCURS_Init(&sw);
/* For the character '#', use red foreground colour, red background,
     and the alternative character set */
Neill_NCURS_CharStyle(&sw, "#", COLOR_RED, COLOR_RED, A_NORMAL);
/* For spaces, use black foreground colour, black background,
     and the alternative character set */
Neill_NCURS_CharStyle(&sw, " ", COLOR_BLACK, COLOR_BLACK, A_NORMAL);

    for (i=0; i<250; i++)
        {
            particle=spawn();
            grid[particle.i][particle.j]='#';
            target=particle;
    
            while ((a=hashtest(grid, target))==0)
            {
                target=targeting(target, direction());
                swap(grid, particle, target);
                particle=target;
            }
            Neill_NCURS_PrintArray(&grid[0][0], SIZE, SIZE, &sw);
            /*  Delay in 1/1000s of a second */
            Neill_NCURS_Delay(10);
            /* Test for mouse click, or ESC key */
            Neill_NCURS_Events(&sw);
        }
do 
{
    Neill_NCURS_Events(&sw);
}
while(!sw.finished);

/* Call this function if we exit() anywhere in the code */
atexit(Neill_NCURS_Done);
exit(EXIT_SUCCESS);

return 0;
}

/* Spawns a particle at a random location on the border of the grid*/
struct coordinates spawn(void)
{
    struct coordinates particle;
    
    switch(rand()%4)
    {
        /* Random number at Western border*/
        case 0  :
        particle.i=0;
        particle.j=rand()%SIZE;
        return particle;    
        break;
        
        /* Random number at Eastern border*/
    	case 1  :
    	particle.i=SIZE-1;
        particle.j=rand()%SIZE;
        return particle;
        break;

        /* Random number at Northern border*/
        case 2  :
        particle.i=rand()%SIZE;
        particle.j=0;
        return particle;
        break;

        /* Random number at Southern border*/
        default :
        particle.i=rand()%SIZE;
        particle.j=SIZE-1;
        return particle;
  
    }
}
/* Test to see if a hash is adjacent to the current particle*/
int hashtest(char grid[SIZE][SIZE], struct coordinates particle)
{
    /* Modulo is used to implement the tortoidal nature of the grid */
        if (grid[(particle.i+(SIZE-1))%SIZE][particle.j]=='#')
        {
            return 401;
        }
        else if (grid[particle.i][(particle.j+(SIZE-1))%SIZE]=='#')
        {
            return 402;
        }
        else if (grid[(particle.i+1)%SIZE][particle.j]=='#')
        {
            return 403;
        }
        else if (grid[particle.i][(particle.j+1)%SIZE]=='#')
        {
            return 404;
        }
        else
        {
            return 0;
        }
}

int direction (void)
{
    switch(rand()%4)
    {
        case 0  :
          return NORTH;
          break;
          
    	case 1  :
          return EAST;
          break;

        case 2  :
          return SOUTH;
          break;

        default :
        return WEST;
    }
}
/* Finds the coordinates of a particle in the given direction */
struct coordinates targeting(struct coordinates particle, int a)
{
    /* Modulo is used to implement the tortoidal nature of the grid */
    struct coordinates target=particle;
      {
        if (a==WEST)
        {
            target.i=(((particle.i)-1)+SIZE)%SIZE;
            return target;
        }
        else if (a==EAST)
        {
            target.i=((particle.i)+1)%SIZE;
            return target;
        }
        else if (a==NORTH)
        {
            target.j=(((particle.j)-1)+SIZE)%SIZE;
            return target;
        }
        else
        {
            target.j=((particle.j)+1)%SIZE;
            return target;
        }
    }
}

void swap(char grid[SIZE][SIZE], struct coordinates particle, struct coordinates target)
{
    char temp = grid[particle.i][particle.j];
    grid[particle.i][particle.j] = grid[target.i][target.j];
    grid[target.i][target.j] = temp;
}
