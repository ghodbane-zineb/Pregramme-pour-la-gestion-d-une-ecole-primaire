#include "TOVC_headers.h"


/**************************************************************************************/
/*********************  M A C H I N E  A B S T R A I T E   ****************************/
/**************************************************************************************/

//permet de mettre a jour les elements de l'entete du fichier
void aff_entete(TOVC *fichier,int m , int valeur){
    switch(m)
    {
        case 1:
        {
            fichier->entete.nbloc = valeur; // nombre total de blocs allou s dans le fichier
        }break;

        case 2:
        {
            fichier->entete.poslibre = valeur; // la postion libre dans le bloc de queue
        }break;
        case 3:
        {
            fichier->entete.nb_elv_ins = valeur; //nombre d'eleves res ins r s depuis la cr ation du fichier
        }break;
        case 4:
        {
            fichier->entete.nb_car_ins = valeur; //nombre de caract res inseres s depuis la cr ation du fichier
        }break;
        case 5:
        {
            fichier->entete.nb_car_sup = valeur; //nombre de caract res supprim s depuis la cr ation du fichier
        }break;
        case 6:
        {
            fichier->entete.seuil = valeur; // Seuil de reorganisation
        }break;
        default:
            printf("Erreur: Champ d'Entete incorrect!\n");
    };
}

//permet d'obtenir les elements de l'entete du fichier
int entete(TOVC *fichier, int p){

    switch(p)
    {
        case 1:
        {
            return(fichier->entete.nbloc);
        }break;
        case 2:
        {
            return(fichier->entete.poslibre);
        }break;
        case 3:
        {
            return(fichier->entete.nb_elv_ins);
        }break;
        case 4:
        {
            return(fichier->entete.nb_car_ins);
        }break;
        case 5:
        {
            return(fichier->entete.nb_car_sup);
        }break;
        case 6:
        {
            return(fichier->entete.seuil);
        }break;
        default:
            printf("Erreur: Champ d'Entete incorrect!\n");

    };
}

// Lire le ieme bloc du fichier et le mettre dans buf
void liredir(TOVC *fichier, int hh , Buffer *buf){
    rewind(fichier->f);                                                                  // repositionnement au debut de fichier
    fseek( fichier->f , sizeof(T_Entete) + sizeof(Tbloc)*(hh-1) , SEEK_SET);             // positionnement au debut du bloc numero i
    fread(buf,sizeof(Buffer),1,fichier->f);                                            // lecture d'"1" bloc du "fichier" dans buf
}

// Ecrire le buffer dans le ieme bloc du fichier
void ecriredir(TOVC *fichier, int hh, Buffer buf){
    rewind(fichier->f);                                                        // repositionnement au debut de fichier
    fseek(fichier->f, sizeof(T_Entete) + (sizeof(struct Tbloc))*(hh-1) , SEEK_SET);     // positionnement au debut du bloc numero i
    fwrite(&buf, sizeof(Buffer) , 1 , fichier->f );                           //ecriture du contenu du buffer dans le bloc numero i du fichier
}

// proc dure d'ouverture du fichier
TOVC *ouvrir(const char nom_physique[], char mode){

    TOVC *fichier = malloc(sizeof(TOVC));                      // allocation  de la structure du fichier
    int seuil;
    if( (tolower(mode)) == 'a' )                        // mode ancien : ouvrir un ancien fichier
    {
        fichier->f = fopen(nom_physique,"rb+");               // ouverture du fichier binaire en mode Lecture/Ecriture

        if ((fichier->f)==NULL){
            printf ("Erreur d'ouverture du fichier %s en mode 'a' ", nom_physique);
            return NULL;
        }
        else
            fread (&(fichier->entete), sizeof(fichier->entete), 1, fichier->f);   //Lecture de l'entete du fichier dans ((*fichier).entete)
    }
    else{
        if(tolower(mode) == 'n')                   // mode nouveau : cr er un nv fichier
        {

            fichier->f = fopen(nom_physique,"wb+");         // Creation d'un nouveau fichier binaire en mode Lecture/Ecriture
            if ((fichier->f)==NULL){
                printf ("Erreur d'ouverture du fichier %s en mode 'n' ", nom_physique);
                return NULL;
            }

            printf("** Donnez un seuil de reorganisation de ce fichier = %%");
            scanf ("%d", &seuil);
            // initialisation de l'ent te
            aff_entete(fichier,1,0);                          // le fichier est vide si nbloc=0
            aff_entete(fichier,2,0);                         // le premier caract re du fichier correspond a la position libre puisqu'il est nouveau
            aff_entete(fichier,3,0);                        // aucun eleve re n'a encore  t  ins r
            aff_entete(fichier,4,0);                       // aucun eleve n'a ete inséré
            aff_entete(fichier,5,0);                      // aucun caract re n'a encore  t  supprim
            aff_entete(fichier,6,seuil);                 // Ajouter le seuil de r organisation

        }
        else                                                // format d'ouverture incorrecte
        {
            printf("mode d'ouverture incorrect : ouverture impossible");
        }
    }
    return(fichier);                                       // renvoyer la structure créee
}

// procedure de fermeture du fichier
void fermer(TOVC *fichier){
    rewind(fichier->f);                                                // on se positionne au d but du fichier
    fwrite(&(fichier->entete),sizeof(fichier->entete),1,fichier->f);  //on enregistre les modifications effectuees sur l'entete au d but du fichier
    fclose(fichier->f);                                              // fermeture du fichier

}




/**************************************************************************************/
/**************************************************************************************/
/*********************  TRAITEMENT DES CHAINES DE CHARACTERES *************************/
/**************************************************************************************/

void convert_numner_chaine(int numro ,int longeur,char * chaine[longeur])
//convertir un entier en une chaine de caract res (en corrigeons la taille avec des 0 si n c ssaire)
{
    int ii,pos;
    itoa(numro,chaine, 10);
    if (longeur-strlen(chaine)>0) //on a besoin d'une correction de la taille (ajouter des zeros non significatifs)
    {

        char* correc = malloc (strlen(chaine)+1);     // Chaine contenant les 0 ajoutes a droite
        for ( ii = 0; ii < (longeur-strlen(chaine)); ii++){
            correc[ii]='0';
        }
        correc[longeur-strlen(chaine)]='\0';    //Ajouter un caract re de fin de chaine pour dire que c la fin de la chaine
        strcat (correc,chaine);
        strcpy (chaine,correc);


        //printf("  %s", chaine);             // c juste pour voir lr r sultat
        //printf("%d\n",strlen(chaine));
    }
}

/*************************************************************************/
void genere_notes(char*marks)
//genere des notes aleatoirement et les concat nent apr s les avoir corriger de taille
{
    char mark[4];
    int i2=1,note;
    while(i2<9){
        note=(rand()%2000+1); //generer une note=(sous forme d'un entier de 4 chiffres 2 avant la virgule et 2 apres)
        convert_numner_chaine(note,4,mark);
        strcat(marks,mark);//concatener les notes dans une seule chaine de char
        i2++;
    }
    marks[strlen(marks)]='\0';//fin de la chaine
    printf("%s\n",marks);
    //printf("%d",strlen(marks));

}
/***************************************************************************/
void creer_notes_moyenne(char*marks,int*moyenne)
{
    *moyenne=0;
    char mark[4];
    int i2=1,note;
    while(i2<9){
        note=(rand()%2000+1); //generer une note=(sous forme d'un entier de 4 chiffres 2 avant la virgule et 2 apres)
        *moyenne=*moyenne+note;
        convert_numner_chaine(note,4,mark);
        strcat(marks,mark);//concatener les notes dans une seule chaine de char
        i2++;
    }
    marks[strlen(marks)]='\0';//fin de la chaine
    printf("%s\n",marks);
    (*moyenne)=(*moyenne)/8;
    //printf("%d",strlen(marks));
}
/****************************************************************************/
int comparaison(const char chaine1[],const char chaine2[])
//fonction quicompare deux chaines de char alphabetiquement et renvoie -1 si la premere est inferieure par % a la deuxiemme (code ascii)
//                                                                      1 si la premere est siperieure par % a la deuxiemme (code ascii)
//                                                                      0 si la premere est identique a la deuxiemme (code ascii)
{
    int i = 0;
    while((chaine1[i]!='\0') && (chaine2[i]!='\0')){
        if(chaine1[i]!= chaine2[i]){
            if(chaine1[i]<chaine2[i]){
                return -1;
            }
            if(chaine1[i]>chaine2[i]){
                return 1;
            }
        }
        else if(chaine1[i] == chaine2[i]){
                i++;//incrementation
        }


    }
    if((chaine1[i]=='\0') && (chaine2[i]=='\0'))
        return 0;
}



/******************************************************************************/

