#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 980
#define SPEED 300
#define COEF 2
#define DELAY 10
#define N (WINDOW_WIDTH/COEF)
#define H (WINDOW_HEIGHT/COEF)

int check_cell(int, int[], int[], int[]);
int get_input(int*);
int to_binary(int, int[]);

int main()
{
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)!=0)
  {
    printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
    return 1;
  }

  //Create Window
  SDL_Window* wmain = SDL_CreateWindow
  (
    "Celular Automata",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,WINDOW_HEIGHT,0
  );//SDL_WINDOW_FULLSCREEN_DESKTOP

  if(!wmain)
  {
    printf("Erro ao gerar janela: %s\n",SDL_GetError());
    SDL_DestroyWindow(wmain);
    SDL_Quit();
    return 1;
  }

  //Create Renderer
  Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer *r = SDL_CreateRenderer(wmain,-1,renderer_flags);
  if(!r)
  {
    printf("Erro ao gerar renderizador: %s\n",SDL_GetError());
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(wmain);
    SDL_Quit();
    return 1;
  }

  SDL_Texture *t = SDL_CreateTexture(r,0,SDL_TEXTUREACCESS_TARGET,WINDOW_WIDTH,WINDOW_HEIGHT);
  if(!t)
  {
    printf("Erro ao gerar renderizador: %s\n",SDL_GetError());
    SDL_DestroyTexture(t);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(wmain);
    SDL_Quit();
    return 1;
  }

  //Give color (black) to the renderer's background
  SDL_RenderClear(r);
  SDL_RenderPresent(r);
  SDL_RenderSetScale(r,COEF,COEF);

  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////
  ////////////////////////////SDL/////////////////////////////////////////////

  int close = 0, left=0,right=0, wr, prtctrl=1;

  printf("Wolfram Rule [0-255, -1 for all]:");

  if(scanf("%d",&wr)==0||wr<-1||wr>255)
  {
    printf("Invalid input.\n");
  }

  else if(wr>=0)
  {
    char title[100];
    snprintf(title,sizeof(title),"Wolfram Rule %d",wr);
    SDL_SetWindowTitle(wmain, title);

    int grid[N]={0}, update[N], rules[8];
    grid[N/2]=1;

    int *row = grid;
    int *newrow = update;

    to_binary(wr,rules);

    SDL_SetRenderDrawColor(r,0,0,0,255);
    SDL_RenderClear(r);
    SDL_RenderSetScale(r,COEF,COEF);

    while(!close)
    {
      for(int k = 1; k<=H;++k)
      {

        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
          switch(e.type)
          {
            case SDL_QUIT:
            close = 1;
            exit(1);
          }
        }

        SDL_SetRenderDrawColor(r,0,0,0,255);
        SDL_RenderClear(r);
        SDL_SetRenderTarget(r, t);
        SDL_RenderSetScale(r,COEF,COEF);

        SDL_SetRenderDrawColor(r,255,0,0,255);
        if(prtctrl){SDL_RenderDrawPoint(r,N/2,0);prtctrl=0;}

        for(int i=0;i<N;++i)
        {
          if(check_cell(i,row,newrow,rules))
          {
            SDL_RenderDrawPoint(r,i,k);
          }
        }

        SDL_SetRenderTarget(r, NULL);
        SDL_RenderCopy(r,t,NULL,NULL);
        SDL_RenderPresent(r);
        SDL_Delay(DELAY);

        int *swap = row;
        row = newrow;
        newrow = swap;

        if(k==H)
        {
          SDL_SetRenderTarget(r,t);
          SDL_SetRenderDrawColor(r,0,0,0,255);
          SDL_RenderClear(r);
          SDL_SetRenderTarget(r,NULL);
          SDL_RenderCopy(r,t,0,0);
          SDL_RenderPresent(r);
          k=-1;
          continue;
        }
      }
      break;
    }
  }

  else if(wr==-1)
  {
    for(wr=0;wr<256;++wr)
    {
      char title[100];
      snprintf(title,sizeof(title),"Wolfram Rule %d",wr);
      SDL_SetWindowTitle(wmain, title);

      int grid[N]={0}, update[N], rules[8];
      grid[N/2]=1;
      int *row = grid;
      int *newrow = update;

      to_binary( wr,rules);

      SDL_SetRenderDrawColor(r,0,0,0,255);
      SDL_RenderClear(r);
      SDL_RenderSetScale(r,COEF,COEF);



      while(!left||!right)
      {
        for(int k = 0; k<=H;++k)
        {
          SDL_Event e;
          while(SDL_PollEvent(&e))
          {
            switch(e.type)
            {
              case SDL_QUIT:
              exit(1);
              case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button)
                {
                  case SDL_BUTTON_LEFT:
                    left=1;
                    break;
                  case SDL_BUTTON_RIGHT:
                    right=1;
                    break;
                }
            }
          }
          if(left){continue;}
          if(wr>0&&right){wr-=2;right=0;break;}

          SDL_SetRenderTarget(r,t);
          SDL_RenderSetScale(r,COEF,COEF);
          SDL_SetRenderDrawColor(r,255,0,0,255);SDL_RenderDrawPoint(r,N/2,0);

          for(int i=0;i<N;++i)
          {
            if(check_cell(i,row,newrow,rules))
            {
              SDL_SetRenderDrawColor(r,255,0,0,255);SDL_RenderDrawPoint(r,i,k+1);
            }
          }

          int *swap = row;
          row = newrow;
          newrow = swap;

          SDL_SetRenderTarget(r,NULL);
          SDL_RenderCopy(r,t,NULL,NULL);
          SDL_RenderPresent(r);
          SDL_Delay(DELAY);
        }

        SDL_SetRenderTarget(r,t);
        SDL_SetRenderDrawColor(r,0,0,0,255);
        SDL_RenderClear(r);
        SDL_SetRenderTarget(r,NULL);
        break;
      }
      if(left){left=0;continue;}
      if(right){right=0;continue;}
    }
  }

  SDL_DestroyTexture(t);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(wmain);
  SDL_Quit();

  return 0;
}


//Transform number between 0-255 into an array of its binaries
int to_binary(int n, int bn[])
{
  int aux = n;
  for(int i=0;i<8;++i)
  {
    bn[i]=aux %2;
    aux /=2;
  }
  return 0;
}

int check_cell(int i, int old[], int new[], int rules[])
{
  int c1 = i-1, c2 = i+1;
  if(c1<0){c1+=N;};
  if(c2>=N){c2-=N;};
  int n1=old[c1], n2=old[c2];
  new[i] = 0;
  switch(old[i])
  {
    case 0:
      if(!n1&&!n2&&rules[0])
      {
        new[i]=1;
        return 1;
      }
      if(!n1&&n2&&rules[1])
      {
        new[i]=1;
        return 1;
      }
      if(n1&&!n2&& rules[4])
      {
        new[i]=1;
        return 1;
      }
      if(n1&&n2&&rules[5])
      {
        new[i]=1;
        return 1;
      }
      break;
    case 1:
      if(!n1&&!n2&&rules[2])
      {
        new[i]=1;
        return 1;
      }
      if(!n1&&n2&&rules[3])
      {
        new[i]=1;
        return 1;
      }
      if(n1&&!n2&&rules[6])
      {
        new[i]=1;
        return 1;
      }
      if(n1&&n2&&rules[7])
      {
        new[i]=1;
        return 1;
      }
      break;
  }
  return 0;
}
