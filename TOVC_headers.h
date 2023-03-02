#ifndef TOVC_HEADERS_H_INCLUDED
#define TOVC_HEADERS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NOM 20                // Taille maximale du nom variable (observation)
#define MAX_PRENEM 10            // Taille maximale du prénom variable (observation)
#define MAX_ENR 76              // Somme des champ (au maximum)
#define TAILLE_BLOC 1000       // Combien de caract�re que peut contenir un bloc




 /********               D E C L A R A T I O N   D E S   T Y P E S           *******/

        /* Structure du Bloc */
typedef struct Tbloc
{
    char tab[TAILLE_BLOC];  // le tableau de caract�re pour les enregistrement

}Tbloc;

        /* Declaration du tampon */
typedef struct Tbloc Buffer;

        /* Structure de l'entete du fichier en memoire centrale */
typedef struct T_Entete
{
    int nbloc;            // nombre total de bloc allou�s dans le fichier
    int poslibre;        // nombre de caract�res dans le dernier bloc
    int nb_elv_ins;     // nombre d'eleves ins�r�s
    int nb_car_ins;     // nombre de caract�res ins�r�s
    int nb_car_sup;    //nombre de caract�res supprim�s
    int seuil;        // Seuil de r�organisation

}T_Entete;

        /* Structure du fichier TOVC */
typedef struct TOVC
{                                                             //HNA KAN KAYN RA9M NRMLM ????
    T_Entete entete;
    FILE *f;

}TOVC;


typedef struct Eleve
{
    char ident[5];
    char cl[3];
    int moy_courant;
}Eleve;

Eleve tri_tab[30];

typedef struct enreg
{
    int moy;
    int nbbloc;
    int pos;
}enreg;

enreg index[36];





char tableau_prenoms_genres[100][15]; //declaration du tableau des prenoms comme une variable globale



/****************************************************************************************************/
/************* E N T E T E S    D E S    F O N C T I O N S    E T  P R O C E D U R E S *************/
/**************************************************************************************************/


/*_____________________________________________________________M A C H I N E   A B S T R A I T E */

void aff_entete(TOVC *fichier,int m , int valeur);                   // Mettre a jour les elements de l'entete du fichier
int entete(TOVC *fichier, int p);                                   // Obtenir les elements de l'entete du fichier
void liredir(TOVC *fichier, int hh , Buffer *buf);                 // Lire le ieme bloc du fichier et le mettre dans buf
void ecriredir(TOVC *fichier, int hh, Buffer buf);                // Ecrire le buffer dans le ieme bloc du fichier
TOVC *ouvrir(const char nom_physique[], char mode);              // procédure d'ouverture du fichier
void fermer(TOVC *fichier);                                     // procédure de fermeture du fichier
void alloc_bloc (TOVC *fichier,  Buffer *buf, int *num);       // Allouer un nouveau bloc pointé par buf dans le fichier et retourner son numero

/*________________________________________________________________________________M O D U L E S */


//*****   Manipulation des chaines  *************************************************************//
void convert_numner_chaine(int numro ,int longeur,char * chaine[longeur]);                       //convertir un entier en une chaine de caract�res (en corrigeons la taille avec des 0 si n�c�ssaire)
void genere_notes(char*marks);                                                                  //genere des notes al�atoirement et les concat�nent apr�s les avoir corriger de taille
void Ecrire_chaine(TOVC *fichier,char chaine[],int longueur,int *i,int *j,Buffer *buf );       // ecrire une chaine de char de un buffer et l'ecrire dans le fichier si le buf est plein
void Ecrire_chaine_wast(TOVC *fichier,char chaine[],int longueur,int *i,int *j,Buffer *buf );
                                                     //afficher le contenu des blocs du fichier
void Alea_prenoms_genres(int*num_ligne,char pre_gen[]);                                      // recuperer une chaine de char de taiile n a partir du buffar
void lire_chaine(TOVC *fichier,int n,int*i,int*j,char chaine[],Buffer *buf) ;                // selectionner un prenom aleatoire du fichier des pr�noms et genre
void chargement_initial(TOVC *fi);
void ajbadd(TOVC *fichier,int n);
void rechaerche(  TOVC *fichier, int *trouv,int *i, int *j,int choice);
int comparaison(const char chaine1[],const char chaine2[]);
void test_fin_bloc(int *position,int*bloc);
void suppression(TOVC *fichier);
void rechaerche_classe(TOVC *fichier,int *trouv,int *i, int *j,char classe[],char nom[],char prenom[]);
void rechaerche_par_ID(TOVC *fichier,int *trouv,int *i, int *j,int identifiant);
void insertion(TOVC *fichier,char classe[],char nom[],char prenom[],char genre[2],char ident[4],char notes[32]);
void tableax_matieres(int n);
void creer_notes_moyenne(char*marks,int*moyenne);
void reorganisation(TOVC *fichier, TOVC *fichier_o, const char nom_nouveau[]);

void tri_selec_perm(Eleve tri_tab[],int tai);
void archivage(TOVC*fi,TOVC*fi2);
void Affichage_bloc2(TOVC * fichier);
void lister(TOVC* fi);
void insertion_archiv(TOVC*fichier,char identifiant[5],char moyenne[5],char classe[3],int c,char moys[30]);
void Mise_a_jour_archiv(TOVC *fichier,char identifiant[5],char classe[3],char moyenne[5],int c,char n_cl[3]);
void chargement_index(TOVC* fi);

//affichage
void afficher_entete(TOVC *fichier, const char *nom_physique);
void Affichage_bloc(TOVC * fichier);
void Affichage_bloco(TOVC * fichier);
void Affichage_bloc2(TOVC * fichier);






#endif // TOVC_HEADERS_H_INCLUDED