void Ecrire_chaine(TOVC *fichier,char chaine[],int longueur,int *i,int *j,Buffer *buf )
//procedure pour ecrire une chaine de taille longeure dans le fichier
{


    int k=0;

    for (k=0;k<longueur;k++){

        if ((*j)< TAILLE_BLOC){

            buf->tab[(*j)]=chaine[k];
            (*j)++;

        }
        else{

            aff_entete(fichier,2,(*j));  //pour la premiere fois en chargement  poslibre
            (*j)=0;

            ecriredir(fichier,(*i),*buf);
            (*i)++;
            buf->tab[(*j)]=chaine[k];
            (*j)++;
            aff_entete(fichier,1,(*i));
        }
         aff_entete(fichier,2,(*j));
    }
    ecriredir(fichier,(*i),*buf);


}
/******************************************************************************/
void Ecrire_chaine_wast(TOVC *fichier,char chaine[],int longueur,int *i,int *j,Buffer *buf ){


    int k=0;

    for (k=0;k<longueur;k++){

        if ((*j)< TAILLE_BLOC){

            //char*kkkkk;
            //strcpy(kkkkk,chaine[k]);
            buf->tab[(*j)]=chaine[k];
            (*j)++;

        }
        else{

            //aff_entete(fichier,2,(*j));
            (*j)=0;
            ecriredir(fichier,(*i),*buf);
            (*i)++;
            buf->tab[(*j)]=chaine[k];
            (*j)++;
            //aff_entete(fichier,1,(*i));
        }
    }
    ecriredir(fichier,(*i),*buf);
   // aff_entete(fichier,2,(*j));
    //aff_entete(fichier,1,(*i));
    //buf->tab[*j]='\0';                                                         /*temporary*/
}
/********************************************************************************/
void Affichage_bloc(TOVC * fichier)
//affichage des blocs
{
    const char nomphysique[]= "LINA.bin";
    fichier =ouvrir(nomphysique,'a');
    Buffer buf ; int i;
    printf("\n |--------------------------------------------------------------------------------------------------------|");
    printf("\n |-----------------------------Le contenu des blocs ------------------------------------------------------|");
    printf("\n |--------------------------------------------------------------------------------------------------------|\n");

    if (entete(fichier,1)>0){

        printf ("\n_____________________________________________________");

        for (i=1; i<=entete(fichier,1);i++){
            printf ("\n|________________    B L O C  %d    __________________|\n",i);
            liredir(fichier,i,&buf);
             buf.tab[TAILLE_BLOC]='\0';
             printf("| %s |",buf.tab); //affichage du contenu du bloc
             printf ("\n------------------------------------------------------");

        }
         printf ("\n");
    }
}

/**************************************************************************************************/
void Affichage_bloco(TOVC * fichier)
//affichage des blocs
{
    const char nomphysique[]= "fichier_organise.bin";
    fichier =ouvrir(nomphysique,'a');
    Buffer buf ; int i;
    printf("\n\n Le contenu des blocs ------------------------------------------------------\n");
    if (entete(fichier,1)>0){

        for (i=1; i<=entete(fichier,1);i++){
            liredir(fichier,i,&buf);
            buf.tab[TAILLE_BLOC]='\0';
            printf(" Le contenu du bloc %d : %s\n",i,buf.tab); //affichage du contenu du bloc
        }
    }
}

/**************************************************************************************************/
void Alea_prenoms_genres(int*num_ligne,char pre_gen[])
/* une procdure pour selectionner un prenom aleatoire du fichier des prenoms et genre*/

{
    char  genre;
    char tp[30];

    int raqm=(*num_ligne);
    strcpy(pre_gen,tableau_prenoms_genres[raqm]);//copier le comteni de la case raqm dans pre_gen
    pre_gen[strlen(pre_gen)-1]='\0';//enlever le retour chariot
    int m=strlen(pre_gen);
    genre=(pre_gen)[m-1];


    pre_gen[strlen(pre_gen)-1]='\0';//enlever le genre
    pre_gen[strlen(pre_gen)-1]='\0';// enlever la virgule

    convert_numner_chaine(strlen(pre_gen),2,tp);
    strcat(tp,pre_gen);
    strcpy(pre_gen,tp);


    pre_gen[strlen(pre_gen)]=genre;
    pre_gen[strlen(pre_gen)]='\0';


}//fin de la proc dure
/********************************************************************************/
void lire_chaine(TOVC *fichier,int n,int*i,int*j,char chaine[],Buffer *buf)
//recuperer une chaine de taille n du beuffer a partir du fichier
{

    liredir( fichier,*i,buf);
    sprintf(chaine,"%s",""); //initialiser la chaine à vide
    int k = 0 ; // indice qui nous permettra de parcourir chaine tout en insérant les caractères de buf
    while (k<n){
        if ((*j)<TAILLE_BLOC){
            chaine[k]=buf->tab[(*j)];
            (*j)++;
        }

        else   //passer au prochain bloc
        {

            (*i)++;
            liredir( fichier,*i,buf);
            chaine[k]=buf->tab[(0)];
            (*j)=1;

        }
        k++;

    }
    chaine[n]='\0';     //Mettre fin à la chaine récupérée

    if ((*j)==TAILLE_BLOC){
        (*i)++;
        (*j)=0;
    }
}
/*****************************************************************************************************/
 void test_fin_bloc(int *position,int*bloc)
 //procedure tres utile pour tester a chaque fois qu'on ajoute ou on soustrait de j une valeure si on a depasser la taille du bloc
{
if((*position)>TAILLE_BLOC) //cas de depassement de la taille
  {
    (*bloc)++;
    (*position)=(*position)%TAILLE_BLOC;

  }
 if((*position)<0)       //verification inverse en cas de soustraction
    {
     (*bloc)--;
     (*position)=(*position)+TAILLE_BLOC;
    }



}


/*****************************************************************************************************/
/*l'utilisateur va entrer un entier pour dire quel critere il souhaite de chercher avec !*/

void rechaerche_par_ID(TOVC *fichier,int *trouv,int *i, int *j,int identifiant)
//recherche avec identifiant

{
    Buffer buf;
    const char nomphysique[]= "LINA.bin";
    fichier = ouvrir(nomphysique,'a');

    char first_name[15];
    char last_name[20];
    char prenom[15];
    char nom[20];

    char eff;
    char efff[4];

    char ID[5]; //l'identifiant qu'on cherche
    char IDE[4];//l idebtifiant du fichier

    char taille_nom[3]; //taille du nom en chaine de char
    int TN;//la taille du nom en entier

    char taille_prenom[3]; //taille du prenom en chaine de char
    int TP;//la taille du prenom en entier

    char classe[3];
    char clas[3];
    char salle[3];
    (*trouv)=0;
    (*i)=1;
    (*j)=0;//initialisation des posions
    char iddd[5];
    //(*trouv)=0;//initialisation de trouv a 0
    int stop=0;//booleen pour l'arret de la recharche

            //traitenet de l'identifiant qu'on veut chercher


            convert_numner_chaine(identifiant ,4,ID) ;  //convertir l'identifiant en chaine de char tout en corrigeons sa taille
            //recherche
            strcpy(iddd,ID);
            liredir(fichier,*i,&buf); // lecture du premier bloc afin d'entammer le parcours
            while(((*i)<(entete(fichier,1)+1))&&(stop==0)&&((*trouv)==0))
                {
                lire_chaine(fichier,3,i,j,efff,&buf);
                lire_chaine(fichier,4,i,j,IDE,&buf); //recuperer l'identifiant

                if ((strcmp(iddd,IDE)==0)) //test de condition d'arret
                {
                    if (efff[0]=='0')
                        {
                        (*trouv)=1;
                        }

                }

                lire_chaine(fichier,2,i,j,taille_nom,&buf);
                TN=atoi(taille_nom);
                //printf("\n%d",(*j));
                if( (*trouv) == 1)
                    {
                    lire_chaine(fichier,TN,i,j,last_name,&buf);
                    printf("\n\n L'eleve cherche est trouve !! \n son nom est : %s", last_name);  //affichage du nom

                    }
                    //printf("\n%d",(*j));
                else //avancement sinon
                    {
                    (*j)=(*j)+TN;
                    test_fin_bloc(j,i); //verification

                    }


                lire_chaine(fichier,2,i,j,taille_prenom,&buf);
                //printf("\n%d",(*j));
                TP=atoi(taille_prenom);
                if( (*trouv)==1 )
                {
                    lire_chaine(fichier,TP,i,j,first_name,&buf);
                    printf("                             \n\n son prenom est : %s\n",first_name);  //affichage du prenom
                    (*j)=(*j)-TP-2-TN-2-4-3;//revenir en arriere pour garder la position du debut de l'enreg
                    test_fin_bloc(j,i);
                    printf("\n Sa position est : --> bloc N: %d  position :  %d  \n",*i, *j);

                }
                else
                {

                    (*j)=(*j)+TP+1+32;  //avancer au prochain enregidtrement j=j+taiile du genre(1)+taille de la chaine des notes(8*4=32)

                    test_fin_bloc(j,i);


                    //printf("\n%d",(*j));
                }


            }
            if (trouv==0)
            {
                printf("\n L'eleve cheche n'existe pas !\n");
            }

}



/************************************************************************************************/


void rechaerche_classe(TOVC *fichier,int *trouv,int *i, int *j,char classe[],char nom[20],char prenom[15])
//recherche avec classe+nom+prenom

