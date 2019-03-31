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

//Fonctions de la page principal de l'interface

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

// initialise les coordonnées et la chaine de caractère du titre
void initZoneTitre(zone *zTitre, char *titre)
 {
	/* int longueur_chaine = strlen(titre); printf("longeur titre = %d\n", longueur_chaine);
	zTitre->texte = (char*)malloc(sizeof(char) * longueur_chaine);
	strcpy(zTitre->texte, titre); */
	changeTitre(zTitre, titre);

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

// enregistre le titre dans la zone titre
void changeTitre(zone *zTitre, char *titre)
{
	free(zTitre->texte); zTitre->texte = NULL; //libere la memoire allouee pour le titre précédent
	int longueur_chaine = strlen(titre);
	zTitre->texte = (char*)malloc(sizeof(char) * longueur_chaine);
	strcpy(zTitre->texte, titre);
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

void monIHM(zone zQuit, zone zHome, zone zRetour, int numpage, int LargeurFenetre)
{
	int ecart_bord = 10;
	DonneesImageRGB *retour, *logo, *croix, *home;
	effaceFenetre(0,102,204);
	logo = lisBMPRGB("logo.bmp");
	croix = lisBMPRGB("croix.bmp");
	home = lisBMPRGB("home.bmp");
	retour = lisBMPRGB("retour.bmp");
	
	afficheImage(zQuit,croix);
	
	if(numpage == 1)
		ecrisImage(LargeurFenetre - logo->largeurImage - ecart_bord, ecart_bord, logo->largeurImage, logo->hauteurImage, logo->donneesRGB); //affiche logo
	
	
	if(numpage > 2) // 2 est la page d'acceuil
	{
		afficheImage(zHome,home);
		afficheImage(zRetour,retour);
	}
}

void afficheTitre(zone zTitre)
{
	couleurCourante(250,255,250);
	rectangle(zTitre.xmin,zTitre.ymin,zTitre.xmax,zTitre.ymax );
	epaisseurDeTrait(5);
	couleurCourante(204,204,0); // Choix de la couleur 
	afficheChaine(zTitre.texte, zTitre.hauteur, zTitre.xmin , zTitre.ymin+10);
}

void afficheAcceuil(zone zTitre)
{
	afficheTitre(zTitre);

	epaisseurDeTrait(1);
	char chaine [] = "Developed by Laura JUBERT and Raphael ROY";
	int hauteur_chaine = 10;
	afficheChaine(chaine, hauteur_chaine, xminChaineCentree(chaine,hauteur_chaine),10);
}

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
	return (largeurFenetre() - longueurChaineEnPixels(chaine,hauteur_chaine)) / 2. ;
}

void afficheMenu(zone zP1)
{ 
	
	couleurCourante(0,100,255);
	rectangle(zP1.xmin, zP1.ymin, zP1.xmax, zP1.ymax);
	
	epaisseurDeTrait(4);
	couleurCourante(230,230,230);
	afficheChaine("zone 1", 30, zP1.xmin+10 , zP1.ymin +10);
	

}


void afficheImage(zone zITest, DonneesImageRGB *image)
{
	ecrisImage(zITest.xmin, zITest.ymin, zITest.xmax-zITest.xmin, zITest.ymax-zITest.ymin, image->donneesRGB);
}


