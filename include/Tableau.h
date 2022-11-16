#ifndef TABLEAU_H
#define TABLEAU_H
#include <SDL.h>
#include <SDL_image.h>

class Tableau
{
    public:
        friend class ai;
        Tableau(int mode);
        Tableau(const Tableau&);
        bool coupPossiblePhase2(int , int);
        bool moulin(int, int& , int);
        int nbrMoulOuPion(int jr,bool moul);
        void jouer(int difficulte, int &c, SDL_Surface *screen, SDL_Window *win);
        int afficher_plateau(SDL_Surface *screen, SDL_Window *win, int &c);

    private:
        int position[24];
        int blanc,noir;
        int quelJoueur;
};

class ai
{
    public:
        friend class Tableau;
        ai(int = 4);
        int minimaxPhase1(Tableau ,int,int& ,bool,int,int);
        int minimaxPhase2(Tableau ,int ,int& ,int& ,bool,int,int);
        int minimaxPhase3(Tableau ,int ,int& ,int& ,bool,int,int);

    private:
        int profondeur,noeuds;
};

#endif // TABLEAU_H
