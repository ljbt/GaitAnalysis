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

void initZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, int LargeurFenetre, int HauteurFenetre, char *titre)
{
			// initialisation de la zone zQuit			
			zQuit->xmax = LargeurFenetre - 4 ; 
			zQuit->xmin = zQuit->xmax - croix->largeurImage;
			zQuit->ymax = HauteurFenetre - 2 ;
			zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

			//initialisation du bouton accueil
			zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
			zHome->xmin = zHome->xmax - home->largeurImage ;
			zHome->ymax = HauteurFenetre-2;
			zHome->ymin = zHome->ymax - home->hauteurImage ;

			//initialisation du bouton retour
			zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
			zRetour->xmin = zRetour->xmax - retour->largeurImage ;
			zRetour->ymax = HauteurFenetre-2 ;
			zRetour->ymin = zRetour->ymax - retour->hauteurImage;

			//initialisation de la zone Titre		
			zTitre->longueur = 650; //choix de la longueur de la zone
			zTitre->hauteur = 60; //pareil mais pour la hauteur
			zTitre->espaceHaut = 10;
			zTitre->espaceBas = 100;
			zTitre->xmin = (LargeurFenetre - zTitre->longueur)/2; //cette formule sert à créer une zone centrée horizontalement
			zTitre->xmax = zTitre->xmin + zTitre->longueur;
			zTitre->ymax = HauteurFenetre - zTitre->espaceHaut;
			zTitre->ymin = zTitre->ymax - zTitre->hauteur ;
			strcpy(zTitre->texte, titre);

}

void redimensionneZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, int LargeurFenetre, int HauteurFenetre)
{
			// initialisation de la zone zQuit			
			zQuit->xmax = LargeurFenetre - 4 ; 
			zQuit->xmin = zQuit->xmax - croix->largeurImage;
			zQuit->ymax = HauteurFenetre - 2 ;
			zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

			//initialisation du bouton accueil
			zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
			zHome->xmin = zHome->xmax - home->largeurImage ;
			zHome->ymax = HauteurFenetre-2;
			zHome->ymin = zHome->ymax - home->hauteurImage ;

			//initialisation du bouton retour
			zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
			zRetour->xmin = zRetour->xmax - retour->largeurImage ;
			zRetour->ymax = HauteurFenetre-2 ;
			zRetour->ymin = zRetour->ymax - retour->hauteurImage;

			//initialisation de la zone Titre		
			zTitre->longueur = 650; //choix de la longueur de la zone
			zTitre->hauteur = 60; //pareil mais pour la hauteur
			zTitre->espaceHaut = 10;
			zTitre->espaceBas = 100;
			zTitre->xmin = (LargeurFenetre - zTitre->longueur)/2; //cette formule sert à créer une zone centrée horizontalement
			zTitre->xmax = zTitre->xmin + zTitre->longueur;
			zTitre->ymax = HauteurFenetre - zTitre->espaceHaut;
			zTitre->ymin = zTitre->ymax - zTitre->hauteur ;

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

	ecrisImage(LargeurFenetre - logo->largeurImage - ecart_bord, ecart_bord, logo->largeurImage, logo->hauteurImage, logo->donneesRGB); //affiche logo
	afficheImage(zQuit,croix);
	
	if(numpage > 1)
	{
		afficheImage(zHome,home);
		afficheImage(zRetour,retour);
	}
	

}

void afficheTitre(zone zTitre)
{
	couleurCourante(0,100,255);
	rectangle(zTitre.xmin, zTitre.ymin, zTitre.xmax, zTitre.ymax);
	
	epaisseurDeTrait(3);
	couleurCourante(0,0,250); // Choix de la couleur 
	afficheChaine(zTitre.texte, 40, zTitre.xmin + (zTitre.longueur/4) + 50 , zTitre.ymin+10);
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