{
    Buffer buf;
    const char nomphysique[]= "LINA.bin";
    fichier = ouvrir(nomphysique,'a');

    char first_name[15];
    char last_name[20];
    //char prenom[15];
    //char nom[20];

    char eff;
    char efff[4];

    char ID[5]; //l'identifiant qu'on cherche
    char IDE[4];//l idebtifiant du fichier
    int identifiant;

    char taille_nom[3]; //taille du nom en chaine de char
    int TN;//la taille du nom en entier

    char taille_prenom[3]; //taille du prenom en chaine de char
    int TP;//la taille du prenom en entier

    //char classe[3];
    char clas[3];
    char salle[3];
    (*trouv)=0;
    (*i)=1;
    (*j)=0;//initialisation des posions
    char iddd[5];
    //(*trouv)=0;//initialisation de trouv a 0
    int stop=0;//booleen pour l'arret de la recharche

           int b,yesclas=0;

            liredir(fichier,*i,&buf); // lecture du premier bloc afin d'entammer le parcours
            char kaa[3];
            strcpy(kaa,classe);



            while((yesclas==0)&&((*i)<(entete(fichier,1)+1)) )
            {


                lire_chaine(fichier,3,i,j,clas,&buf);
                for(b=1;b<3;b++)
                    {
                    salle[b-1]=clas[b];
                    }

                salle[2]='\0';
                kaa[2]='\0';
                if(/*(clas[0]='0')&&*/(strcmp(salle,kaa) == 0))
                    {                        //laisser la condition de l'effacement pour la fin de la recherche
                    yesclas=1;
                    }
                else{ //avancer jusqu'a trouver la classe
                    (*j)=(*j)+4;
                    test_fin_bloc(j,i);

                    lire_chaine(fichier,2,i,j,taille_nom,&buf);
                    TN=atoi(taille_nom);
                    (*j)=(*j)+TN;
                    test_fin_bloc(j,i);

                    lire_chaine(fichier,2,i,j,taille_prenom,&buf);
                    TP=atoi(taille_prenom);
                    (*j)=(*j)+TP+1+32;
                    test_fin_bloc(j,i);



                }
            }
                while((yesclas==1)&&(stop==0)&&((*trouv)==0)) //boucle de la classe trouvé +recherche du nom et prenom
                    {

                    (*j)=(*j)+4;
                    test_fin_bloc(j,i);

                    lire_chaine(fichier,2,i,j,taille_nom,&buf);
                    TN=atoi(taille_nom);
                    lire_chaine(fichier,TN,i,j,last_name,&buf);
                    while((comparaison(nom,last_name)==1 )&&(yesclas==1)) //tant qu'on est pas arrivée a un nom qui depasse celui recherché alphabetiquement


                       {
                        lire_chaine(fichier,2,i,j,taille_prenom,&buf);
                        TP=atoi(taille_prenom);
                        (*j)=(*j)+TP+1+32+1;   //aller a la fin d l'enregistrement et sauter l'effacement
                        test_fin_bloc(j,i);

                        lire_chaine(fichier,2,i,j,salle,&buf);//verification de la classe
                        salle[2]='\0';
                       if ((strcmp(salle,kaa)!=0))
                       {
                           yesclas=0;
                       }

                        (*j)=(*j)+4;//depasser l'identifiant
                        test_fin_bloc(j,i);

                        lire_chaine(fichier,2,i,j,taille_nom,&buf);
                        TN=atoi(taille_nom);
                        lire_chaine(fichier,TN,i,j,last_name,&buf);

                       }

                       if(comparaison(nom,last_name)==0 && (yesclas=1))    //sortir de la boucle avec la premiere condition =0 <=> ona trouver le nom
                       {

                        lire_chaine(fichier,2,i,j,taille_prenom,&buf);
                        TP=atoi(taille_prenom);
                        lire_chaine(fichier,TP,i,j,first_name,&buf);

                        if(comparaison(prenom,first_name)==0)
                            {
                            if((clas[0]='0'))               //verification de l'effecement
                            {

                            (*trouv)=1;
                            (*j)=(*j)-TP-TN-2-2-4;  //retourner pour afficher l'ID
                            test_fin_bloc(j,i);
                            lire_chaine(fichier,4,i,j,ID,&buf);
                            printf("\n léeleve est trouve don matricule est :  %s",ID);
                            (*j)=(*j)-4-3;         //retourner pour avoir la position du debut de l'enregistrement
                            test_fin_bloc(j,i);
                            }
                            }
                          else{
                             if(comparaison(prenom,first_name)==1)
                         {
                            (*j)=(*j)+1+32;        //sauter le genre + les notes et aller vers l'enregistrement suivant
                            test_fin_bloc(j,i);
                            stop=1;
                            printf("  cette valeure n'existe pas mais on a trouver sa position");
                            printf("\n\n  La position courante: %d %d\n\n ",*i, *j);
                          }
                          else{
                          if(comparaison(prenom,first_name)==-1)
                          {
                            (*j)=(*j)-2-2-TP-TN-4-3;
                            test_fin_bloc(j,i);
                            stop=1;
                            printf("  cette valeure n'existe pas mais on a trouver sa position");
                            printf("\n\n  La position courante: %d %d\n\n ",*i, *j);
                          }
                          }
                          }


                       }
                       else
                       {
                           if((((comparaison(nom,last_name))==(-1))&& (yesclas=1))  ||  (((comparaison(nom,last_name)==(1)) )&&(yesclas==0)) ) //soit on arrive a sa position alphabetquement sans la trouver ou bien on arrive a la fin de la classe (qui est sa position car le nom est superieur a tous les autres)
                       {
                        stop=1;
                        (*j)=(*j)-TN-2-4-3; //revenir pour garder sa position au debut de la recherche
                        test_fin_bloc(j,i);
                        printf("  cette valeure n'existe pas mais on a trouver sa position");
                        printf("\n\n  La position courante: %d %d\n\n ",*i, *j);

                       }
                       }

                }
                if (trouv==0)
            {
                printf("\n L'eleve cheche n'existe pas !\n");
            }



 } //end of recherche






