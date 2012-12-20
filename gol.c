#include <stdio.h> 
#include "SDL/SDL.h"
#include <pthread.h>
#include <errno.h>

#define WIDTH 800
#define HEIGHT 600
#define DEPTH 32

#define MOD(x,y) (x < 0 ? y + x : x >= y? y - x :  x)

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  
    
    //converts the 24 bits (three 8 bit ints) of color to 32 bits (rgb)
    colour = SDL_MapRGB( screen->format, r, g, b );
  
    pixmem32 = (Uint32*) screen->pixels  + (WIDTH*y) + (x);
    *pixmem32 = colour;
}

void rect(SDL_Surface *screen, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
    int i,j;
    for(i=x;i<x+w;i++)
    {
        for(j=y;j<y+h;j++)
        {
            setpixel(screen,i,j,r,g,b);
        }
    }
}
void width(SDL_Surface *screen, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, int width)
{
    int i,j;
    for (i=x;i<x+w;i++)
    {
        if(i <= x+width || i >= x+w-width-1)
        {
            for(j=y;j<y+h;j++)
            {
              setpixel(screen,i,j,r,g,b);
            }
        }
        else
        {
            for(j=y;j<=y+width;j++)
            {  
                setpixel(screen,i,j,r,g,b);
            }
            for(j=y+h-1;j>=y+h-width-1;j--)
            {  
                setpixel(screen,i,j,r,g,b);
            }
        }
    }
    
}


void DrawScreen(SDL_Surface* screen)
{
    if(SDL_MUSTLOCK(screen))
    {
        if (SDL_LockSurface(screen) < 0) return;
    }


    if(SDL_MUSTLOCK(screen))
    {
        SDL_UnlockSurface(screen);
    }
}

int main( int argc, char* argv[])
{
    SDL_Surface *screen;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(WIDTH,HEIGHT,DEPTH,SDL_HWSURFACE);
    
    int keypress = 0;
    int pause = 1;
    int mx, my;
    int mousedown = 0;
    int size =3;
    int dx = WIDTH/size;
    int dy = HEIGHT/size;
    int grid[dy][dx];
    int new[dy][dx];
    int (*gr)[dx] = grid;
    int (*nw)[dx] = new;
    int (*temp)[dy];
    int black = -1;//black = 1, -1 = red
    int i,j;
    for(i=0;i<dy;i++){
        for(j=0;j<dx;j++) grid[i][j] = 0;
    }
    
    
    while(!keypress) 
    {
         while(SDL_PollEvent(&event)) 
         {      
              switch (event.type) 
              {
                  case SDL_QUIT:
	                 keypress = 1;
	                 break;
                  case SDL_KEYDOWN:
                       pause = pause*-1;
                     black = -1 * black;
                       break;
                  case SDL_MOUSEBUTTONDOWN:
                       mousedown = 1;

                       break;
                  case SDL_MOUSEBUTTONUP:
                       mousedown = 0;
                       break;
 
              }
         }
        if(mousedown == 1){
            gr[(my/size)][(mx/size)] = 1;
            //uncomment this line for some wonderous things 
            //for(i=my;i<(400)+my;i=i+size){gr[i/size][mx/size]=1;rect(screen,(mx/size)*size,(i/size)*size,size,size,250,0,0);}
            rect(screen,(mx/size)*size,(my/size)*size,size,size,250,0,0);
         }
     
     if (pause == -1){
     for(i=0;i<dy;i++)
     {
        int count;
        int right = 0;
        //initialize
        int left = gr[dy][dx] + gr[0][dx] + gr[1][dx];
        int middle = gr[dy][0] + gr[0][0] + gr[1][0];

        for(j=0;j<dx;j++)
        {
            right = gr[MOD(i-1,dy)][MOD(j+1,dx)] + gr[i][MOD(j+1,dx)] + gr[MOD(i+1,dy)][MOD(j+1,dx)];

            int this = gr[i][j];
            count = left + middle + right - this;

            //change the state of old depending 
             if (this == 1)
             {
                if (count==3 || count == 2)//|| count ==4||count==6||count==7||count==8) 
                   {nw[i][j] = 1;}
                else 
                {
                    nw[i][j]=0;
                    rect(screen,j*size,i*size,size,size,0,0,0);
                }
             }
            else
            {
                if (count==3  )//|| count==6 ||count==7||count==8) 
                {
                    rect(screen,j*size,i*size,size,size,250,0,0);
                    nw[i][j] = 1;}
                else 
                {
                    nw[i][j]=0;
                }
            }
            left = middle;
            middle = right;
        }
     }
    

    temp = gr;
    gr = nw;
    nw = temp;
    }//end pause
       
        SDL_GetMouseState(&mx,&my);
        SDL_Flip(screen);
    }
    SDL_Quit();
    return 0;
}
