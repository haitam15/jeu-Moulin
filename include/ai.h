#ifndef AI_H
#define AI_H
#include "Tableau.h"

class ai
{
    public:
        //friend class Tableau;
        ai(int = 4);
        int minimaxPhase1(Tableau ,int,int& ,bool);
        int minimaxPhase2(Tableau ,int ,int& ,int& ,bool);
        int minimaxPhase3(Tableau ,int ,int& ,int& ,bool);

    private:
        int profondeur,noeuds;
};

#endif // AI_H