/**************************************************************************************************/
void suppression(TOVC *fichier)
//procedure de suppression logique
{

  int trouv,i,j,identifiant;
  char prenom[15];
  char nom[20];
  char classe[3];
  Buffer buf;
  char eff[2],TN[2],TP[2];
  int T_enreg=7;

  const char nomphysique[]= "LINA.bin";
  fichier = ouvrir(nomphysique,'a');
 int choix;
 printf("voulez vous supprimer un elevee par :\n    1-son identifiant\n    2-sa classe + Nom + Prenom");
 scanf("%d",&choix);
 switch (choix)
            {
          case 1:
            {
               printf("entrez l'identifiant que vous voulez chercher !\n");
               scanf("%d",&identifiant);//recherche avec identifiant
               rechaerche_par_ID(fichier,&trouv,&i,&j,identifiant);

            }break;
          case 2:
            {
              printf("\n\n Entrez la classe de l'étudiant  !\n");
            scanf("%s",classe);
            classe[strlen(classe)]='\0';

            printf("entrer le nom de l'etudiant  :  ");
            scanf("%s",nom);
            nom[strlen(nom)]='\0';

            printf("entrer le prenom de l'etudiant  :  ");
            scanf("%s",prenom);
            prenom[strlen(prenom)]='\0';

            rechaerche_classe(fichier,&trouv,&i,&j,classe,nom,prenom);

            }break;

            }
            if (trouv==1)
            {

            lire_chaine(fichier,2,&i,&j,eff,&buf);
            liredir(fichier,i,&buf);
            eff[0]='1';  //changer le caractere de l'effacement en 1
            j=(j-2);
            test_fin_bloc(&j,&i);
            Ecrire_chaine_wast(fichier,eff,2,&i,&j,&buf);
            //calculons combien de caracteres on a supprimés
            j=j+5;//sauter l'identifiant
            test_fin_bloc(&j,&i);

            lire_chaine(fichier,2,&i,&j,TN,&buf);
            j=j+atoi(TN);
            test_fin_bloc(&j,&i);
            T_enreg=T_enreg+2+atoi(TN);
            lire_chaine(fichier,2,&i,&j,TP,&buf);
            T_enreg=T_enreg+2+atoi(TP)+1+32;
            aff_entete(fichier,5,(entete(fichier,5)+T_enreg));
            printf("\nle nombre de caractere supprimes est : --->%d\n",entete(fichier,5));
            Affichage_bloc(fichier);
            afficher_entete(fichier,nomphysique);
            }
            else
            {
                printf("\n   > L'eleve que vous voulez supprimer n'existe pas !\n");
            }

            fermer(fichier);


}
/*********************************************************************************************/
void insertion(TOVC *fichier,char classe[],char nom[],char prenom[],char genre[2],char ident[4],char notes[32])
//procedure d'insertion d'un enreg a partir de sa classe nom et prenom pour determiner sa position exacte dans la fichier
{
    Buffer buf;
    int i=1,j=0;
    char TN[2];//chaine de caracteres pour la taille du nom
    char TP[2];//chaine de caracteres pour la taille du prenom
    int trouv=0;
    int nouv=0;//booleen en cas d'utilisation d'un nouveau bloc
    char eff[80]="0";//initialisation de la chaine de concatenation avec le 0 de l'effecement
    int k;

    classe[strlen(classe)]='\0'; //mettre fin aux chaines de char
    nom[strlen(nom)]='\0';
    prenom[strlen(prenom)]='\0';
    genre[1]='\0';
    convert_numner_chaine(strlen(nom),2,TN);
    //concatenation dans la chaine eff
    strcat(eff,classe);
    strcat(eff,ident);
    strcat(eff,TN);

     convert_numner_chaine(strlen(prenom),2,TP);
     TP[strlen(TP)]='\0';

     rechaerche_classe(fichier,&trouv,&i,&j,classe,nom,prenom);



     const char nomphysique[]= "LINA.bin";
     fichier = ouvrir(nomphysique,'a');
    //concatenation du tout (pour avoir un enregistrement complet)
    strcat(eff,nom);
    convert_numner_chaine(strlen(prenom),2,TP);
    strcat(eff,TP);
    strcat(eff,prenom);
    strcat(eff,genre);
    strcat(eff,notes);
    eff[strlen(eff)]='\0';

    printf("LE nouveau enregistrement est : %s\n",eff);
    int taille_enreg;
    taille_enreg=strlen(eff);
    if (i>0)
    liredir(fichier,i,&buf);
    if (trouv==0)//la donnée n'existe pas
    {
       aff_entete(fichier,4,entete(fichier,4)+taille_enreg);
       if(entete(fichier,1) <= 0) // le fichier est vide
       {
         for (k = 0; k < taille_enreg; k++) {

               buf.tab[k] = eff[k];
            }
            buf.tab[k] = '\0';
            i = 1;
            aff_entete(fichier,1,i);
            ecriredir(fichier,i,buf);
            aff_entete(fichier,2,k);
            aff_entete(fichier,3,entete(fichier,3)+1);  // mise a jour du nombre d'eleves inseres
            aff_entete(fichier,4,taille_enreg);
       }
       else//le fichier n'est pas vide
       {
          int nbr_char_pas_aff=0,nbr_char_aff=0,nbr=0;
           nbr_char_pas_aff=TAILLE_BLOC*(i-1)+j+1;
          nbr_char_aff=entete(fichier,4)-nbr_char_pas_aff;
          //pour poslibre
          if((entete(fichier,2)+taille_enreg)>TAILLE_BLOC-1)
            {
              aff_entete(fichier,2,((entete(fichier,2)+taille_enreg)%TAILLE_BLOC));
            }
          else//la poslibre ne depasse pas la taille du bloc
            {
              aff_entete(fichier,2,entete(fichier,2)+taille_enreg);
            }
         char sauv[150];//pour sauvegarder le prochin decalage
         int stop=1;
         // printf("le nomre d'eleves chargés est de : %d\n",entete(fichier,3));
         while((nbr<nbr_char_aff)&&(stop==1))
           {

              for (k=0;k<taille_enreg;k++)
                {
                   sauv[k]=buf.tab[j];
                   buf.tab[j]=eff[k];
                   nbr++;
                   if((j)+1==TAILLE_BLOC)
                         {
                            buf.tab[j+1]='\0';
                            j=0;
                            ecriredir(fichier,i,buf);
                             i++;
                             if(i>entete(fichier,1))
                                   {
                                       nouv=1;
                                   }
                            if(nouv==0)
                                  {
                                          liredir(fichier,i,&buf);
                                  }
                        }
                     else
                       {
                           j++;
                       }
                    if((i>=entete(fichier,1))&&(j==entete(fichier,2))) // on arrive a la fin du fichier
                      {
                         stop=0;
                          sauv[k+1] = '\0';
                      }
                 }
               strcpy(eff,sauv);
            }  // end of while

            if(nouv==1)
             aff_entete(fichier,1,i);
          // buf.tab[j] = '\0';
           ecriredir(fichier,i,buf);
       }

    }
    else{printf("la donnée existe deja !!!!!!!!!!!");}



    printf("la position libre dans le dernier bloc est: %d\n",entete(fichier,2));
    printf("le nomre d'eleves chargés est de : %d\n",entete(fichier,3));
    printf("le nomre de caracteres inseres est de : %d\n",entete(fichier,4));
    printf("////////%d",i);
    //Affichage_bloc(fichier);
    fermer(fichier);

}
/******************************************************************************************************/
TOVC* archiv;
void mise_a_jour(TOVC *fichier,int n)//n est un entier     1-->pour modifier une note
{ //il ne faut pas oublier d'augumenter le nombre d'insertions et de suppressions a la fin !******************************************************
const char nomphysique[]= "LINA.bin";
fichier = ouvrir(nomphysique,'a');
 int trouv,i,j,identifiant;

 TOVC* indexe;

 chargement_index(indexe);

 Buffer buf;
 int choix;
 char nom[20]="";
 char nom1[20]="";
 char notes[32];
 char prenom[20]="";
 char prenom1[20]="";
 char classe[2];
 char salle[2];
 char classe1[2];
 char genre[2];
 char nouv_e[100];
 char ident[4]="";
 char eff[2];
 int mat;
 int note;
 char matiere[4];
 char nouv_note[4];
 char TN[2]="";
 char TP[2]="";
 char tn[2]="";
 char tp[2]="";
 char nn[2],p[2];
 int taille_e=7;
 int x;
 char iidd[4];



 printf("voulez vous trouver l'eleve par :\n    1-son identifiant\n    2-sa classe + Nom + Prenom");
 scanf("%d",&choix);
 switch (choix)
            {
          case 1:
            {
               printf("entrez l'identifiant que vous voulez chercher !\n");
               scanf("%d",&identifiant);//recherche avec identifiant
               rechaerche_par_ID(fichier,&trouv,&i,&j,identifiant);

            }break;
          case 2:
            {
              printf("\n\n Entrez la classe de l'étudiant  !\n");
            scanf("%s",classe1);
            classe1[strlen(classe1)]='\0';

            printf("entrer le nom de l'etudiant  :  ");
            scanf("%s",nom1);
            nom1[strlen(nom1)]='\0';

            printf("entrer le prenom de l'etudiant  :  ");
            scanf("%s",prenom1);
            prenom1[strlen(prenom1)]='\0';
            rechaerche_classe(fichier,&trouv,&i,&j,classe1,nom1,prenom1);

            }break;
            }

if (trouv==1)
               {
                  j=j+1;
                  test_fin_bloc(&j,&i);
                  lire_chaine(fichier,2,&i,&j,classe1,&buf);
                  lire_chaine(fichier,4,&i,&j,iidd,&buf);
                  test_fin_bloc(&j,&i);
                  liredir(fichier,i,&buf);
                  lire_chaine(fichier,2,&i,&j,nn,&buf);
                  x=atoi(nn);
                  j=j+x;
                  test_fin_bloc(&j,&i);
                  taille_e=taille_e+2+atoi(nn);
                  lire_chaine(fichier,2,&i,&j,p,&buf);
                  taille_e=taille_e+2+atoi(p)+1+32;
                  j=j-2-x-2-7;
                  test_fin_bloc(&j,&i);





  switch(n)
  {
  case 1:       //mise a jour d'une note
    {
        char note1[5],note2[5],note3[5],note4[5],note5[5],note6[5],note7[5],note8[5];

     printf("\n veillez entrer le numero de la matiere que vous voulez mettre a jour \n") ;
     scanf("%d",&mat);
     printf("\n veillez entrer la nouvelle note:2 numeros avant la virgule et 2 apres(sans ecrire la virgule)\n") ;
     scanf("%d",&note);
     //note=note*100;
     convert_numner_chaine(note,4,nouv_note);
     char mal[4];
     strcpy(mal,nouv_note);
     j=j+7;
     test_fin_bloc(&j,&i);
     lire_chaine(fichier,2,&i,&j,TN,&buf);
     j=j+atoi(TN);
     test_fin_bloc(&j,&i);
     lire_chaine(fichier,2,&i,&j,TP,&buf);
     j=j+atoi(TP)+(mat-1)*4;
     test_fin_bloc(&j,&i);
     Ecrire_chaine(fichier,mal,4,&i,&j,&buf); //mise a jour
     j=j-4-(mat-1)*4;
     test_fin_bloc(&j,&i);
     lire_chaine(fichier,4,&i,&j,note1,&buf);  //on récupère les notes de toutes les matières
     lire_chaine(fichier,4,&i,&j,note2,&buf);
     lire_chaine(fichier,4,&i,&j,note3,&buf);
     lire_chaine(fichier,4,&i,&j,note4,&buf);
     lire_chaine(fichier,4,&i,&j,note5,&buf);
     lire_chaine(fichier,4,&i,&j,note6,&buf);
     lire_chaine(fichier,4,&i,&j,note7,&buf);
     lire_chaine(fichier,4,&i,&j,note8,&buf);
     int moy;
     moy=(atoi(note1)+atoi(note2)+atoi(note3)+atoi(note4)+atoi(note5)+atoi(note6)+atoi(note7)+atoi(note8))/8;
     char chmoy[5];
     convert_numner_chaine(moy,4,chmoy);

     Mise_a_jour_archiv(archiv,iidd,classe1,chmoy,1,"");
     fichier_index(indexe);




    }break;
  case 2:  //mise a jour du nom et du prenom
    {

      printf("\n veillez entrer la mise a jour du :\n (remarque :si il n'ya pas de mise a jour dans l'un des deux veiller le reecrire): \n") ;
      printf("---------> 1 : NOM");
      scanf("%s",nom);
      char nommm[20];
      strcpy(nommm,nom);
      printf("\n---------> 2 : PRENOM") ;
      scanf("%s",prenom);
      char prenommm[20];
      strcpy(prenommm,prenom);
      nom[strlen(nom)]='\0';
      prenom[strlen(prenom)]='\0';
      j++;
      test_fin_bloc(&j,&i);
      lire_chaine(fichier,2,&i,&j,classe,&buf);
      lire_chaine(fichier,4,&i,&j,ident,&buf);
      char identt[4];
      strcpy(identt,ident);
      lire_chaine(fichier,2,&i,&j,TN,&buf);
      j=j+atoi(TN);
      char tn[2];
      strcpy(tn,TN);
      test_fin_bloc(&j,&i);
      lire_chaine(fichier,2,&i,&j,TP,&buf);
      j=j+atoi(TP);
      test_fin_bloc(&j,&i);
      lire_chaine(fichier,1,&i,&j,genre,&buf);
      lire_chaine(fichier,32,&i,&j,notes,&buf);
      j=j-32-1-2-2-atoi(TP)-atoi(tn)-7;
      strcpy(identt,ident);
      test_fin_bloc(&j,&i);
      //SUPPRIMONS L'ancien enregistrement
      lire_chaine(fichier,2,&i,&j,eff,&buf);
      eff[0]='1';
      j=(j-2);
      test_fin_bloc(&j,&i);
      Ecrire_chaine_wast(fichier,eff,2,&i,&j,&buf);
      aff_entete(fichier,5,entete(fichier,5)+taille_e);//mise a jour du nombre de caractere supprimes
      printf("\n le nombre de caracteres supprimées est     :    %d",entete(fichier,5));
      fermer(fichier);
      //insertion du nouveau enregistrement
      insertion(fichier,classe,nom,prenom,genre,identt,notes);


        }break;
       case 3:
    {
        char cl[3];
     printf("\n entrer la nouvelle classe : ");
     scanf("%s",&salle);
     salle[strlen(salle)]='\0';
     j=j+1;
     test_fin_bloc(&j,&i);
     lire_chaine(fichier,2,&i,&j,cl,&buf);

      lire_chaine(fichier,4,&i,&j,ident,&buf);
      char identt[4];
      strcpy(identt,ident);
      lire_chaine(fichier,2,&i,&j,TN,&buf);
      strcpy(tn,TN);
      tn[2]='\0';
      int x;
      x=atoi(tn);
      //strcpy(tn,TN);
      lire_chaine(fichier,atoi(TN),&i,&j,nom,&buf);
      nom[strlen(nom)]='\0';
      lire_chaine(fichier,2,&i,&j,TP,&buf);
      lire_chaine(fichier,atoi(TP),&i,&j,prenom,&buf);
      lire_chaine(fichier,1,&i,&j,genre,&buf);
      lire_chaine(fichier,32,&i,&j,notes,&buf);
      j=j-32;
      j=j-1;
      j=j-2-2;
      j=j-x;
      j=j-atoi(TP);
      j=j-7;
      test_fin_bloc(&j,&i);
      //SUPPRIMONS L'ancien enregistrement logiquement
      lire_chaine(fichier,2,&i,&j,eff,&buf);
      eff[0]='1';
      j=(j-2);
      test_fin_bloc(&j,&i);
      Ecrire_chaine_wast(fichier,eff,2,&i,&j,&buf);
      aff_entete(fichier,5,entete(fichier,5)+taille_e);//mise a jour du nombre de caractere supprimes
      fermer(fichier);
      //insertion du nouveau enregistrement
      insertion(fichier,salle,nom,prenom,genre,identt,notes);
      char mo[5]="";
      Mise_a_jour_archiv(archiv,ident,cl,mo,2,salle);
      fichier_index(indexe);




    }break;
      }
               }
               else
               {
                   printf("\nl'element cherché n'est pas trouvé !!! \n");
               }
  fermer(fichier);
}


