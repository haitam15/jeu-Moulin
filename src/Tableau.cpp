#include "Tableau.h"

Tableau::Tableau(int mode)
{
    for(int i=0;i<24;i++)  position[i]=0;
    blanc = noir = 0 ;
    quelJoueur = (mode==2)?1:3; // 1 si joueur1 , 2 si joueur2  et  3 si joueur , 4 si ai

}

Tableau::Tableau(const Tableau& tab)
{
    for(int i=0;i<24;i++)  position[i]=tab.position[i];
    blanc = tab.blanc ; noir  = tab.noir;
    quelJoueur = tab.quelJoueur;
}

int Tableau::nbrMoulOuPion(int jr,bool moul) // si moulin == true on va calculer le nombre des moulins sinon on calcule le nombres des pions
{
    // queljoueur = jr
    int posiMoul;
    int nbreMoulins = 0 , pions = 0;
    for(int i=0 ; i<24 ; i++){
            if(position[i]==jr && moulin(i,posiMoul,jr)) nbreMoulins++;
            if(position[i]==jr) pions++;
        }
    if(moul) return nbreMoulins;
    // le nombres des moulins n'est pas exacte mais on s'interesse a la difference
    return pions;

}

void Tableau::jouer(int difficulte,int &c, SDL_Surface *screen, SDL_Window *win)
{

    // implementation de la premiere phase
    int posiMoulin,posi,adversaire;
    ai machine=(difficulte==1)?ai(3):ai(6);
    for(int pionJouer = 0 ;pionJouer < 18 ; pionJouer++ )
    {
        if(quelJoueur==1 || quelJoueur==2 || quelJoueur==3)
        {
            adversaire=(quelJoueur==1)?2:(quelJoueur==2)?1:4;
            do{
                posi=afficher_plateau(screen,win,c);
                if (posi==-1)
                    return;
            }while(position[posi]!=0);
            position[posi]=quelJoueur;
            if (quelJoueur==2)
                blanc++;
            else
                noir++;
            if(moulin(posi,posiMoulin,quelJoueur)) // la fonction moulin pour checker est ce qu'il y a un moulin dans une position donné
            {
                do{
                    posi=afficher_plateau(screen,win,c);
                    if (posi==-1)
                        return;
                }while(position[posi]!=adversaire || moulin(posi,posiMoulin ,adversaire));
                position[posi]=0;
                if (quelJoueur==2)
                    noir--;
                else
                    blanc--;
            }
            quelJoueur=adversaire;
        }
        else{
            afficher_plateau(screen,win,c);
            machine.minimaxPhase1(*this,0,posi,true,-99999,99999);
            position[posi]=quelJoueur;
            blanc++;
            if(moulin(posi,posiMoulin,4))
            {
                int aSupp;
                bool testSuppr = false;
                for(int i=0 ; i<24 ; i++)
                {
                    if(position[i]==0 && moulin(i,aSupp,3)){
                        position[aSupp]=0 ; noir--;
                        testSuppr=true;
                        break;
                    }
                }
                if(!testSuppr)
                {
                    for(int i = 0;i<24;i++){
                        if(position[i]==3 && !moulin(i,posiMoulin,3)){
                            position[i]=0; noir--;
                            break;
                        }
                    }
                }
            }
            quelJoueur = 3;
        }
    }
    while(noir!=2 && blanc!=2) // la deuxieme et la 3 eme phase
    {
        if(quelJoueur==1 || quelJoueur==2 || quelJoueur==3)
        {
            adversaire=(quelJoueur==1)?2:(quelJoueur==2)?1:4;
            int from,to;
            do{
                from=afficher_plateau(screen,win,c);
                if (from==-1)
                    return;
            }while(position[from]!=quelJoueur);
            do{
                do{
                    to=afficher_plateau(screen,win,c);
                    if (to==-1)
                        return;
                }while(position[to]!=0);
            }while(!coupPossiblePhase2(from ,to) && (quelJoueur!=2 || blanc!=3) && (quelJoueur==2 || noir!=3));
            position[to] =quelJoueur ; position[from]=0;
            if(moulin(to,posiMoulin,quelJoueur)){
                int aSupp;
                do{
                    aSupp=afficher_plateau(screen,win,c);
                    if (aSupp==-1)
                        return;
                }while(position[aSupp]!=adversaire || moulin(aSupp,posiMoulin,adversaire));
                position[aSupp]=0;
                if (quelJoueur==2)
                    noir--;
                else
                    blanc--;
            }
            quelJoueur=adversaire;
        }
        else{
            if(blanc!=3){
                int from , to;
                afficher_plateau(screen,win,c);
                machine.minimaxPhase2(*this,0,from,to,true,-99999,99999);
                position[from]=0; position[to]=4;
                if(moulin(posi,posiMoulin,4))
                {
                    int aSupp;
                    bool testSuppr = false;
                    for(int i=0 ; i<24 ; i++)
                    {
                        if(position[i]==0 && moulin(i,aSupp,3)){
                            position[aSupp]=0 ; noir--;
                            testSuppr=true;
                            break;
                        }
                    }
                    if(!testSuppr)
                    {
                        for(int i = 0;i<24;i++){
                            if(position[i]==3 && !moulin(i,posiMoulin,3)){
                                position[i]=0; noir--;
                                break;
                            }
                        }
                    }
                }
                quelJoueur = 3;
            }
            else if(blanc==3){
                int from , to;
                afficher_plateau(screen,win,c);
                machine.minimaxPhase3(*this,0,from,to,true,-99999,99999);
                position[from]=0; position[to]=4;
                if(moulin(posi,posiMoulin,4))
                {
                    int aSupp;
                    bool testSuppr = false;
                    for(int i=0 ; i<24 ; i++)
                    {
                        if(position[i]==0 && moulin(i,aSupp,3)){
                            position[aSupp]=0 ; noir--;
                            testSuppr=true;
                            break;
                        }
                    }
                    if(!testSuppr)
                    {
                        for(int i = 0;i<24;i++){
                            if(position[i]==3 && !moulin(i,posiMoulin,3)){
                                position[i]=0; noir--;
                                break;
                            }
                        }
                    }
                }
                quelJoueur = 3;
            }
        }
    }
    int winner;
    if (quelJoueur==1 || quelJoueur==2)
        winner=(noir==2)?2:1;
    else
        winner=(noir==2)?4:3;
    SDL_Delay(800);
    SDL_Surface *rejouer=IMG_Load("..\\image\\rejouer.png"),*score=NULL;
    switch (winner)
    {
    case 1 :
        score=IMG_Load("..\\image\\player1_win.png");
        SDL_BlitSurface(score,NULL,screen,NULL);
        SDL_UpdateWindowSurface(win);
        SDL_Delay(2000);
        SDL_BlitSurface(rejouer,NULL,score,NULL);
        break;
    case 2 :
        score=IMG_Load("..\\image\\player2_win.png");
        SDL_BlitSurface(score,NULL,screen,NULL);
        SDL_UpdateWindowSurface(win);
        SDL_Delay(2000);
        SDL_BlitSurface(rejouer,NULL,score,NULL);
        break;
    case 3 :
        score=IMG_Load("..\\image\\player_win.png");
        SDL_BlitSurface(score,NULL,screen,NULL);
        SDL_UpdateWindowSurface(win);
        SDL_Delay(2000);
        SDL_BlitSurface(rejouer,NULL,score,NULL);
        break;
    case 4 :
        score=IMG_Load("..\\image\\player_lose.png");
        SDL_BlitSurface(score,NULL,screen,NULL);
        SDL_UpdateWindowSurface(win);
        SDL_Delay(2000);
        SDL_BlitSurface(rejouer,NULL,score,NULL);
        break;
    }
    bool cond=true;
    SDL_Event event;
    while(cond)
    {
        SDL_BlitSurface(score,NULL,screen,NULL);
        SDL_UpdateWindowSurface(win);
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT :
	            c=0;
                break;
            case SDL_MOUSEMOTION :
                if ( 630<=event.motion.x && event.motion.x<=769 && 427<=event.motion.y && event.motion.y<=533 )
                    {
                        SDL_Surface *rejouer1=IMG_Load("..\\image\\rejouer1.png");
                        SDL_Rect rect={630,427,139,106};
                        SDL_BlitSurface(rejouer1,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                cond=false; //ReJouer
                                SDL_FreeSurface(rejouer);
                                SDL_FreeSurface(score);
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 630<=event.motion.x && event.motion.x<=769 && 427<=event.motion.y && event.motion.y<=533 ))
                                break;
                        }while(1);
                        SDL_FreeSurface(rejouer1);
                    }
                if ( 473<=event.motion.x && event.motion.x<=611 && 435<=event.motion.y && event.motion.y<=533 )
                    {
                        SDL_Surface *rejouer2=IMG_Load("..\\image\\rejouer2.png");
                        SDL_Rect rect={473,435,138,98};
                        SDL_BlitSurface(rejouer2,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=1; //Menu
                                cond=false;
                                SDL_FreeSurface(rejouer);
                                SDL_FreeSurface(score);
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 473<=event.motion.x && event.motion.x<=611 && 435<=event.motion.y && event.motion.y<=533 ))
                                break;
                        }while(1);
                        SDL_FreeSurface(rejouer2);
                    }
                if ( 322<=event.motion.x && event.motion.x<=453 && 437<=event.motion.y && event.motion.y<=532 )
                    {    
                        SDL_Surface *rejouer3=IMG_Load("..\\image\\rejouer3.png");
                        SDL_Rect rect={322,437,131,95};
                        SDL_BlitSurface(rejouer3,&rect,screen,&rect);
                        SDL_UpdateWindowSurface(win);
                        do{
                            if (event.type==SDL_MOUSEBUTTONDOWN)
                            {
                                c=0;  //quitter
                                cond=false;
                                SDL_FreeSurface(rejouer);
                                SDL_FreeSurface(score);
                                break;
                            }
                            SDL_WaitEvent(&event);
                            if ( !( 322<=event.motion.x && event.motion.x<=453 && 437<=event.motion.y && event.motion.y<=532 ))
                                break;
                        }while(1);
                        SDL_FreeSurface(rejouer3);
                    }
                break;
        }
    }
}

