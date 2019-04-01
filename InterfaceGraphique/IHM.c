#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>
#include <complex.h>
#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
#include "ESLib.h"	
#include "OutilsLib.h"	
#include "BmpLib.h"

#include "definitions.h"
#include "IHM.h"
#include "fonctionsTraitementImage.h"


// retourne la longueur en pixels de la chaine à partir de sa hauteur ( et de son nombre de caractères )
int longueurChaineEnPixels(char *chaine, int hauteur_chaine)
{
	int nb_caractere = strlen(chaine); 
	int longueur_chaine = 1320./274. * nb_caractere + ((255-41.*(1320./274.))/10.) * hauteur_chaine ; // formule pour calculer la longueur en pixels de la chaine
	return longueur_chaine;
}

// retourne le xmin de la chaine à centrer dans la fenêtre à partir de sa hauteur
int xminChaineCentree(char *chaine, int hauteur_chaine)
{
	return (largeurFenetre() - tailleChaine(chaine,hauteur_chaine)/* longueurChaineEnPixels(chaine,hauteur_chaine) */) / 2. ;
}

void afficheImage(zone zITest, DonneesImageRGB *image)
{
	ecrisImage(zITest.xmin, zITest.ymin, zITest.xmax-zITest.xmin, zITest.ymax-zITest.ymin, image->donneesRGB);
}


void initZones(zone *zQuit, zone *zHome, zone *zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix)
{
	// initialisation de la zone zQuit			
	zQuit->xmax = largeurFenetre() - 4 ; 
	zQuit->xmin = zQuit->xmax - croix->largeurImage;
	zQuit->ymax = hauteurFenetre() - 2 ;
	zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

	//initialisation du bouton accueil
	zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
	zHome->xmin = zHome->xmax - home->largeurImage ;
	zHome->ymax = hauteurFenetre()-2;
	zHome->ymin = zHome->ymax - home->hauteurImage ;

	//initialisation du bouton retour
	zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
	zRetour->xmin = zRetour->xmax - retour->largeurImage ;
	zRetour->ymax = hauteurFenetre()-2 ;
	zRetour->ymin = zRetour->ymax - retour->hauteurImage;
}
void redimensionneZones( zone *zQuit, zone *zHome, zone *zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix)
{
	// initialisation de la zone zQuit			
	zQuit->xmax = largeurFenetre() - 4 ; 
	zQuit->xmin = zQuit->xmax - croix->largeurImage;
	zQuit->ymax = hauteurFenetre() - 2 ;
	zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

	//initialisation du bouton accueil
	zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
	zHome->xmin = zHome->xmax - home->largeurImage ;
	zHome->ymax = hauteurFenetre()-2;
	zHome->ymin = zHome->ymax - home->hauteurImage ;

	//initialisation du bouton retour
	zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
	zRetour->xmin = zRetour->xmax - retour->largeurImage ;
	zRetour->ymax = hauteurFenetre()-2 ;
	zRetour->ymin = zRetour->ymax - retour->hauteurImage;
}

// initialise les coordonnées et la chaine de caractère du titre
void initZoneTitre(zone *zTitre, char *titre)
 {
	/* int longueur_chaine = strlen(titre); printf("longeur titre = %d\n", longueur_chaine);
	zTitre->texte = (char*)malloc(sizeof(char) * longueur_chaine);
	strcpy(zTitre->texte, titre); */
	changeTexteZone(zTitre, titre);

	zTitre->hauteur = HAUTEUR_TITRE_ACCUEIL; 
	zTitre->longueur = longueurChaineEnPixels(zTitre->texte,zTitre->hauteur);

	zTitre->xmin = xminChaineCentree(zTitre->texte,zTitre->hauteur);
	zTitre->xmax = zTitre->xmin + zTitre->longueur;
	zTitre->ymax = hauteurFenetre()/2. +zTitre->hauteur/2.; // centré aussi sur la hauteur pour page d'accueil
	zTitre->ymin = zTitre->ymax - zTitre->hauteur ;
	
	// pour titre sur pages apres accueil
	zTitre->espaceHaut = 10; 
	zTitre->espaceBas = 100;
 }


// enregistre la chaine dans la zone 
void changeTexteZone(zone *z, char *chaine)
{
	free(z->texte); z->texte = NULL; //libere la memoire allouee pour le titre précédent
	int longueur_chaine = strlen(chaine);
	z->texte = (char*)malloc(sizeof(char) * longueur_chaine);
	strcpy(z->texte, chaine);
}