/*******************************************************************************************************/
 // Affichage de l'entete du fichier vu comme LOVC
void afficher_entete(TOVC *fichier, const char *nom_physique)
{
    fichier= ouvrir (nom_physique, 'a');    //Ouverture en mode ancien
    if (fichier != NULL)        //le fichier existe
    {
        printf ("\n\t******************    E N T E T E    ********************\n\n");
        printf("\t> le nombre total de bloc                      >>>    %d\n",entete(fichier,1));
        printf("\t> la position libre dans la dernier bloc       >>>    %d\n",entete(fichier,2));
        printf("\t> le nombre total d'eleves est                 >>>    %d\n",entete(fichier,3));
        printf("\t> la nombre total de caracteres inseres est    >>>    %d\n",entete(fichier,4));
        printf("\t> le nombre total de caracteres supprimes est  >>>    %d\n",entete(fichier,5));
        printf("\t> le seuil de reorganisation de ce fichier     >>>    %d\n\n",entete(fichier,6));


        printf ("\t*********************************************************\n\n");
    }
    else
        printf ("\a Fichier %s n'existe pas !!", nom_physique);
    fermer(fichier);
}


/**************************************************************************************************/

void tableax_matieres(int n)
{
       char ligne[15];
       switch(n)
       {
       case 1:
        {
           // pour les matieres de 2eme année ,3,4,5

        int i=0;
        FILE *fic_MATIERE_2_3_4_5=NULL;
        fic_MATIERE_2_3_4_5 = fopen("M2345.txt", "r");
        if (fic_MATIERE_2_3_4_5==NULL){
            printf("ce fichier n'existe pas");
        }

        while((fgets(ligne,15,fic_MATIERE_2_3_4_5)!= NULL) && (i<8)){
            printf("\nN°:%d --------> %s",i+1,ligne);
            i++;
        }

        fclose(fic_MATIERE_2_3_4_5);

        }break;
        case 2:
        {
           // pour les matieres de 1eme année et prescolaires

        int i=0;
        FILE *fic_MATIERE_p_1=NULL;
        fic_MATIERE_p_1 = fopen("M01.txt", "r");
        if (fic_MATIERE_p_1==NULL){
            printf("ce fichier n'existe pas");
        }

        while((fgets(ligne,15,fic_MATIERE_p_1)!= NULL) && (i<8)){
             printf("\nN:%d --------> %s",i+1,ligne);
            i++;
        }

        fclose(fic_MATIERE_p_1);


        }break;
        default:
            {
                printf("Erreur champs");
            }break;

       }



}
/******************************************************************************************************/
void chargement_initial(TOVC *fi)