bool Tableau::moulin(int posi,int &refe , int jr)
{
    switch(posi)
	{
        case 0:
        {
		if(position[1]==jr && position[2]==jr){
            refe = 1;
            return true;
        }

        if(position[9] == jr && position[21] == jr){
            refe = 9;
            return true;
        }
        return false;
        }
	    case 1:{

		    if(position[0] == jr && position[2] == jr){
                refe = 0;
                return true;
            }

            if(position[4] == jr && position[7] == jr){
                refe = 4; return true;
            }
            return false;
        }
	    case 2:{

		    if(position[1] == jr && position[0] == jr){
                refe = 1;
                return true;
            }

            if(position[14] == jr && position[23] == jr){
                refe = 14; return true;
            }
            return false;
        }
	    case 3:{

		    if(position[4] == jr && position[5] == jr){
                refe = 4;
                return true;
            }

            if(position[10] == jr && position[18] == jr){
                refe = 10; return true;
            }
            return false;
        }
	    case 4:{

		    if(position[3] == jr && position[5] == jr){
                refe = 3;
                return true;
            }

            if(position[1] == jr && position[7] == jr){
                refe = 1; return true;
            }
            return false;
        }
	    case 5:{

		    if(position[4] == jr && position[3] == jr){
                refe = 3;
                return true;
            }

            if(position[13] == jr && position[20] == jr){
                refe = 13; return true;
            }
            return false;
        }
	    case 6:{

		    if(position[7] == jr && position[8] == jr){
                refe = 7;
                return true;
            }

            if(position[11] == jr && position[15] == jr){
                refe = 11; return true;
            }
            return false;
        }
	    case 7:{

		    if(position[4] == jr && position[1] == jr){
                refe = 4;
                return true;
            }

            if(position[6] == jr && position[8] == jr){
                refe = 6; return true;
            }
            return false;
        }
	    case 8:{

		    if(position[12] == jr && position[17] == jr){
                refe = 12;
                return true;
            }

            if(position[7] == jr && position[6] == jr){
                refe = 6; return true;
            }
            return false;
        }
        case 9:{

		    if(position[0] == jr && position[21] == jr){
                refe = 0;
                return true;
            }

            if(position[10] == jr && position[11] == jr){
                refe = 10; return true;
            }
            return false;
        }
        case 10:{

		    if(position[9] == jr && position[11] == jr){
                refe = 9;
                return true;
            }

            if(position[3] == jr && position[18] == jr){
                refe = 3; return true;
            }
            return false;
        }
        case 11:{

		    if(position[6] == jr && position[15] == jr){
                refe = 6;
                return true;
            }

            if(position[10] == jr && position[9] == jr){
                refe = 10; return true;
            }
            return false;
        }
        case 12:{

		    if(position[8] == jr && position[17] == jr){
                refe = 8;
                return true;
            }

            if(position[13] == jr && position[14] == jr){
                refe = 13; return true;
            }
            return false;
        }
        case 13:{

		    if(position[12] == jr && position[14] == jr){
                refe = 12;
                return true;
            }

            if(position[5] == jr && position[20] == jr){
                refe = 5; return true;
            }
            return false;
        }
        case 14:{

		    if(position[2] == jr && position[23] == jr){
                refe = 2;
                return true;
            }

            if(position[13] == jr && position[12] == jr){
                refe = 13; return true;
            }
            return false;
        }
        case 15:{

		    if(position[11] == jr && position[6] == jr){
                refe = 11;
                return true;
            }

            if(position[16] == jr && position[17] == jr){
                refe = 16; return true;
            }
            return false;
        }
        case 16:{

		    if(position[15] == jr && position[17] == jr){
                refe = 15;
                return true;
            }

            if(position[19] == jr && position[22] == jr){
                refe = 19; return true;
            }
            return false;
        }
        case 17:{

		    if(position[15] == jr && position[16] == jr){
                refe = 15;
                return true;
            }

            if(position[12] == jr && position[8] == jr){
                refe = 12; return true;
            }
            return false;
        }
        case 18:{

		    if(position[10] == jr && position[3] == jr){
                refe = 10;
                return true;
            }

            if(position[19] == jr && position[20] == jr){
                refe = 19; return true;
            }
            return false;
        }
        case 19:{

		    if(position[18] == jr && position[20] == jr){
                refe = 18;
                return true;
            }

            if(position[16] == jr && position[22] == jr){
                refe = 16; return true;
            }
            return false;
        }
        case 20:{

		    if(position[19] == jr && position[18] == jr){
                refe = 19;
                return true;
            }

            if(position[13] == jr && position[5] == jr){
                refe = 13; return true;
            }
            return false;
        }
        case 21:{

		    if(position[9] == jr && position[0] == jr){
                refe = 9;
                return true;
            }

            if(position[22] == jr && position[23] == jr){
                refe = 22; return true;
            }
            return false;
        }
        case 22:{

		    if(position[21] == jr && position[23] == jr){
                refe = 21;
                return true;
            }

            if(position[16] == jr && position[19] == jr){
                refe = 16; return true;
            }
            return false;
        }
        case 23:{

		    if(position[22] == jr && position[21] == jr){
                refe = 22;
                return true;
            }

            if(position[14] == jr && position[2] == jr){
                refe = 14; return true;
            }
            return false;
        }
        default :
            break;
}
	return false;
}

