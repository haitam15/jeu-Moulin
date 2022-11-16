#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Tableau.h"
#define W 1100
#define H 619

int main(int argc, char **argv)
{
    SDL_Window *win=NULL;
    SDL_Surface *screen=NULL;
    SDL_Surface *menu=IMG_Load("..\\image\\menu.png"),*menu1=IMG_Load("..\\image\\menu1.png"),*menu2=IMG_Load("..\\image\\menu2.png"),*menu3=IMG_Load("..\\image\\menu3.png");
    SDL_Surface *niveau=IMG_Load("..\\image\\niveau.png"),*niveau1=IMG_Load("..\\image\\niveau1.png"),*niveau2=IMG_Load("..\\image\\niveau2.png"),*niveau3=IMG_Load("..\\image\\niveau3.png");
    SDL_Event event;
    SDL_Rect rect;
    int c=1,difficulte=0;
    if (SDL_Init(SDL_INIT_VIDEO)!=0)
        exit(1);
    win=SDL_CreateWindow("Moulin",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_SHOWN);
    screen=SDL_GetWindowSurface(win);
    while(c)
    {
        if (c==1)
        {
            //printf("\n 1- Player 1 vs Player 2 \n 2- player 1 vs machine \n 3- Quitter \n");
            SDL_BlitSurface(menu,NULL,screen,NULL);
            SDL_UpdateWindowSurface(win);
            SDL_WaitEvent(&event);
            switch (event.type)
            {
            case SDL_QUIT :
	            c=0;
                break;
            case SDL_MOUSEMOTION :
                if ( 555<=event.motion.x && event.motion.x<=1062 && 181<=event.motion.y && event.motion.y<=505 )
                    {
                        rect={555,181,507,324};
                        SDL_BlitSurface(menu1,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=2; //Joeur
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 555<=event.motion.x && event.motion.x<=1062 && 181<=event.motion.y && event.motion.y<=505 ))
                                break;
                        }while(1);
                    }
                if ( 24<=event.motion.x && event.motion.x<=531 && 184<=event.motion.y && event.motion.y<=506 )
                    {
                        rect={24,184,507,322};
                        SDL_BlitSurface(menu2,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=4; //Machine
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 24<=event.motion.x && event.motion.x<=531 && 184<=event.motion.y && event.motion.y<=506 ))
                                break;
                        }while(1);
                    }
                if ( 406<=event.motion.x && event.motion.x<=668 && 478<=event.motion.y && event.motion.y<=612 )
                    {    
                        rect={406,478,262,134};
                        SDL_BlitSurface(menu3,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=0;  //quitter
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 406<=event.motion.x && event.motion.x<=668 && 478<=event.motion.y && event.motion.y<=612 ))
                                break;
                        }while(1);
                    }
                break;
            }
        }
        if (c==4)
        {
            difficulte=0;
            SDL_BlitSurface(niveau,NULL,screen,NULL);
            SDL_UpdateWindowSurface(win);
            SDL_WaitEvent(&event);
            switch (event.type)
            {
            case SDL_QUIT :
	            c=0;
                break;
            case SDL_MOUSEMOTION :
                if ( 450<=event.motion.x && event.motion.x<=649 && 258<=event.motion.y && event.motion.y<=353 )
                    {
                        rect={450,258,199,95};
                        SDL_BlitSurface(niveau1,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                difficulte=1; //Facile
                                c=3;
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 450<=event.motion.x && event.motion.x<=649 && 258<=event.motion.y && event.motion.y<=353 ))
                                break;
                        }while(1);
                    }
                if ( 447<=event.motion.x && event.motion.x<=649 && 371<=event.motion.y && event.motion.y<=467 )
                    {
                        rect={447,371,199,96};
                        SDL_BlitSurface(niveau2,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                difficulte=2; //Difficile
                                c=3;
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 447<=event.motion.x && event.motion.x<=649 && 371<=event.motion.y && event.motion.y<=467 ))
                                break;
                        }while(1);
                    }
                if ( 453<=event.motion.x && event.motion.x<=652 && 488<=event.motion.y && event.motion.y<=583 )
                    {
                        rect={453,488,199,95};
                        SDL_BlitSurface(niveau3,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=0; //Quitter
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 453<=event.motion.x && event.motion.x<=652 && 488<=event.motion.y && event.motion.y<=583 ))
                                break;
                        }while(1);
                    }
                break;
            }
        }
        if (c==2 || c==3)
        {
            Tableau T=Tableau(c);
            T.jouer(difficulte,c,screen,win);
            difficulte=0;
        }
    }
    SDL_FreeSurface(menu);
    SDL_FreeSurface(menu1);
    SDL_FreeSurface(menu2);
    SDL_FreeSurface(menu3);
    SDL_FreeSurface(niveau);
    SDL_FreeSurface(niveau1);
    SDL_FreeSurface(niveau2);
    SDL_FreeSurface(niveau3);
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}