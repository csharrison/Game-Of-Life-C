#include <stdio.h> 
#include "SDL/SDL.h"
#define WIDTH 1000
#define HEIGHT 600
#define DEPTH 32


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
        if (SDL_LockSurface(screen) < 0)
        {return;}
    }


    if(SDL_MUSTLOCK(screen))
    {
        SDL_UnlockSurface(screen);
    }
}

int ourmod(int num, int den)
{
    if (num < 0)
    {
        return den + num;
    }
    else if (num >= den)
    {
        return den - num;
    }
    return num;
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
    int grid[dx][dy];
    int new[dx][dy];
    int (*gr)[dy] = grid;
    int (*nw)[dy] = new;
    int (*temp)[dy];
    int black = -1;//black = 1, -1 = red
    int i,j;
    for(i=0;i<dx;i++)
    {
        for(j=0;j<dy;j++)
        {
            grid[i][j] = 0;
        }
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
            gr[(mx/size)][(my/size)] = 1;
            //uncomment this line for some wonderous things 
            //for(i=my;i<(200)+my;i=i+size){gr[mx/size][i/size]=1;rect(screen,(mx/size)*size,(i/size)*size,size,size,250,0,0);}
            rect(screen,(mx/size)*size,(my/size)*size,size,size,250,0,0);
         }
     
     if (pause == -1){
     for(i=0;i<dx;i++)
     {
        for(j=0;j<dy;j++)
        {
            //need to count the neighbors
            int inneri,innerj;
            int count = 0;
            for(inneri=i-1;inneri<i+2;inneri++)
            {
                for(innerj=j-1;innerj<j+2;innerj++)
                {

                    if (! (innerj == j && inneri == i))
                    {
                    if (gr[ourmod(inneri,(WIDTH/size))][ourmod(innerj,(HEIGHT/size))]== 1){count++;}
                    }
                }

            }


            //change the state of old depending 
             if (gr[i][j] == 1)
             {
                if (count==3 ||count == 2)//|| count ==4||count==6||count==7||count==8) 
                   {nw[i][j] = 1;}
                else 
                {
                    nw[i][j]=0;
                    rect(screen,i*size,j*size,size,size,0,0,0);
                }
             }
            else
            {
                if (count==3)// || count==6 ||count==7||count==8) 
                {
                    rect(screen,i*size,j*size,size,size,250,0,0);
                    nw[i][j] = 1;}
                else 
                {
                    nw[i][j]=0;
                }
            }
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