bool Tableau::coupPossiblePhase2(int from , int to)
{
	if (from == to)
		return false;
	if (from == 0)
		return( to == 1 || to == 9);
	if (from == 1)
		return(to == 0 || to == 2 || to==4);
	if (from == 2)
		return(to == 1 || to == 14);
	if (from == 3)
		return(to == 4 || to == 10);
	if (from == 4)
		return (to == 3 || to == 5 || to == 1 || to == 7);
	if (from == 5)
		return (to == 4 || to == 13);
	if (from == 6)
		return (to == 7 || to == 11);
	if (from == 7)
		return(to == 4 || to == 6 || to == 8);
	if (from == 8)
		return(to == 12 || to == 7);
	if (from == 9)
		return(to == 0 || to == 10 || to == 21);
	if (from == 10)
		return(to == 9 || to == 11 || to == 3 || to == 18);
	if (from == 11)
		return(to == 6 || to == 10 || to == 15);
	if (from == 12)
		return(to == 8 || to == 13 || to == 17);
	if (from == 13)
		return(to == 12 || to == 14 || to == 5 || to == 20);
	if (from == 14)
		return(to == 2 || to == 23 || to == 13);
	if (from == 15)
		return(to == 11 || to == 16);
	if (from == 16)
		return(to == 15 | to == 17 || to == 19);
	if(from == 17)
		return(to == 16 || to == 12);
	if (from == 18)
		return(to == 10 || to == 19);
	if (from == 19)
		return(to == 18 || to == 20 || to == 16 || to == 22);
	if (from == 20)
		return(to == 19 || to == 13);
	if (from == 21)
		return(to == 9 || to == 22);
	if (from == 22)
		return(to == 21 || to == 19 || to == 23);
	if (from == 23)
		return(to == 22 || to == 14);
	return false;

}