{
    Buffer buf;
    const char nomphysique[]= "LINA.bin";
    fi = ouvrir(nomphysique,'n'); // ouverture en mode nouveau d'un fichier
    //if (fichier != NULL)   // Fichier de donnée ouvert avec succès
    char c [15];
    char NOM [50];
    char PRENOM_GENRE[50];
    //char *PRENOM_GENRE = malloc(sizeof(char)*50) ;
    int ID= 504;
    char ident[4];
    int classe;
    int C=1;
    int niv=0;
    int i=1,j=0;
    int eff=0;
    char effacement[2];
    int nbr_char_ins=0;

    FILE *fic_noms=NULL;
    char NOTES[33];
    fic_noms = fopen("Noms.txt", "r");

    if (fic_noms==NULL ){
        printf("ce fichier n'existe pas");
    }
    else{
        //create the table of names
        char ligne[15];
        int i3=0;
        FILE *fic_prenoms_genre=NULL;
        fic_prenoms_genre = fopen("Prenoms_Genre.txt", "r");
        if (fic_prenoms_genre==NULL){
            printf("ce fichier n'existe pas");
        }

        while((fgets(ligne,15,fic_prenoms_genre)!= NULL) && (i3<100)){
            strcpy(tableau_prenoms_genres[i3],ligne);
            i3++;
        }

        fclose(fic_prenoms_genre);

        while (niv<6){
            C=1;
            while(C<4){
                classe=(niv*10)+C;

                while((fgets(c,20,fic_noms)!= NULL) && (c[0]!='#'))/*on lit le fichier tant qu'on ne recoit pas null ou bien #*/{

                    convert_numner_chaine(eff,1,effacement);

                    Ecrire_chaine(fi,effacement,strlen(effacement),&i,&j,&buf);
                    nbr_char_ins=nbr_char_ins+1;
                    char salle[2];
                    convert_numner_chaine(classe,2,salle);
                    if (niv==0)
                    {
                        salle[0]='p';
                    }
                    Ecrire_chaine(fi,salle,strlen(salle),&i,&j,&buf);
                    nbr_char_ins=nbr_char_ins+2;
                    printf("%s  ",salle);

                    convert_numner_chaine(ID,4,ident);
                    printf("%s  ",ident);
                    Ecrire_chaine(fi,ident,strlen(ident),&i,&j,&buf);
                    nbr_char_ins=nbr_char_ins+4;
                    ID--;
                    // fgets(c,15,fic_noms);
                    strcpy(NOM,c);
                    NOM[strlen(NOM)-1]='\0';
                     char TN[20];
                    convert_numner_chaine(strlen(NOM),2,TN);
                    strcat(TN,NOM);
                    strcpy(NOM,TN);

                    printf("%s  ",NOM);
                    Ecrire_chaine(fi,NOM,strlen(NOM),&i,&j,&buf);
                    nbr_char_ins=nbr_char_ins+2+strlen(NOM);
                    printf("%d",(i));
                    printf("   %d",(j));
                    int numligne=(rand()%100);
                    printf("  ");

                    Alea_prenoms_genres(&numligne,PRENOM_GENRE);
                    printf("%s",PRENOM_GENRE);
                    //printf("  ");
                    Ecrire_chaine(fi,PRENOM_GENRE,strlen(PRENOM_GENRE),&i,&j,&buf);
                    nbr_char_ins=nbr_char_ins+strlen(PRENOM_GENRE);
                    char NOTES[33]="";
                    {
                        genere_notes(NOTES);
                        Ecrire_chaine(fi,NOTES,strlen(NOTES),&i,&j,&buf);
                        nbr_char_ins=nbr_char_ins+strlen(NOTES);
                    }
                    printf("\nLe nomre de caracteres inseres a l'instant est : --------------------------->%d\n",nbr_char_ins);

                }

                printf("//////////////////////////////////////////////////////////////////////////\n");
                C++;
                // printf("%d",(i));
            }
            niv++;

        }
        printf("\n--------------------------------------------------------------------------");
        buf.tab[j+1]='\0';
        ecriredir(fi,i,buf);

    }

    printf("\nle nombre de blocs total est de: %d\n",entete(fi,1));
    aff_entete(fi,3,504);//affecter le nombre d'insertions effectués
    aff_entete(fi,4,nbr_char_ins); //le nombre de caracteres inseres
    printf("la position libre dans le dernier bloc est: %d\n",entete(fi,2));
    printf("le nomre d'eleves chargés est de : %d\n",entete(fi,3));
    printf("le nomre de caracteres inseres est de : %d\n",entete(fi,4));
    /*i=1;
    j=0;
    char chaine[15];
    lire_chaine(fi,10,&i,&j,chaine,&buf);*/
    fermer(fi);

}
/****************************************************************************************************/
void reorganisation(TOVC *fichier, TOVC *fichier_o, const char nom_nouveau[])
{
    Buffer buf;
    Buffer bufo;
    char chaine[8];
    char TP[50],TN[3],nom[20],reste[60];//reste represente le prenom+genre+les notes
    int fin=0,cpt=0;

    nom_nouveau="fichier_organise.bin";
    fichier_o = ouvrir(nom_nouveau,'n'); // ouverture du nouveau fichier
    const char nomphysique[]= "LINA.bin";
    fichier = ouvrir(nomphysique,'a');
    int ind_libre = entete(fichier,2);
    int der_bloc = entete(fichier,1);
    printf ("\n ind_libre = %d  der_bloc = %d ",ind_libre,der_bloc);
    if (fichier_o!= NULL)
    {
     printf("\nle fichier nouveau est non NULL");
     if(entete(fichier,1) != 0) // si le fichier initial n'est pas vide
        {
            printf("\nle fichier est non vide ");
                //Parcourir tout le fichier
            int i=1,j=0;//les indices de blocs et de position de fichier initial
            int a=1,b=0;//les indices de blocs et de position de fichier organisé

            while(fin==0)  //tq on est pas arrivée a la fin du fichier
             {
               if (( i == der_bloc) && ( j >= ind_libre))
                     fin = 1 ;
               else
               {
                  //recuperation de la donnée
                  lire_chaine(fichier,7,&i,&j,chaine,&buf) ;
                  chaine[strlen(chaine)]='\0';
                  if(chaine[0]=='0')//la donnée n'est pas effacée logiquement
                  {
                      Ecrire_chaine(fichier_o,chaine,strlen(chaine),&a,&b,&bufo);
                      lire_chaine(fichier,2,&i,&j,TN,&buf);
                      lire_chaine(fichier,atoi(TN),&i,&j,nom,&buf);
                      strcat(TN,nom);
                      strcpy(nom,TN);
                      Ecrire_chaine(fichier_o,nom,strlen(nom),&a,&b,&bufo);
                      lire_chaine(fichier,2,&i,&j,TP,&buf);
                      lire_chaine(fichier,atoi(TP)+1+32,&i,&j,reste,&buf);
                      strcat(TP,reste);
                      strcpy(reste,TP);
                      Ecrire_chaine(fichier_o,reste,strlen(reste),&a,&b,&bufo);

                  }
                  else //depasser l'enregistrement s'il est logiquement effacé
                  {
                     lire_chaine(fichier,2,&i,&j,TN,&buf);
                     j=j+atoi(TN);
                     test_fin_bloc(&j,&i);
                     lire_chaine(fichier,2,&i,&j,TP,&buf);
                     j=j+atoi(TP)+1+32;
                     test_fin_bloc(&j,&i);

                  }
                  cpt++;

               }
             }

             bufo.tab[b+1]='\0';
             ecriredir(fichier_o,i,bufo);

             printf("\n nombre de boucles : %d ",cpt);
           //  Affichage_bloc(fichier_o);

    }
}
    printf("lenombre de blocs total est de: %d\n",entete(fichier_o,1));
   // aff_entete(fi,4,nbr_char_ins); //le nombre de caracteres inseres
    printf("la position libre dans le dernier bloc est: %d\n",entete(fichier_o,2));
    //printf("le nomre d'eleves chargés est de : %d\n",entete(fichier_o,3));
    //printf("le nomre de caracteres inseres est de : %d\n",entete(fichier_o,4));
//    printf("////////%d",i);

    fermer(fichier_o);
}

/****************************************************************************************/
TOVC*fi2;

/*On a choisit le modele TOVC pour l'archive*/
/* Le fichier archive contient les identifiants des eleves, leurs classes de l'année en cours et leurs moyennes des années
précédentes sachant que l'archive est ordonné par moyennes */

/*On construit un tableau d'index qui contient le min et max de chaque classe et leurs positions et numeros du bloc dans le
fichier initial puis le mettre dans un fichier et on charge le fichier dans un tableau pour l'utiliser */


