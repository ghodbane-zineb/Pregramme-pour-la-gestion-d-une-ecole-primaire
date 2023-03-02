#include "TOVC_headers.h"

 /*************MENU***********/
void menu ()

		{
		printf ("\n\t\t _________________________________________________________________________\n");
		printf ("\t\t|\t                                                                  |\n");
	    printf ("\t\t|                     <<<< MENU PRINCIPAL >>>>                            |\n");
	    printf ("\t\t|\t                                                                  |\n");
	    printf ("\t\t|_________________________________________________________________________|\n");
	    printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -1. Creer un nouveau fichier TOVC des eleves avec un chargement  |\n");
		printf ("\t\t|\t     initial et l'archiver                                        |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -2. Recherche d'un eleve selon son identifiant.                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -3. Recherche d'un eleve selon sa classe,nom et prenom.          |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -4. Supprimer logiquement un eleve.                              |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -5. inserer un nouvel eleve dans notre ecole.                    |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -6. Mise a jour d'un eleve .                                     |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -7. Reorganisation.                                              |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -8. Affichage de l'entete.                                       |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t -9. Quitter                                                      |\n");
		printf ("\t\t|\t                                                                  |\n");
		printf ("\t\t|\t                                                                  |\n");
        printf ("\t\t|_________________________________________________________________________|\n\n");
        }