int Tableau::afficher_plateau(SDL_Surface *screen, SDL_Window *win,int &c)
{
    SDL_Surface *plateau=(quelJoueur==1)?IMG_Load("..\\image\\plateau1.png"):(quelJoueur==2)?IMG_Load("..\\image\\plateau2.png"):(quelJoueur==3)?IMG_Load("..\\image\\plateau3.png"):IMG_Load("..\\image\\plateau4.png");
    SDL_Surface *bleu=IMG_Load("..\\image\\bleu.png"), *rouge=IMG_Load("..\\image\\rouge.png");
    SDL_Event event;
    SDL_Rect rect;
    SDL_BlitSurface(plateau,NULL,screen,NULL);
    if (position[0]==1 || position[0]==3)
    {
        rect={422,81,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[0]==2 || position[0]==4)
    {
        rect={422,81,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
        if (position[1]==1 || position[1]==3)
    {
        rect={648,81,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[1]==2 || position[1]==4)
    {
        rect={648,81,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[2]==1 || position[2]==3)
    {
        rect={875,81,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[2]==2 || position[2]==4)
    {
        rect={875,81,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[3]==1 || position[3]==3)
    {
        rect={496,155,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[3]==2 || position[3]==4)
    {
        rect={496,155,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[4]==1 || position[4]==3)
    {
        rect={648,155,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[4]==2 || position[4]==4)
    {
        rect={648,155,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[5]==1 || position[5]==3)
    {
        rect={800,155,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[5]==2 || position[5]==4)
    {
        rect={800,155,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[6]==1 || position[6]==3)
    {
        rect={572,232,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[6]==2 || position[6]==4)
    {
        rect={572,232,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[7]==1 || position[7]==3)
    {
        rect={648,232,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[7]==2 || position[7]==4)
    {
        rect={648,232,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[8]==1 || position[8]==3)
    {
        rect={724,232,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[8]==2 || position[8]==4)
    {
        rect={724,232,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[9]==1 || position[9]==3)
    {
        rect={422,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[9]==2 || position[9]==4)
    {
        rect={422,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[10]==1 || position[10]==3)
    {
        rect={496,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[10]==2 || position[10]==4)
    {
        rect={496,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[11]==1 || position[11]==3)
    {
        rect={572,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[11]==2 || position[11]==4)
    {
        rect={572,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[12]==1 || position[12]==3)
    {
        rect={724,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[12]==2 || position[12]==4)
    {
        rect={724,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[13]==1 || position[13]==3)
    {
        rect={800,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[13]==2 || position[13]==4)
    {
        rect={800,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[14]==1 || position[14]==3)
    {
        rect={875,306,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[14]==2 || position[14]==4)
    {
        rect={875,306,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[15]==1 || position[15]==3)
    {
        rect={572,383,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[15]==2 || position[15]==4)
    {
        rect={572,383,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[16]==1 || position[16]==3)
    {
        rect={648,383,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[16]==2 || position[16]==4)
    {
        rect={648,383,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[17]==1 || position[17]==3)
    {
        rect={724,383,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[17]==2 || position[17]==4)
    {
        rect={724,383,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[18]==1 || position[18]==3)
    {
        rect={496,460,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[18]==2 || position[18]==4)
    {
        rect={496,460,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[19]==1 || position[19]==3)
    {
        rect={648,460,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[19]==2 || position[19]==4)
    {
        rect={648,460,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[20]==1 || position[20]==3)
    {
        rect={800,460,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[20]==2 || position[20]==4)
    {
        rect={800,460,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[21]==1 || position[21]==3)
    {
        rect={422,534,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[21]==2 || position[21]==4)
    {
        rect={422,534,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[22]==1 || position[22]==3)
    {
        rect={648,534,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[22]==2 || position[22]==4)
    {
        rect={648,534,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    if (position[23]==1 || position[23]==3)
    {
        rect={875,534,50,50};
        SDL_BlitSurface(bleu,NULL,screen,&rect);
    }
    else if (position[23]==2 || position[23]==4)
    {
        rect={875,534,50,50};
        SDL_BlitSurface(rouge,NULL,screen,&rect);
    }
    SDL_UpdateWindowSurface(win);
    if (quelJoueur==4)
    {
        SDL_Delay(1000);
        return 0;
    }
    while(1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                c=0;
                return -1;
            case SDL_MOUSEBUTTONDOWN :
                if ( 992<=event.button.x && event.button.x<=1084 && 247<=event.button.y && event.button.y<=351 )
                    {
                        c=1;
                        SDL_FreeSurface(plateau);
                        SDL_FreeSurface(rouge);
                        SDL_FreeSurface(bleu);
                        return -1;
                    } //Menu
                else if ( 992<=event.button.x && event.button.x<=1084 && 339<=event.button.y && event.button.y<=432 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return -1; //Rejouer
                }
                else if ( 992<=event.button.x && event.button.x<=1084 && 154<=event.button.y && event.button.y<=247 )
                    {
                        c=0;
                        SDL_FreeSurface(plateau);
                        SDL_FreeSurface(rouge);
                        SDL_FreeSurface(bleu);
                        return -1;
                    } //Quitter
                else if ( 422<=event.button.x && event.button.x<=472 && 81<=event.button.y && event.button.y<=111 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 0;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 81<=event.button.y && event.button.y<=111 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 1;
                }
                else if ( 875<=event.button.x && event.button.x<=925 && 81<=event.button.y && event.button.y<=111 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 2;
                }
                else if ( 496<=event.button.x && event.button.x<=546 && 155<=event.button.y && event.button.y<=205 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 3;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 155<=event.button.y && event.button.y<=205 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 4;
                }
                else if ( 800<=event.button.x && event.button.x<=850 && 155<=event.button.y && event.button.y<=205 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 5;
                }
                else if ( 572<=event.button.x && event.button.x<=622 && 232<=event.button.y && event.button.y<=282 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 6;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 232<=event.button.y && event.button.y<=282 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 7;
                }
                else if ( 724<=event.button.x && event.button.x<=774 && 232<=event.button.y && event.button.y<=282 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 8;
                }
                else if ( 422<=event.button.x && event.button.x<=472 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 9;
                }
                else if ( 496<=event.button.x && event.button.x<=546 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 10;
                }
                else if ( 572<=event.button.x && event.button.x<=622 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 11;
                }
                else if ( 724<=event.button.x && event.button.x<=774 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 12;
                }
                else if ( 800<=event.button.x && event.button.x<=850 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 13;
                }
                else if ( 875<=event.button.x && event.button.x<=925 && 306<=event.button.y && event.button.y<=356 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 14;
                }
                else if ( 572<=event.button.x && event.button.x<=622 && 383<=event.button.y && event.button.y<=433 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 15;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 383<=event.button.y && event.button.y<=433 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 16;
                }
                else if ( 724<=event.button.x && event.button.x<=774 && 383<=event.button.y && event.button.y<=433 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 17;
                }
                else if ( 496<=event.button.x && event.button.x<=546 && 460<=event.button.y && event.button.y<=510 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 18;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 460<=event.button.y && event.button.y<=510 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 19;
                }
                else if ( 800<=event.button.x && event.button.x<=850 && 460<=event.button.y && event.button.y<=510 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 20;
                }
                else if ( 422<=event.button.x && event.button.x<=472 && 534<=event.button.y && event.button.y<=584 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 21;
                }
                else if ( 648<=event.button.x && event.button.x<=698 && 534<=event.button.y && event.button.y<=584 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 22;
                }
                else if ( 875<=event.button.x && event.button.x<=925 && 534<=event.button.y && event.button.y<=584 )
                {
                    SDL_FreeSurface(plateau);
                    SDL_FreeSurface(rouge);
                    SDL_FreeSurface(bleu);
                    return 23;
                }
        }
    }
}