void archivage(TOVC*fi,TOVC*fi2)
//archiver le fichier initial(garder les identifiants,les classes et les moyenne de toutes les années scolaires des étudiants
{
 Buffer buf1;
 Buffer buf2;
 const char nomphysique[]= "LINA.bin";
 fi = ouvrir(nomphysique,'a');
 const char nomphysique1[]= "archive.bin";
 fi2 = ouvrir(nomphysique1,'n');

 int nbr_char_ins=0;
 int i1,j,i2,j2;
 i1=1 ; j=0;
 liredir(fi,i1, &buf1);
 i2=1;j2=0;int t=0;
 char identifiant[5];
 char LN[3];
 char LP[3];
 char classe[3];
 char note1[5],note2[5],note3[5],note4[5],note5[5],note6[5],note7[5],note8[5];
 int moy;
 int x=0;
 j++;   //pour sauter le shamps d'effacement//
 lire_chaine (fi,2, &i1, &j,classe,&buf1);   //on récupère la classe
 lire_chaine (fi,4, &i1, &j,identifiant,&buf1);  //on récupère l'identifiant
 lire_chaine (fi,2, &i1, &j, LN,&buf1);j=j+atoi(LN); //on récupère la taille du nom et avancer
 lire_chaine (fi,2, &i1, &j, LP,&buf1);j=j+atoi(LP)+1;  //on récupère la taille du prenom et avancer

 lire_chaine(fi,4,&i1,&j,note1,&buf1);  //on récupère les notes de toutes les matières
 lire_chaine(fi,4,&i1,&j,note2,&buf1);
 lire_chaine(fi,4,&i1,&j,note3,&buf1);
 lire_chaine(fi,4,&i1,&j,note4,&buf1);
 lire_chaine(fi,4,&i1,&j,note5,&buf1);
 lire_chaine(fi,4,&i1,&j,note6,&buf1);
 lire_chaine(fi,4,&i1,&j,note7,&buf1);
 lire_chaine(fi,4,&i1,&j,note8,&buf1);
 moy=(atoi(note1)+atoi(note2)+atoi(note3)+atoi(note4)+atoi(note5)+atoi(note6)+atoi(note7)+atoi(note8))/8;

//pour l'affichage
 printf("\n Le contenu du tableau index cree: la moyenne min et max de chaque classe avec ses positions dans le fichier initial et le num du bloc\n");

 char classe2[3] ="p1";
 int k;
 int o;
 while ((i1 < entete(fi, 1)) || ((i1 == entete(fi, 1)) && (j< entete(fi, 2))))

  {
      t=0;
      while (strcmp(classe,classe2)==0)
      {
        tri_tab[t].moy_courant= moy;   //remplir le tableau tri_tab par les informations des eleves d'une meme classe
        strcpy(tri_tab[t].ident,identifiant);
        tri_tab[t].ident[4]='\0';
        strcpy(tri_tab[t].cl,classe);
        tri_tab[t].cl[2]='\0';
         t++;
        j=j+1;   //pour sauter le champ d'effacement
        if (j>=TAILLE_BLOC)
        {
            i1++;
            j=j%TAILLE_BLOC;
        }
        strcpy(classe2,classe);
        lire_chaine (fi,2, &i1, &j,classe,&buf1);
        lire_chaine (fi,4, &i1, &j,identifiant,&buf1);
        lire_chaine (fi,2, &i1, &j, LN,&buf1);j=j+atoi(LN);
        if (j>=TAILLE_BLOC)
        {
            i1++;
            j=j%TAILLE_BLOC;
        }
        lire_chaine (fi,2, &i1, &j, LP,&buf1);j=j+atoi(LP)+1;
        if (j>=TAILLE_BLOC)
        {
            i1++;
            j=j%TAILLE_BLOC;
        }
        lire_chaine(fi,4,&i1,&j,note1,&buf1);
        lire_chaine(fi,4,&i1,&j,note2,&buf1);
        lire_chaine(fi,4,&i1,&j,note3,&buf1);
        lire_chaine(fi,4,&i1,&j,note4,&buf1);
        lire_chaine(fi,4,&i1,&j,note5,&buf1);
        lire_chaine(fi,4,&i1,&j,note6,&buf1);
        lire_chaine(fi,4,&i1,&j,note7,&buf1);
        lire_chaine(fi,4,&i1,&j,note8,&buf1);
        moy=(atoi(note1)+atoi(note2)+atoi(note3)+atoi(note4)+atoi(note5)+atoi(note6)+atoi(note7)+atoi(note8))/8;

      }
 tri_selec_perm(tri_tab,t-1); //trier tri_tab selon les moyennes

 for(o=0;o<t;o++)  //boucle pour remplir le tableau d'index
 {
   if (o==0)
   {
     index[x].moy=tri_tab[0].moy_courant;  //mettre le num du bloc et la position de la moyenne min
     printf("\n Min: %d",index[x].moy);
     index[x].pos=j2;
     printf("  |%d",index[x].pos);
     index[x].nbbloc=i2;
     printf("  |%d",index[x].nbbloc);
     x++;
   }
   else
   {
       if (o==(t-1))
       {
         index[x].moy=tri_tab[o].moy_courant;  //mettre le num du bloc et la position de la moyenne max
         printf("\n Max: %d",index[x].moy);
         index[x].pos=j2;
         printf(" |%d",index[x].pos);
         index[x].nbbloc=i2;
         printf("  |%d",index[x].nbbloc);
         printf("\n");
         x++;
       }
   }
   //remplir le fichier d'archive a partir du tableau tri_tab
   Ecrire_chaine(fi2,tri_tab[o].ident,4,&i2,&j2,&buf2 );
   nbr_char_ins=nbr_char_ins+4;

   Ecrire_chaine(fi2,tri_tab[o].cl,2,&i2,&j2,&buf2 );
   nbr_char_ins=nbr_char_ins+2;

   char chmoy[5];
   convert_numner_chaine(tri_tab[o].moy_courant,4,chmoy);
   Ecrire_chaine(fi2,chmoy,4,&i2,&j2,&buf2 );
   nbr_char_ins=nbr_char_ins+4;
  char cl;

   if (classe2[0]!='p')
   {
     int d;
       cl=classe2[0];

       d=(int)cl;
       d=d-48;

      int p;

      for (p=0;p<d;p++)  //boucle pour generer les moyennes des années précédentes
       {

          int cmp=(rand()%1000+1000);
          convert_numner_chaine(cmp,4,chmoy);
          Ecrire_chaine(fi2,chmoy,4,&i2,&j2,&buf2 );
          nbr_char_ins=nbr_char_ins+4;
       }
   }
 }
  strcpy(classe2,classe);
}

aff_entete(fi2,4,nbr_char_ins);
fermer(fi2);

}

/********************************************************************************************/
void fichier_index(TOVC* fi)
//garder le tableau d'index dans un fichier
{
    const char nomphysique2[]= "index.bin" ;
    fi= ouvrir(nomphysique2,'n');

    Buffer buf;
    int i=1;
    int j=0;
    int k;
    int nb_char=0;
    char moy[5];
    char pos[4];
    char numbloc[3];
    for (k=0;k<=35;k++)
    {
        //récupérer les informations a partir du tableau index et remplir le fichier d'index
      convert_numner_chaine(index[k].moy,4,moy);
      Ecrire_chaine(fi,moy,4,&i,&j,&buf);
      nb_char=nb_char+4;
      convert_numner_chaine(index[k].nbbloc,2,numbloc) ;
      Ecrire_chaine(fi,numbloc,2,&i,&j,&buf);
      nb_char=nb_char+2;
      convert_numner_chaine(index[k].pos,3,pos) ;
      Ecrire_chaine(fi,pos,3,&i,&j,&buf);
      nb_char=nb_char+3;

    }
    aff_entete(fi,1,i);
    aff_entete(fi,2,j);
    aff_entete(fi,4,nb_char);
    fermer(fi);
}
/**************************************************************************************************************/
void chargement_index(TOVC* fi)
//mettre le fichier d'index dans un tableau
{
    const char nomphysique2[]= "index.bin";
    fi =ouvrir(nomphysique2,'a');

    Buffer buf;
    int i=1;
    int j=0;
    int k=0;
    char moy[5];
    char pos[4];
    char numbloc[3];
    while ((i<entete(fi,1))|| ((i=entete(fi,1))&& (j<entete(fi,2))))
    {
      lire_chaine(fi,4,&i,&j,moy,&buf) ;  //on récupère la moyenne
      lire_chaine(fi,2,&i,&j,numbloc,&buf) ;  //on récupère le numéro du bloc
      lire_chaine(fi,3,&i,&j,pos,&buf) ;  //on récupère la position
    //remplir le tableau d'index     enlever les commentaires si tu veux voir le contenu du tableau index apres chargement
      index[k].moy=atoi(moy);
      //printf("\n%d",index[k].moy);
      index[k].nbbloc=atoi(numbloc);
      //printf("\n%d",index[k].nbbloc);
      index[k].pos=atoi(pos);
      //printf("\n%d",index[k].pos);
      k++;
    }

}

