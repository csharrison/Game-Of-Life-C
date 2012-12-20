#include <stdio.h> 
#include "SDL/SDL.h"
#include <pthread.h>
#include <errno.h>

//#define WIDTH 800
//#define HEIGHT 600
#define DEPTH 32

#define GWIDTH (256*3)
#define GHEIGHT 800
#define SIZE 1
#define WIDTH (256*3)//200*3
#define MOD(x,y) (x < 0 ? y + x : x % y)

void rect(SDL_Surface *screen, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b){
    SDL_Rect rect = {x,y,w,h};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r,g,b));
}

void generate_region(SDL_Surface *screen, int starty, int dy, int old[GHEIGHT][GWIDTH], int new[GHEIGHT][GWIDTH]){
    int x,y;
    for(y = starty; y < starty+dy ; y++){
        int count,left,middle,right;

        int ym,yp;
        ym = MOD((y - 1), GHEIGHT);
        yp = MOD((y + 1), GHEIGHT);
        left =   old[ym][GWIDTH-1] + old[y][GWIDTH-1] + old[yp][GWIDTH-1];
        middle = old[ym][0]        + old[y][0]        + old[yp][0];
        for(x = 0; x < GWIDTH ; x++){
            int nx = MOD((x+1), GWIDTH);
            right = old[ym][nx] + old[y][nx] + old[yp][nx];

            int this = old[y][x];
            count = left + middle + right - this;

            //change the state of old depending 
             if (this == 1)
             {
                if (count==3 || count == 2)//|| count ==4||count==6||count==7||count==8) 
                   {new[y][x] = 1;}
                else 
                {
                    new[y][x] = 0;
                    rect(screen,x*SIZE,y*SIZE,SIZE,SIZE,0,0,0);
                }
             }
            else
            {
                if (count==3  )//|| count==6 ||count==7||count==8) 
                {
                    rect(screen,x*SIZE,y*SIZE,SIZE,SIZE,250,0,0);
                    new[y][x] = 1;}
                else 
                {
                    new[y][x]=0;
                }
            }
            left = middle;
            middle = right;
        }
    }
}

int main( int argc, char* argv[])
{
    int HEIGHT = SIZE * GHEIGHT;

    SDL_Surface *screen;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(WIDTH,HEIGHT,DEPTH,SDL_HWSURFACE);
    
    int keypress = 0;
    int pause = 1;
    int mx, my;
    int mousedown = 0;

    int grid[GHEIGHT][GWIDTH];
    int new[GHEIGHT][GWIDTH];
    int (*gr)[GWIDTH] = grid;
    int (*nw)[GWIDTH] = new;
    int (*temp)[GWIDTH];
    int black = -1;//black = 1, -1 = red
    int x,y;
    for(y = 0; y< GHEIGHT;y++){
        for(x = 0;x< GWIDTH;x++) grid[y][x] = 0;
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
            gr[(my/SIZE)][(mx/SIZE)] = 1;
            for(int i=my;i<(400)+my;i=i+SIZE){
                gr[MOD(i/SIZE,GHEIGHT)][MOD(mx/SIZE,GWIDTH)]=1;
                rect(screen,MOD(mx/SIZE * SIZE,WIDTH),MOD(i/SIZE * SIZE,HEIGHT),SIZE,SIZE,250,0,0);
            }
            rect(screen,(mx/SIZE)*SIZE,(my/SIZE)*SIZE,SIZE,SIZE,250,0,0);
        }
    int workers = 2;
    if (pause == -1){
        int dy = GHEIGHT/workers;
        for(y = 0;y < GHEIGHT;y+=dy){
            generate_region(screen, y, dy, gr, nw);
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
