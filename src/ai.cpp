#include "Tableau.h"

ai::ai(int prf)
{
    profondeur = prf;
    noeuds = 0;
}

int ai::minimaxPhase1(Tableau tab,int depth,int& position_ai,bool isMax,int alpha,int beta)
{
    int posMoulin;
    if( depth==profondeur || ( (tab.blanc==9) && tab.noir==9 ) )
        return 2* (tab.nbrMoulOuPion(4,true) - tab.nbrMoulOuPion(3,true))+ (tab.blanc - tab.noir);
    if(isMax){
        int eval,evalMax = -99999;
        for(int i = 0; i < 24; i++){
            if(tab.position[i] == 0){
                tab.blanc++;
                tab.position[i] = 4;
                if(tab.moulin(i,posMoulin,4))
                {
                    for(int j=0;j<24;j++){
                        if(tab.position[j]==3 && !tab.moulin(j,posMoulin,3)){
                            noeuds++;
                            tab.position[j]=0;
                            tab.noir--;
                            eval = minimaxPhase1(tab,depth+1,position_ai,!isMax,alpha,beta);
                            tab.position[j]=3; tab.noir++;
                            if(eval>evalMax){
                                evalMax = eval;
                                position_ai = i;
                            }
                            if (eval>alpha) alpha=eval;
                            if (beta<=alpha) return evalMax;

                        }
                    }
                    tab.position[i]=0;
                    tab.blanc--;
                    continue;
                }
                noeuds++;
                eval = minimaxPhase1(tab,depth+1,position_ai,!isMax,alpha,beta);
                if(eval>evalMax){
                    if(depth==0) position_ai = i;
                    evalMax = eval;
                }
                if (eval>alpha) alpha=eval;
                if (beta<=alpha) return evalMax;
                tab.position[i]=0;
                tab.blanc--;
            }
        }
        return evalMax;
    } else {
        int eval_min = 99999;
        int eval;
        for(int i = 0; i < 24; i++){
            if(tab.position[i] == 0){
                tab.position[i] = 3;
                tab.noir++;
                if(tab.moulin(i,posMoulin,3))
                {
                    for(int j=0;j<24;j++){
                        if(tab.position[j]==4 && !tab.moulin(j,posMoulin,4)){
                            noeuds++;
                            tab.position[j]=0;
                            tab.blanc--;
                            eval = minimaxPhase1(tab,depth+1,position_ai,!isMax,alpha,beta);
                            tab.position[j]=4; tab.blanc++;
                            if(eval<eval_min){
                                eval_min = eval;
                            }
                            if (eval<beta) beta=eval;
                            if (beta<=alpha) return eval_min;
                        }
                    }
                    tab.position[i]=0;
                    tab.noir--;
                    continue;
                }
                noeuds++;
                eval = minimaxPhase1(tab,depth+1,position_ai,!isMax,alpha,beta);
                if(eval<eval_min){
                    eval_min= eval;
                }
                if (eval<beta) beta=eval;
                if (beta<=alpha) return eval_min;
                tab.position[i]=0;
                tab.noir--;
            }
        }
        return eval_min;
    }
}

