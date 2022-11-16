#ifndef JOUEUR_H
#define JOUEUR_H


class joueur
{
    friend class Tableau;
    public:
        joueur(int = 5);
        int minimaxPhase1();
        int minimaxPhase2();
        int minimaxPhase3();

    private:
        int profondeur,noeuds;
};

#endif // JOUEUR_H