// change la zone d'affichage du titre en fonction du numéro de page
// attention le centrage se fait en fonction de zTitre.texte
void changeZoneTitre(zone *zTitre,int numPage)
{
	if(numPage > 1)
	{	
		zTitre->hauteur = HAUTEUR_TITRE; 
		zTitre->longueur = longueurChaineEnPixels(zTitre->texte,zTitre->hauteur);

		zTitre->xmin = xminChaineCentree(zTitre->texte,zTitre->hauteur);
		zTitre->xmax = zTitre->xmin + zTitre->longueur;

		zTitre->espaceHaut = 10; 
		zTitre->espaceBas = 100;

		zTitre->ymax = hauteurFenetre() - zTitre->espaceHaut;
		zTitre->ymin = zTitre->ymax - zTitre->hauteur ;
	}
}

// redimmensionne la zone d'affichage du titre pour que celle-ci
// reste centrée quand on étire la fenêtre
// attention le centrage se fait en fonction de zTitre.texte
void redimmensionneZoneTitre(zone *zTitre)
{

	zTitre->xmin = xminChaineCentree(zTitre->texte,zTitre->hauteur);
	zTitre->xmax = zTitre->xmin + zTitre->longueur;

}
// initialise la zone d'affichage du ptient actuel en fonction de la zone du titre (car se place juste en dessous)
void initZonesPatientActuel(zone zTitre, zone *zPatientActuel, zone *zPrenomPatientActuel, zone *zNomPatientActuel, zone *zVoirFiche)
 {
	// contenant
	zPatientActuel->espaceHaut = 50; 
	zPatientActuel->espaceBas = 50;
	zPatientActuel->espaceGauche = 50; 
	zPatientActuel->espaceDroite = 50;
	zPatientActuel->hauteur = HAUTEUR_TEXTE; 
	changeTexteZone(zPatientActuel,"Patient actuel : ");
	zPatientActuel->longueur = largeurFenetre() - zPatientActuel->espaceGauche - zPatientActuel->espaceDroite ;
	zPatientActuel->xmin = zPatientActuel->espaceGauche;
	zPatientActuel->xmax = zPatientActuel->xmin + zPatientActuel->longueur;
	zPatientActuel->ymax = zTitre.ymin - zPatientActuel->espaceHaut; 
	zPatientActuel->ymin = zPatientActuel->ymax - zPatientActuel->hauteur ;

	// zones interieures
	zVoirFiche->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zVoirFiche,"Voir fiche");
	zVoirFiche->espaceGauche = 50; 
	zVoirFiche->espaceDroite = 50;
	zVoirFiche->longueur = tailleChaine(zVoirFiche->texte,zVoirFiche->hauteur) + 10;
	zVoirFiche->xmin = zPatientActuel->xmax - zVoirFiche->longueur - zVoirFiche->espaceDroite;
	zVoirFiche->xmax = zVoirFiche->xmin + zVoirFiche->longueur;
	zVoirFiche->ymax = zPatientActuel->ymax + 5; 
	zVoirFiche->ymin = zPatientActuel->ymin - 5;

	zPrenomPatientActuel->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPrenomPatientActuel,"Prenom : ");
	zPrenomPatientActuel->espaceGauche = 50; 
	zPrenomPatientActuel->espaceDroite = 50;
	zPrenomPatientActuel->xmin = zPatientActuel->xmin + 10 + tailleChaine(zPrenomPatientActuel->texte, zPrenomPatientActuel->hauteur);
	zPrenomPatientActuel->xmax = zPrenomPatientActuel->xmin + 10;
	zPrenomPatientActuel->ymax = zPatientActuel->ymax; 
	zPrenomPatientActuel->ymin = zPatientActuel->ymin;

	zNomPatientActuel->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zNomPatientActuel,"Nom : ");
	zNomPatientActuel->espaceGauche = 50; 
	zNomPatientActuel->espaceDroite = 50;
	zNomPatientActuel->xmin = zPrenomPatientActuel->xmax + 10 + tailleChaine(zNomPatientActuel->texte, zNomPatientActuel->hauteur);
	zNomPatientActuel->ymax = zPatientActuel->ymax;
	zNomPatientActuel->ymin = zPatientActuel->ymin;
 }