int ai::minimaxPhase2(Tableau tab,int depth,int& from ,int& to,bool isMax,int alpha,int beta)
{
    int posMoul;
    if(depth==profondeur || tab.blanc == 3 || tab.noir == 2 )
        return 2*(tab.nbrMoulOuPion(2,true) - tab.nbrMoulOuPion(1,true))+ tab.blanc-tab.noir;
    if(isMax){
        int eval , evalMax = -999999;
        for(int i=0;i<24;i++){
            if(tab.position[i]==4){
                for(int j=0;j<24;j++){
                    if(tab.coupPossiblePhase2(i,j) && tab.position[j]==0){
                        tab.position[i] = 0 ; tab.position[j] = 4;
                        if(tab.moulin(j,posMoul,4)){
                            for(int n=0;n<24;n++){
                                if(tab.position[n]==3 && !tab.moulin(n,posMoul,3)){
                                    tab.position[n]=0;tab.noir--;
                                    noeuds++;
                                    eval = minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                                    tab.position[n]=3 ; tab.noir++;
                                    if(eval>evalMax){
                                        evalMax = eval;
                                        if(depth == 0){from = i ; to = j;}
                                    }
                                    if (eval>alpha) alpha=eval;
                                    if (beta<=alpha) return evalMax;
                                }
                            }
                            tab.position[i] = 4 ; tab.position[j]=0;
                            continue;
                        }
                        noeuds++;
                        eval = minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                        tab.position[j]=0;tab.position[i]=4;
                        if(eval>evalMax){
                            evalMax = eval;
                            if(depth == 0) {from=i; to = j;}
                        }
                        if (eval>alpha) alpha=eval;
                        if (beta<=alpha) return evalMax;
                    }
                }
            }
        }
        return evalMax;
    } else {
        int eval,eval_min = 999999;
        if(tab.noir!=3){
            for(int i=0;i<24;i++){
                if(tab.position[i]==3){
                    for(int j=0; j<24; j++){
                        if(tab.coupPossiblePhase2(i,j) && tab.position[j]==0){
                            tab.position[i] = 0; tab.position[j]=3;
                            if(tab.moulin(j,posMoul,3)){
                                for(int n=0;n<24;n++){
                                    if(tab.position[n]==4 && !tab.moulin(n,posMoul,4)){
                                        tab.position[n]=0;tab.blanc--;
                                        noeuds++;
                                        eval = minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                                        tab.position[n]=4 ; tab.blanc++;
                                        if(eval<eval_min){
                                            eval_min = eval;
                                        }
                                        if (eval<beta) beta=eval;
                                        if (beta<=alpha) return eval_min;
                                    }
                                }
                                tab.position[i] = 3 ; tab.position[j]=0;
                                continue;
                                }
                                noeuds++;
                                eval=minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                                tab.position[i]=3 ;tab.position[j]=0;
                                if(eval<eval_min) eval_min=eval;
                                if (eval<beta) beta=eval;
                                if (beta<=alpha) return eval_min;
                        }
                    }
                }
            }
            return eval_min;
        } else {
            for(int i=0;i<24;i++){
                if(tab.position[i]==3){
                    for(int j=0;j<24 ;j++){
                        if(tab.position[j]==0){
                            tab.position[i] = 0; tab.position[j]=3;
                            if(tab.moulin(j,posMoul,3)){
                                for(int n=0;n<24;n++){
                                    if(tab.position[n]==4 && !tab.moulin(n,posMoul,4)){
                                        tab.position[n]=0;tab.blanc--;
                                        noeuds++;
                                        eval = minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                                        tab.position[n]=4 ; tab.blanc++;
                                        if(eval<eval_min){
                                            eval_min = eval;
                                        }
                                        if (eval<beta) beta=eval;
                                        if (beta<=alpha) return eval_min;
                                    }
                                }
                                tab.position[i] = 3 ; tab.position[j]=0;
                                continue;
                            }
                            noeuds++;
                            eval=minimaxPhase2(tab,depth+1,from,to,!isMax,alpha,beta);
                            tab.position[i]=3; tab.position[j]=0;
                            if(eval<eval_min) eval=eval_min;
                            if (eval<beta) beta=eval;
                            if (beta<=alpha) return eval_min;
                        }
                    }
                }
            }
            return eval_min;
        }
    }
}
int ai::minimaxPhase3(Tableau tab,int depth,int& from , int& to, bool isMax,int alpha,int beta)
{
    if(depth == profondeur || tab.blanc == 2 || tab.noir == 2)
        return 2*(tab.nbrMoulOuPion(2,true) - tab.nbrMoulOuPion(1,true))+ (tab.blanc - tab.noir);
    int posMoul;
    if(isMax){
        int eval =0, evalMax = -99999;
        for(int i=0;i<24;i++){
            if(tab.position[i]==4){
                for(int j=0;j<24 ;j++){
                    if(tab.position[j]==0){
                        tab.position[i] = 0; tab.position[j]=4;
                        if(tab.moulin(j,posMoul,4)){
                            for(int n=0;n<24;n++){
                                if(tab.position[n]==3 && !tab.moulin(n,posMoul,3)){
                                    tab.position[n]=0;tab.noir--;
                                    noeuds++;
                                    eval = minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                                    tab.position[n]=3 ; tab.noir++;
                                    if(eval>evalMax){
                                        evalMax = eval;
                                        if(depth == 0) {from=i; to=j;}
                                    }
                                    if (eval>alpha) alpha=eval;
                                    if (beta<=alpha) return evalMax;
                                }
                            }
                            tab.position[i] = 4; tab.position[j]=0;
                            continue;
                        }
                        noeuds++;
                        eval=minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                        tab.position[i]=4; tab.position[j]=0;
                        if(eval>evalMax) {
                            evalMax=eval;
                            if(depth == 0) {from=i;to=j;}
                        }
                        if (eval>alpha) alpha=eval;
                        if (beta<=alpha) return evalMax;
                    }
                }
            }
        }
        return evalMax;
    } else {
        int eval=0 ,eval_min=99999;
        if(tab.noir!=3){
            for(int i=0;i<24;i++){
                if(tab.position[i]==3){
                    for(int j=0; j<24; j++){
                        if(tab.coupPossiblePhase2(i,j) && tab.position[j]==0){
                            tab.position[i] = 0; tab.position[j]=3;
                           if(tab.moulin(j,posMoul,3)){
                                for(int n=0;n<24;n++){
                                    if(tab.position[n]==4 && !tab.moulin(n,posMoul,4)){
                                        tab.position[n]=0;tab.blanc--;noeuds++;
                                        eval = minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                                        tab.position[n]=4 ; tab.blanc++;
                                        if(eval<eval_min){
                                            eval_min = eval;
                                        }
                                        if (eval<beta) beta=eval;
                                        if (beta<=alpha) return eval_min;
                                    }
                                }
                                tab.position[i] = 3 ; tab.position[j]=0;
                                continue;
                                }
                                noeuds++;
                                eval=minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                                tab.position[i]=3 ;tab.position[j]=0;
                                if(eval<eval_min) eval_min=eval;
                                if (eval<beta) beta=eval;
                                if (beta<=alpha) return eval_min;
                        }
                    }
                }
            }
            return eval_min;
        }
        else {
          for(int i=0;i<24;i++){
            if(tab.position[i]==3){
                for(int j=0;j<24 ;j++){
                    if(tab.position[j]==0){
                        tab.position[i] = 0; tab.position[j]=3;
                        if(tab.moulin(j,posMoul,3)){
                            for(int n=0;n<24;n++){
                                if(tab.position[n]==4 && !tab.moulin(n,posMoul,4)){
                                    tab.position[n]=0;tab.blanc--;noeuds++;
                                    eval = minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                                    tab.position[n]=4 ; tab.blanc++;
                                    if(eval<eval_min){
                                        eval_min = eval;
                                    }
                                    if (eval<beta) beta=eval;
                                    if (beta<=alpha) return eval_min;
                                }
                            }
                            tab.position[i] = 3; tab.position[j]=0;
                            continue;
                        }
                        noeuds++;
                        eval=minimaxPhase3(tab,depth+1,from,to,!isMax,alpha,beta);
                        tab.position[i]=3; tab.position[j]=0;
                        if(eval<eval_min) {
                            eval_min=eval;
                        }
                        if (eval<beta) beta=eval;
                        if (beta<=alpha) return eval_min;
                    }
                }
            }
        }
        return eval_min;
        }
    }

}