/**************************************************************************************************************/
void tri_selec_perm(Eleve tri_tab[],int tai)
//trier le tableau tri_tab par moyenne
{
  int n=0,m=0;
  for (n=0;n<=tai-1;n++)
  {
      for (m=n+1;m<=tai;m++)
      {
        if (tri_tab[n].moy_courant > tri_tab[m].moy_courant)  // Permuter les deux éléments
         {
            Eleve sauv;

            strcpy(sauv.ident,tri_tab[n].ident);
            strcpy(sauv.cl,tri_tab[n].cl);
            sauv.moy_courant=tri_tab[n].moy_courant;

            strcpy(tri_tab[n].ident,tri_tab[m].ident);
            strcpy(tri_tab[n].cl,tri_tab[m].cl);
            tri_tab[n].moy_courant=tri_tab[m].moy_courant;

            strcpy(tri_tab[m].ident,sauv.ident);
            strcpy(tri_tab[m].cl,sauv.cl);
            tri_tab[m].moy_courant=sauv.moy_courant;
         }
      }
  }
}
/**********************************************************************************************************/
void lister(TOVC* fi)
//afficher les eleves qui ont eu des moyennes>12
{

    const char nomphysique1[]= "archive.bin";
    fi =ouvrir(nomphysique1,'a');

    Buffer buf;
    char cl[3];
    char moy[5];
    char ident[5];
    int s=0;
    int a;
    int k,i,j;
    for (k=0;k<36;k=k+2)
    {
 printf("\n=============================================================");
       if ((index[k].moy<=1200) && (index[k+1].moy>=1200)) //tester esq 12 est compris entre le min et max de la classe k
       {
          if (k!=0)
          {
           if (k%3==0)
           {
               s=s+4;  // pour chaque niveau ya une moyenne en plus dans l'archive par rapport a l'année précédente donc a chaque fois
                                       // on ajoute 4 au pas de saut
           }
          }
          i=index[k].nbbloc; //récupérer le num du bloc ou il existe le min
          j=index[k].pos;    //récuperer la position du min
          while ((i<index[k+1].nbbloc) || ((i==index[k+1].nbbloc)&&(j<=index[k+1].pos))) //tant que on a pas arriver a la fin de la classe
          {
              j=j+6; //pour sauter l'identifiant et la classe
              test_fin_bloc(&j,&i);
              lire_chaine(fi,4,&i,&j,moy,&buf);
              a=j;

              while (atoi(moy)<1200)  //tant que on a pas trouvé une moyenne>=12
              {
                 j=j+6+s;
                 test_fin_bloc(&j,&i);
                 lire_chaine(fi,4,&i,&j,moy,&buf);
                 a=j;
              }
              //j=j-10;
              int w;
              for (w=1;w<=10;w++)
              {
                   j=j-1;
                   if (j==0)
                   {
                     i=i-1;          //on revient au bloc précédant
                     j=TAILLE_BLOC-(10-w)  ;
                   }
              }

              lire_chaine(fi,4,&i,&j,ident,&buf);        // on récupere les informations des elèves pour les afficher
              printf("\n Le matricule ->%s",ident);
              lire_chaine(fi,2,&i,&j,cl,&buf);
              printf("\n La classe ->%s",cl);
              printf("\n La moyenne ->%s",moy);
              printf("\n");
              j=a+s; // pour sauter les moyennes des années précédentes
              test_fin_bloc(&j,&i);

          }
       }
    }
}
/***************************************************************************************/
void insertion_archiv(TOVC*fichier,char identifiant[5],char moyenne[5],char classe[3],int c,char moys[30])
//insertion d'un eleve dans l'archive a cause du transfert ou des mises a jours
{
    const char nomphysique1[]= "archive.bin";
    fichier =ouvrir(nomphysique1,'a');

  Buffer buf;
  int cl;
  int m;
  int niv;
  int v;
  int a,s=0;
  char moy[5];
  int taille_enreg;
  int stop=1;

  int nouv=0;
  int nbr_char_pas_aff=0,nbr_char_aff=0,nbr=0;
  char eff[50]="";
  char sauv[50];

  //calculer le niveau p,1,2,3,4,5
  if (classe[0]=='p')
    {niv=0;}
  else
  {
      niv=(int)classe[0]-48;
  }
  s=niv*4;

  //calculer la classe 1,2,3
  cl=(int)classe[1]-48;
  if (cl==1){v=0+6*niv;}
  else
  {
      if (cl==2){v=2+6*niv;}
      else{v=4+6*niv;}
  }

  int i=1,j=0;
  i=index[v].nbbloc;
  j=index[v].pos;


              j=j+6; //pour sauter l'identifiant et la classe
              test_fin_bloc(&j,&i);
              lire_chaine(fichier,4,&i,&j,moy,&buf);
              a=j;

              while (((atoi(moy)<atoi(moyenne))&& (j<=index[v+1].pos+10 )) ||  ((atoi(moy)<atoi(moyenne))&&(i<index[v+1].nbbloc)))
                                                                                //tant que on a pas trouvé la bonne position

              {
                 j=j+6+s;
                 test_fin_bloc(&j,&i);
                 lire_chaine(fichier,4,&i,&j,moy,&buf);
                 a=j;
              }
              j=j-10;
              test_fin_bloc(&j,&i);


  //liredir(fichier,i,&buf);

  strcat(eff,identifiant);
  strcat(eff,classe);
  strcat(eff,moyenne);
          int w;
         if (c==1) //insertion d'un nouvel eleve

          { for (w=1;w<=niv;w++)
             {
                strcat(eff,"0000");  //garder les moyennes des années précédentes vides
             }
          }

         else   //mise a jour d'une note ou de la classe
        {
           strcat(eff,moys);
        }

  eff[strlen(eff)]='\0';


    taille_enreg=strlen(eff);
  aff_entete(fichier,4,entete(fichier,4)+taille_enreg);
  if((entete(fichier,2)+taille_enreg)>TAILLE_BLOC-1)
    {
        aff_entete(fichier,2,(entete(fichier,2)+taille_enreg)%TAILLE_BLOC);
    }
    else//la position libre ne depasse pas la taille du bloc
    {
       aff_entete(fichier,2,entete(fichier,2)+taille_enreg);
    }

  nbr_char_pas_aff=TAILLE_BLOC*(i-1)+j+1;
  nbr_char_aff=entete(fichier,4)-nbr_char_pas_aff;


  if (atoi(moyenne)>index[v+1].moy)  //mise a jour du max de l'index si moyenne>max
  {
    index[v+1].moy= atoi(moyenne);
    index[v+1].pos=j;
    index[v+1].nbbloc=i;
  }
  else
   if (atoi(moyenne)<index[v].moy)  //mise a jour du min de l'index si moyenne<min
    {
      index[v].moy= atoi(moyenne);
      index[v].pos=j;
      index[v].nbbloc=i;
    }


  while((nbr<nbr_char_aff)&&(stop==1))
           {


              int k;
              for (k=0;k<taille_enreg;k++)
                {
                   sauv[k]=buf.tab[j];
                   buf.tab[j]=eff[k];
                   nbr++;
                   if((j)+1==TAILLE_BLOC)
                         {
                            buf.tab[j+1]='\0';
                            j=0;
                            ecriredir(fichier,i,buf);
                             i++;
                             if(i>entete(fichier,1))
                                   {
                                       nouv=1;
                                   }
                            if(nouv==0)
                                  {
                                          liredir(fichier,i,&buf);
                                  }
                        }
                     else
                       {
                           j++;
                       }
                    if((i>=entete(fichier,1))&&(j==entete(fichier,2))) // on arrive a la fin du fichier
                      {
                         stop=0;
                          sauv[k+1] = '\0';
                      }
                 }
               for (m=0;m<strlen(eff);m++)
               {
                 eff[m]=sauv[m];
               }

            }  // end of while

            if(nouv==1)
             aff_entete(fichier,1,i);

           ecriredir(fichier,i,buf);

           for (w=v+2;w<=35;w++) //modifier les postions des min et max de l'index a cause des décalages
           {
               j=index[w].pos+strlen(eff);
               i=index[w].nbbloc;
               test_fin_bloc(&j,&i);
               index[w].pos=j;
               //printf("\n%d",index[w].pos); enlever le commentaire si tu veux voir le contenu du tableau index
               index[w].nbbloc=i;
               //printf("\n%d",index[w].nbbloc);
           }
          // printf("\n%d",index[1].pos);
          // printf("\n%d",index[1].nbbloc);


fermer(fichier);
}
/***********************************************************************************************************************/
void Mise_a_jour_archiv(TOVC *fichier,char identifiant[5],char classe[3],char moyenne[5],int c,char n_cl[3])
//faire le mise a jour d'une moyenne d'un eleve
{

    const char nomphysique1[]= "archive.bin";
    fichier =ouvrir(nomphysique1,'a');


    Buffer buf;
  int cl;
  int niv;
  int v;
  if (classe[0]=='p')        //calculer l'indice correspondant pour accéder à l'index
    {niv=0;}
  else
  {
      niv=(int)classe[0]-48;
  }
  cl=(int)classe[1]-48;
  if (cl==1){v=0+6*niv;}
  else
  {
      if (cl==2){v=2+6*niv;}
      else{v=4+6*niv;}
  }
  int i=1,j=0;
  i=index[v].nbbloc;
  j=index[v].pos;

  char id[5];
  char moys[20];
  int d=0;
  if (classe[0]!='p'){d=(int)classe[0]-48;}

  lire_chaine(fichier,4,&i,&j,id,&buf);
  id[4]='\0';
  identifiant[4]='\0';
  while (strcmp(identifiant,id)!=0)      //tant que on a pas trouvé l'identifiant
  {
      j=j+6+d*4; //6:pour sauter la classe et la 1er moyenne  d*4:pour sauter les moyennes des années précédentes
      test_fin_bloc(&j,&i);
      lire_chaine(fichier,4,&i,&j,id,&buf);
  }
  j=j-4;
  Ecrire_chaine(fichier,"0000",4,&i,&j,&buf);
  if (c==1) //mise a jour d'une note
  {
    j=j+6;
    lire_chaine(fichier,4*niv,&i,&j,moys,&buf);
    insertion_archiv(fichier,id,moyenne,classe,2,moys);
  }
  else  //mise a jour de la classe
  {
    j=j+2;
    lire_chaine(fichier,4,&i,&j,moyenne,&buf);
    lire_chaine(fichier,4*niv,&i,&j,moys,&buf);
    insertion_archiv(fichier,id,moyenne,n_cl,2,moys);

  }

fermer(fichier);
}
/***************************************************************************************************************************/
void Affichage_bloc2(TOVC * fichier)
{
    const char nomphysique1[]= "archive.bin";
    fichier =ouvrir(nomphysique1,'a');


    Buffer buf ; int i;
    printf("\n\n Le contenu des blocs ------------------------------------------------------\n");
    if (entete(fichier,1)>0){

        for (i=1; i<=entete(fichier,1);i++){
            liredir(fichier,i,&buf);
            printf(" Le contenu du bloc %d : %s\n",i,buf.tab);
        }
    }
    //printf("\n%d",entete(fichier,4));
    fermer(fichier);
}
/********************************************************************************************************************************/
void Affichage_bloc3(TOVC * fichier)
{
    const char nomphysique2[]= "index.bin";
    fichier =ouvrir(nomphysique2,'a');


    Buffer buf ; int i;
    printf("\n\n Le contenu des blocs ------------------------------------------------------\n");
    if (entete(fichier,1)>0){

        for (i=1; i<=entete(fichier,1);i++){
            liredir(fichier,i,&buf);
            printf(" Le contenu du bloc %d : %s\n",i,buf.tab);
        }
    }
    //printf("\n%d",entete(fichier,4));
}

/**************************************************************************************************************************************/