//srand(time(NULL));
int main()
{
        TOVC*fich;
        const char nomphysique[]= "LINA.bin";

       TOVC*fich_o;
       const char nom_nouveau[20];

       TOVC* indexe;
       TOVC* archiv;

       int trouv;
       char *chaine;

       int i,j,k;
       int choice,ch;


       int stop=0;
       char prenom[15];
       char nom[20];
       char classe[3];
       char genre[2];
       int identifiant;


    srand(time(NULL));
    int choix, stopp=0;
    char notes[32];
    int moyenne;

    //creer_notes_moyenne(&notes,&moyenne);
    //moyenne=moyenne+1;

    do
    {
        do
        {
            system("cls");
            menu();
            printf("*** Veuillez saisir une choix: ");
            scanf("%d",&choix);
        } while ((choix<1)||(choix>10));



        switch (choix)
        {
            case 1:
            {

                system("cls");
                printf ("\n\t_____________________      C R E A T I O N     _____________________\n\n") ;
                printf("R E M A R Q U E :\n");
                printf("\n On a cree le fichier initial et son fichier archive donc toute modification dans le fichier\n initial implique un changement dans l'archive");
                printf("\n");
                chargement_initial(fich);//du fichier initial
                Affichage_bloc(fich);//affichage du contenu du fichier initial
                afficher_entete(fich,nomphysique);
                archivage(fich,archiv);//du fichier archive
                Affichage_bloc2(archiv);//affichage du contenu du fichier archive
                fichier_index(indexe);//mettre le tableau index dans un fichier
                printf("\n   ->Le contenu de l'index");
                Affichage_bloc3(indexe);//affichage du contenu du fichier indexe
                lister(indexe);
                system("pause");
                break;
            }
            case 2:
            {
                system("cls");
                printf ("\n\t_____________________      R E C H E R C H E      _____________________\n\n") ;
                printf ("\n\t_____________________         P A R   ID          _____________________\n\n") ;

                 printf("R E M A R Q U E :\n");
                printf("Les indentifiants des eleves existants sont compris initialement entre 1 et 504 et a chaque fois qu'on insere\n un nouvel eleve on incremente le plus grand identifiant deja existe dans l'entete\n ");
                printf("  >  Entrez l'identifiant que vous voulez chercher ! : ");
                scanf("%d",&identifiant);//recherche avec identifiant
                printf("\nIdentifiant entree: %d\n", identifiant);
                rechaerche_par_ID(fich,&trouv,&i,&j, identifiant);
                system("pause");
                break;

            }
            case 3:
            {
                system("cls");
                printf ("\n\t_____________________      R E C H E R C H E               _____________________\n\n") ;
                printf ("\n\t_____________________    P A R  classe+nom+prenom          _____________________\n\n") ;

                 Affichage_bloc(fich);//afficher le contenu du fichier initial pour faciliter la recherche
                 printf("R E M A R Q U E :\n");
                  printf("Les classes sont: p1 p2 p3 | 11 12 13 | 21 22 23 | 31 32 33 | 41 42 43 | 51 52 53 |\n");
                printf("\n\n   > Entrez la classe de l'etudiant  !");
                scanf("%s",classe);
                classe[strlen(classe)]='\0';

                printf("   > Entrer le nom de l'etudiant  (en majuscule):  ");
                scanf("%s",nom);
                nom[strlen(nom)]='\0';

                printf("   > Entrer le prenom de l'etudiant (juste le premier caractere en majuscule) :  ");
                scanf("%s",prenom);
                prenom[strlen(prenom)]='\0';

                rechaerche_classe(fich,&trouv,&i,&j,classe,nom,prenom);
                system("pause");
                break;

            }
            case 4:
            {
                system("cls");
                printf ("\n\t_____________________      S U P P R E S S I O N     _____________________\n\n") ;
                Affichage_bloc(fich);//afficher le contenu du fichier initial pour faciliter la suppressin
                suppression(fich);
                system("pause");
                break;

            }
            case 5:
            {
                system("cls");
                printf ("\n\t_____________________      I N S E R T I O N      _____________________\n\n") ;

                char nomm[20];
                char prenomm[15];
                fich = ouvrir(nomphysique,'a');
                printf("\n\nveuillez donnez les informations de l'eleves que vous voulez inserer comme nouvel éleve !\n\n");
                printf("\n\n Entrez la classe de l'étudiant  !\n");
                scanf("%s",classe);

                printf("entrer le nom de l'etudiant  :  ");
                scanf("%s",nom);

                strcpy(nomm,nom);


                printf("entrer le prenom de l'etudiant  :  ");
                scanf("%s",prenom);

                strcpy(prenomm,prenom);

                printf("\n\n--->entrez le genre de l'eleve par un 'F' pour le Feminin et un 'M' pour Masculin  : ");
                scanf("%s",genre);


                char notes[33]="";
                int moyenne;
               creer_notes_moyenne(notes,&moyenne);
               char notess[33];
               strcpy(notess,notes);

                char ident[4];
                int ID;
                fich = ouvrir(nomphysique,'a');
                ID=entete(fich,3)+1;
                fich = ouvrir(nomphysique,'a');
                aff_entete(fich,3,entete(fich,3)+1);
                fermer(fich);
                printf("___________________%d",entete(fich,3));
                convert_numner_chaine(ID,4,ident);
                  //dans le fichier initial
                insertion(fich,classe,nom,prenom,genre,ident,notes);
                Affichage_bloc(fich);
                afficher_entete(fich,nomphysique);
                   //dans le fichier archive
                char chmoy[5];
                convert_numner_chaine(moyenne,4,chmoy);
               chargement_index(indexe);
                insertion_archiv(archiv,ident,chmoy,classe,1,"");
                fichier_index(indexe);
                Affichage_bloc3(indexe);
                Affichage_bloc2(archiv);
                system("pause");
                break;

            }
            case 6:
            {
                   system("cls");
                   printf ("\n\t_____________________     M I S E    A   J O U R     _____________________\n\n") ;
                   int m;
                   printf("\nvouler vous faire une mise a jour au niveau de :\n");
                   printf("                    1---> une note\n");
                   printf("                    2---> nom et/ou prenom\n");
                   printf("                    3---> la classe\n");

                   printf("votre choix : \n");
                   scanf("%d",&m);
                      //dans le fichier initial
                   mise_a_jour(fich,m);
                   Affichage_bloc(fich);
                   afficher_entete(fich,nomphysique);
                      //dans le fichier archive
                    //pour chaque eleve qu'on fait la mise a jour =>sa position dans l'archive change donc on met '0000' comme identifiant dans l'ancienne position
                        //pour garder la trace de la mise a jour dans l'archive
                        if (m!=2)
                        {
                            Affichage_bloc3(indexe);
                            Affichage_bloc2(archiv);
                        }

                  system("pause");
                  break;
            }
            case 7:
            {
                   system("cls");
                   printf ("\n\t____________________________    R E O R G A N I S A T I O N    ______________________________\n\n") ;
                   printf("R E M A R Q U E :\n");
                   printf(" Puisque le seuil de reorganisation est assez eleve , on prefere mettre la reorganisation comme option dans le menu affin \n de voir sonn effet !\n\n");
                   printf("et pour bien voir son effet on prefere que vous faisaient plusiers suppression dans le mm bloc pour vraiment voir la difference ! ");
                   printf("\nVoici le contenu du fichier avant :\n");
                   scanf("");
                   Affichage_bloc(fich);
                   printf("appuyez sur ENTRER pour le reorganiser .");
                   scanf("");
                   reorganisation(fich,fich_o,nom_nouveau);
                   Affichage_bloco(fich_o);
                   //afficher_entete(fich_o,nom_nouveau);
                  system("pause");
                  break;
            }

            case 8:
            {

                system("cls");
                printf ("\n\t________________________________      A F F I C H A G E    E N T E T E     ________________________________\n\n") ;
                afficher_entete(fich,nomphysique);
                system("pause");
                break;
                  system("pause");
                  break;
            }
            case 9:
            {
                system("cls");
                printf ("\n\n\n\t_____________________       F I N     _____________________\n\n") ;
                stop = 1;
                break;
            }


        }
  }while(stopp==0);

   //  tableax_matieres();
   //  affiche(tableau_matieres_01, 8);
   //  affiche(tableau_matieres_2345, 8);


     chargement_initial(fich);
     Affichage_bloc(fich);

  //   reorganisation(fich,fich_o,nom_nouveau);
//     const char nomphysique[]= "LINA.bin";
     fich = ouvrir(nomphysique,'a');
     printf("                            %d",entete(fich,2));
     printf("                            %d",entete(fich,1));

     /* mise_a_jour(fich,2);
      Affichage_bloc(fich);*/
     suppression(fich);
     Affichage_bloc(fich);
     mise_a_jour(fich,2);
      Affichage_bloc(fich);

     suppression(fich);
     Affichage_bloc(fich);
     suppression(fich);
     Affichage_bloc(fich);
     suppression(fich);
     Affichage_bloc(fich);
     reorganisation(fich,fich_o,nom_nouveau);
   //  mise_a_jour(fich,3);
      Affichage_bloco(fich_o);


     do{
       char nomm[20];
       char prenomm[15];

       printf("\n\nveuillez donnez les informations de l'eleves que vous voulez inserer comme nouvel éleve !\n\n");
       printf("\n\n Entrez la classe de l'étudiant  !\n");
       scanf("%s",classe);

       printf("entrer le nom de l'etudiant  :  ");
       scanf("%s",nom);

       strcpy(nomm,nom);


       printf("entrer le prenom de l'etudiant  :  ");
       scanf("%s",prenom);

       strcpy(prenomm,prenom);

       printf("\n\n--->entrez le genre de l'eleve par un 'F' pour le Feminin et un 'M' pour Masculin  : ");
       scanf("%s",genre);


       char notes[33]="";
       genere_notes(notes);
       char notess[33];
       strcpy(notess,notes);

       char ident[4];
       int ID;

      ID=entete(fich,3)+1;
      aff_entete(fich,3,ID);

      convert_numner_chaine(ID,4,ident);

       insertion(fich,classe,nom,prenom,genre,ident,notes);
       Affichage_bloc(fich);

      }while (stop==0);










    do{

            printf("\n\n\nheyy veuiller choisir entre :\n\n    1-recherche avec identifiant\n    2-recherche avec nom prenom et classe");
            scanf("%d",&choice);
            switch (choice)
            {
          case 1:
            {

            printf("entrez l'identifiant que vous voulez chercher !\n");
            scanf("%d",&identifiant);//recherche avec identifiant
            printf("Identifiant entree: %d\n\n", identifiant);
            rechaerche_par_ID(fich,&trouv,&i,&j, identifiant);

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

            rechaerche_classe(fich,&trouv,&i,&j,classe,nom,prenom);

            }break;
            }
      //suppression(fich,(&i),(&j));
     //Affichage_bloc(fich);

     }while (stop==0);



   // }while(stopp==0);
return 0;
}