void redimmensionneZonePatientActuel(zone *zPatientActuel, zone zTitre)
 {
	zPatientActuel->longueur = largeurFenetre() - zPatientActuel->espaceGauche - zPatientActuel->espaceDroite ;
	zPatientActuel->xmin = zPatientActuel->espaceGauche;
	zPatientActuel->xmax = zPatientActuel->xmin + zPatientActuel->longueur;
	zPatientActuel->ymax = zTitre.ymin - zPatientActuel->espaceHaut; 
	zPatientActuel->ymin = zPatientActuel->ymax - zPatientActuel->hauteur ;
 }

// recupere le prenom et le nom du patient et les enregistre dans des variables
void gestionNomPrenomPatient(zone *zPrenom, zone *zNom, char **prenom, char **nom)
{
	char chaine [] = "Non renseigne";
	if(*prenom == NULL || *nom == NULL)
	{
		int longueur_chaine = strlen(chaine);
		*prenom = (char*)malloc(sizeof(char) * longueur_chaine);
		*nom = (char*)malloc(sizeof(char) * longueur_chaine);
		strcpy(*prenom, chaine);
		strcpy(*nom, chaine);
	}

	// change la zone d'affichage en fonction des chaines de caractère
	zPrenom->xmax = zPrenom->xmin + tailleChaine(*prenom, zPrenom->hauteur);
	zNom->xmin = zPrenom->xmax + zPrenom->espaceDroite + tailleChaine(zNom->texte,zNom->hauteur) + 10;
	zNom->xmax = zNom->xmin + tailleChaine(*nom, zNom->hauteur);
}

 void affichePatientActuel(zone zPatientActuel, zone zPrenom, char *prenom, zone zNom,char *nom, zone zVoirFiche)
 {
	rectangle(zPatientActuel.xmin,zPatientActuel.ymin,zPatientActuel.xmax,zPatientActuel.ymax);;
	int longueur_chaine_pixel = tailleChaine(zPatientActuel.texte,zPatientActuel.hauteur);
	couleurCourante(100,100,100);
	rectangle(zPatientActuel.xmin, zPatientActuel.ymin, zPatientActuel.xmin + longueur_chaine_pixel, zPatientActuel.ymax);
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zPatientActuel.texte, zPatientActuel.hauteur, zPatientActuel.xmin,zPatientActuel.ymin);

	afficheChaine(zPrenom.texte, zPrenom.hauteur, zPrenom.xmin - tailleChaine(zPrenom.texte, zPrenom.hauteur),zPrenom.ymin);
	afficheChaine(prenom,zPrenom.hauteur, zPrenom.xmin,zPrenom.ymin);

	afficheChaine(zNom.texte, zNom.hauteur, zNom.xmin - tailleChaine(zPrenom.texte, zNom.hauteur),zNom.ymin);
	afficheChaine(zNom.texte, zNom.hauteur, zNom.xmin,zNom.ymin);

	couleurCourante(100,100,100);
	rectangle(zPatientActuel.xmin, zPatientActuel.ymin, zPatientActuel.xmin + longueur_chaine_pixel, zPatientActuel.ymax);
	couleurCourante(0,0,0);
	afficheChaine(zPatientActuel.texte, zPatientActuel.hauteur, zPatientActuel.xmin,zPatientActuel.ymin);
 } 


void monIHM(zone zQuit, zone zHome, zone zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, DonneesImageRGB *logo, int numpage)
{
	int ecart_bord = 10;
	effaceFenetre(0,102,204);

	afficheImage(zQuit,croix);
	
	if(numpage == 1)
		ecrisImage(largeurFenetre() - logo->largeurImage - ecart_bord, ecart_bord, logo->largeurImage, logo->hauteurImage, logo->donneesRGB); //affiche logo
	
	if(numpage > 2) // 2 est la page d'acceuil
	{
		afficheImage(zHome,home);
		afficheImage(zRetour,retour);
	}
}

void afficheTitre(zone zTitre, int epaisseur_trait)
{
/* 	couleurCourante(250,255,250);
	rectangle(zTitre.xmin,zTitre.ymin,zTitre.xmax,zTitre.ymax ); */
	epaisseurDeTrait(epaisseur_trait);
	couleurCourante(204,204,0); // Choix de la couleur 
	afficheChaine(zTitre.texte, zTitre.hauteur, zTitre.xmin , zTitre.ymin+10);
}

void afficheAcceuil(zone zTitre)
{
	afficheTitre(zTitre,5);

	epaisseurDeTrait(1);
	char chaine [] = "Developed by Laura JUBERT and Raphael ROY";
	int hauteur_chaine = 10;
	afficheChaine(chaine, hauteur_chaine, xminChaineCentree(chaine,hauteur_chaine),10);
}



