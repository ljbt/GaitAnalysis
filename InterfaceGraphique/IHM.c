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
	zTitre->longueur = tailleChaine(zTitre->texte,zTitre->hauteur);

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
		zTitre->longueur = tailleChaine(zTitre->texte,zTitre->hauteur);

		zTitre->xmin = xminChaineCentree(zTitre->texte,zTitre->hauteur);
		zTitre->xmax = zTitre->xmin + zTitre->longueur;

		zTitre->espaceHaut = 10; 
		zTitre->espaceBas = 100;

		zTitre->ymax = hauteurFenetre() - zTitre->espaceHaut;
		zTitre->ymin = zTitre->ymax - zTitre->hauteur ;
	}
}

// redimensionne la zone d'affichage du titre pour que celle-ci
// reste centrée quand on étire la fenêtre
// attention le centrage se fait en fonction de zTitre.texte
void redimensionneZoneTitre(zone *zTitre)
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

	zPrenomPatientActuel->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPrenomPatientActuel,"Prenom : ");
	zPrenomPatientActuel->espaceGauche = 5; 
	zPrenomPatientActuel->espaceDroite = 5;
	zPrenomPatientActuel->xmin = zPatientActuel->xmin + tailleChaine(zPatientActuel->texte,zPatientActuel->hauteur) + 10 + tailleChaine(zPrenomPatientActuel->texte, zPrenomPatientActuel->hauteur);
	zPrenomPatientActuel->xmax = zPrenomPatientActuel->xmin + 10;
	zPrenomPatientActuel->ymax = zPatientActuel->ymax; 
	zPrenomPatientActuel->ymin = zPatientActuel->ymin;

	zNomPatientActuel->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zNomPatientActuel,"Nom : ");
	zNomPatientActuel->espaceGauche = 5; 
	zNomPatientActuel->espaceDroite = 5;
	zNomPatientActuel->xmin = zPrenomPatientActuel->xmax + zPrenomPatientActuel->espaceDroite + zNomPatientActuel->espaceGauche + tailleChaine(zNomPatientActuel->texte, zNomPatientActuel->hauteur);
	zNomPatientActuel->ymax = zPatientActuel->ymax;
	zNomPatientActuel->ymin = zPatientActuel->ymin;

	zVoirFiche->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zVoirFiche,"Voir fiche");
	zVoirFiche->espaceGauche = 10; 
	zVoirFiche->espaceDroite = 5;
	zVoirFiche->longueur = tailleChaine(zVoirFiche->texte,zVoirFiche->hauteur);
	zVoirFiche->xmin = zNomPatientActuel->xmax + zVoirFiche->espaceGauche;
	zVoirFiche->xmax = zVoirFiche->xmin + zVoirFiche->longueur;
	zVoirFiche->ymax = zPatientActuel->ymax + 5; 
	zVoirFiche->ymin = zPatientActuel->ymin - 5;
 }

void redimensionneZonePatientActuel(zone zTitre, zone *zPatientActuel, zone *zVoirFiche, zone *zPrenomPatientActuel, zone *zNomPatientActuel )
 {
	zPatientActuel->longueur = largeurFenetre() - zPatientActuel->espaceGauche - zPatientActuel->espaceDroite ;
	zPatientActuel->xmin = zPatientActuel->espaceGauche;
	zPatientActuel->xmax = zPatientActuel->xmin + zPatientActuel->longueur;
	zPatientActuel->ymax = zTitre.ymin - zPatientActuel->espaceHaut; 
	zPatientActuel->ymin = zPatientActuel->ymax - zPatientActuel->hauteur ;

	zPrenomPatientActuel->xmin = zPatientActuel->xmin + tailleChaine(zPatientActuel->texte,zPatientActuel->hauteur) + 10 + tailleChaine(zPrenomPatientActuel->texte, zPrenomPatientActuel->hauteur);
	zPrenomPatientActuel->xmax = zPrenomPatientActuel->xmin + zPrenomPatientActuel->longueur;
	zPrenomPatientActuel->ymax = zPatientActuel->ymax; 
	zPrenomPatientActuel->ymin = zPatientActuel->ymin;

	zNomPatientActuel->xmin = zPrenomPatientActuel->xmax + zPrenomPatientActuel->espaceDroite + zNomPatientActuel->espaceGauche + tailleChaine(zNomPatientActuel->texte, zNomPatientActuel->hauteur);
	zNomPatientActuel->xmax = zNomPatientActuel->xmin + zNomPatientActuel->longueur;
	zNomPatientActuel->ymax = zPatientActuel->ymax;
	zNomPatientActuel->ymin = zPatientActuel->ymin;

	zVoirFiche->xmin = zNomPatientActuel->xmax + zNomPatientActuel->espaceDroite +	 zVoirFiche->espaceGauche;
	zVoirFiche->xmax = zVoirFiche->xmin + zVoirFiche->longueur + 20;
	zVoirFiche->ymax = zPatientActuel->ymax + 5; 
	zVoirFiche->ymin = zPatientActuel->ymin - 5;

 }

// recupere le prenom et le nom du patient et les enregistre dans des variables
void gestionNomPrenomPatient(zone *zPrenom, zone *zNom, char **prenom, char **nom)
{
	//char chaine [] = "Non renseigne";
	if(*prenom != NULL || *nom != NULL)
	{
/* 		int longueur_chaine = strlen(chaine);
		*prenom = (char*)malloc(sizeof(char) * longueur_chaine);
		*nom = (char*)malloc(sizeof(char) * longueur_chaine);
		strcpy(*prenom, chaine);
		strcpy(*nom, chaine); */
		zPrenom->longueur = tailleChaine(*prenom, zPrenom->hauteur);
		zNom->longueur = tailleChaine(*nom, zNom->hauteur);
	}
}

 void affichePatientActuel(zone zPatientActuel, zone zPrenom, char *prenom, zone zNom,char *nom, zone zVoirFiche)
 {
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zPatientActuel.texte, zPatientActuel.hauteur, zPatientActuel.xmin,zPatientActuel.ymin);

	afficheChaine(zPrenom.texte, zPrenom.hauteur, zPrenom.xmin - tailleChaine(zPrenom.texte,zPrenom.hauteur),zPrenom.ymin);
	afficheChaine(zNom.texte, zNom.hauteur, zNom.xmin - tailleChaine(zNom.texte,zNom.hauteur),zNom.ymin);
	if(prenom != NULL)	
		afficheChaine(prenom,zPrenom.hauteur, zPrenom.xmin,zPrenom.ymin);
	if( nom != NULL)
		afficheChaine(nom, zNom.hauteur, zNom.xmin,zNom.ymin);
	
	if( prenom != NULL &&  nom != NULL)
	{
		couleurCourante(204,204,0);
		rectangle(zVoirFiche.xmin, zVoirFiche.ymin, zVoirFiche.xmax, zVoirFiche.ymax);
		couleurCourante(0,0,0);
		afficheChaine(zVoirFiche.texte, zVoirFiche.hauteur, zVoirFiche.xmin+10,zVoirFiche.ymin+5);
	}
 } 

// fonction qui place les zones de la partie patient à charger en dessous de la zone patient actuel
void initZonesChargerPatient(zone zPatientActuel, zone *zChargerPatient, zone *zPrenom, zone *zNom, zone *zCharger)
{
	// contenant
	zChargerPatient->espaceHaut = 50; 
	zChargerPatient->espaceBas = 50;
	zChargerPatient->espaceGauche = 50; 
	zChargerPatient->espaceDroite = 50;
	zChargerPatient->hauteur = HAUTEUR_TEXTE; 
	changeTexteZone(zChargerPatient,"Charger patient : ");
	zChargerPatient->longueur = largeurFenetre() - zChargerPatient->espaceGauche - zChargerPatient->espaceDroite ;
	zChargerPatient->xmin = zChargerPatient->espaceGauche;
	zChargerPatient->xmax = zChargerPatient->xmin + zChargerPatient->longueur;
	zChargerPatient->ymax = zPatientActuel.ymin - zChargerPatient->espaceHaut; 
	zChargerPatient->ymin = zChargerPatient->ymax - zChargerPatient->hauteur ;

	// zones interieures

	zPrenom->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPrenom,"Prenom : ");
	zPrenom->espaceGauche = 10; 
	zPrenom->espaceDroite = 10;
	zPrenom->xmin = zChargerPatient->xmin + tailleChaine(zChargerPatient->texte,zChargerPatient->hauteur) + 10 + tailleChaine(zPrenom->texte, zPrenom->hauteur);
	zPrenom->xmax = zPrenom->xmin + 150;
	zPrenom->ymax = zChargerPatient->ymax; 
	zPrenom->ymin = zChargerPatient->ymin;
	zPrenom->saisie = false;

	zNom->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zNom,"Nom : ");
	zNom->espaceGauche = 10; 
	zNom->espaceDroite = 10;
	zNom->xmin = zPrenom->xmax + zPrenom->espaceDroite + zNom->espaceGauche +  tailleChaine(zNom->texte, zNom->hauteur);
	zNom->xmax = zNom->xmin + 150;
	zNom->ymax = zChargerPatient->ymax;
	zNom->ymin = zChargerPatient->ymin;
	zNom->saisie = false;

	zCharger->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zCharger,"Charger donnees");
	zCharger->espaceGauche = 20; 
	zCharger->espaceDroite = 5;
	zCharger->longueur = tailleChaine(zCharger->texte,zCharger->hauteur);
	zCharger->xmin = zNom->xmax + zNom->espaceDroite + zCharger->espaceGauche;
	zCharger->xmax = zCharger->xmin + zCharger->longueur;
	zCharger->ymax = zChargerPatient->ymax + 5; 
	zCharger->ymin = zChargerPatient->ymin - 5;
}

void afficheChargerPatient(zone zChargerPatient, zone zPrenom, char *prenom, zone zNom, char *nom, zone zCharger)
{
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zChargerPatient.texte, zChargerPatient.hauteur, zChargerPatient.xmin,zChargerPatient.ymin);

	afficheChaine(zPrenom.texte, zPrenom.hauteur, zPrenom.xmin - tailleChaine(zPrenom.texte,zPrenom.hauteur),zPrenom.ymin);
	afficheChaine(zNom.texte, zNom.hauteur, zNom.xmin - tailleChaine(zNom.texte,zNom.hauteur),zNom.ymin);
	
	if(zPrenom.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zPrenom.xmin, zPrenom.ymin-5, zPrenom.xmax+10, zPrenom.ymax+5);

	if(zNom.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zNom.xmin, zNom.ymin-5, zNom.xmax+10, zNom.ymax+5);

	couleurCourante(0,0,0);
	if(prenom != NULL)
		afficheChaine(prenom,zPrenom.hauteur, zPrenom.xmin+5,zPrenom.ymin+5);
	if(nom != NULL)
		afficheChaine(nom, zNom.hauteur, zNom.xmin+5,zNom.ymin+5);

	if((prenom != NULL && nom != NULL) && (strlen(prenom) && strlen(nom))) // On test si non null et non vide 
	{	
		couleurCourante(204,204,0);
		rectangle(zCharger.xmin, zCharger.ymin, zCharger.xmax+20, zCharger.ymax);
		couleurCourante(0,0,0);
		afficheChaine(zCharger.texte, zCharger.hauteur, zCharger.xmin+10,zCharger.ymin+5);
	}		
}

void redimensionneZoneChargerPatient(zone zPatientActuel,zone *zChargerPatient,zone *zPrenom,zone *zNom,zone *zCharger)
{
	// contenant
	zChargerPatient->longueur = largeurFenetre() - zChargerPatient->espaceGauche - zChargerPatient->espaceDroite ;
	zChargerPatient->xmin = zChargerPatient->espaceGauche;
	zChargerPatient->xmax = zChargerPatient->xmin + zChargerPatient->longueur;
	zChargerPatient->ymax = zPatientActuel.ymin - zChargerPatient->espaceHaut;
	zChargerPatient->ymin = zChargerPatient->ymax - zChargerPatient->hauteur ;

	// zones interieures
	zPrenom->ymax = zChargerPatient->ymax; 
	zPrenom->ymin = zChargerPatient->ymin;

	zNom->ymax = zChargerPatient->ymax;
	zNom->ymin = zChargerPatient->ymin;

	zCharger->ymax = zChargerPatient->ymax + 5; 
	zCharger->ymin = zChargerPatient->ymin - 5;
}

// fonction qui place les zones de la partie patient à créer en dessous de la zone patient à charger
void initZonesNouveauPatient(zone zChargerPatient, zone *zNouveauPatient, zone *zPrenom, zone *zNom, zone *zTaille, zone *zPoids, zone *zCreerPatient)
{
	// contenant
	zNouveauPatient->espaceHaut = 50; 
	zNouveauPatient->espaceBas = 50;
	zNouveauPatient->espaceGauche = 50; 
	zNouveauPatient->espaceDroite = 50;
	zNouveauPatient->hauteur = HAUTEUR_TEXTE; 
	changeTexteZone(zNouveauPatient,"Creer patient : ");
	zNouveauPatient->longueur = largeurFenetre() - zNouveauPatient->espaceGauche - zNouveauPatient->espaceDroite ;
	zNouveauPatient->xmin = zNouveauPatient->espaceGauche;
	zNouveauPatient->xmax = zNouveauPatient->xmin + zNouveauPatient->longueur;
	zNouveauPatient->ymax = zChargerPatient.ymin - zNouveauPatient->espaceHaut; 
	zNouveauPatient->ymin = zNouveauPatient->ymax - 2*zNouveauPatient->hauteur -zNouveauPatient->espaceBas ;

	// zones interieures

	zPrenom->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPrenom,"Prenom : ");
	zPrenom->espaceGauche = 10; 
	zPrenom->espaceDroite = 10;
	zPrenom->espaceBas = 10;
	zPrenom->xmin = zNouveauPatient->xmin + tailleChaine(zNouveauPatient->texte,zNouveauPatient->hauteur) + 10 + tailleChaine(zPrenom->texte, zPrenom->hauteur);
	zPrenom->xmax = zPrenom->xmin + 150;
	zPrenom->ymax = zNouveauPatient->ymax; 
	zPrenom->ymin = zNouveauPatient->ymax - zNouveauPatient->hauteur;
	zPrenom->saisie = false;

	zNom->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zNom,"Nom : ");
	zNom->espaceGauche = 10; 
	zNom->espaceDroite = 10;
	zNom->espaceBas = 10;
	zNom->xmin = zPrenom->xmax + zPrenom->espaceDroite + zNom->espaceGauche +  tailleChaine(zNom->texte, zNom->hauteur);
	zNom->xmax = zNom->xmin + 150;
	zNom->ymax = zNouveauPatient->ymax;
	zNom->ymin = zNouveauPatient->ymax - zNouveauPatient->hauteur;
	zNom->saisie = false;


	zTaille->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zTaille,"Taille : ");
	zTaille->espaceGauche = 10; 
	zTaille->espaceDroite = 10;
	zTaille->espaceHaut = 10;
	zTaille->xmin = zNouveauPatient->xmin + tailleChaine(zNouveauPatient->texte,zNouveauPatient->hauteur) + 10 + tailleChaine(zTaille->texte, zTaille->hauteur);
	zTaille->xmax = zTaille->xmin + 150;
	zTaille->ymax = zPrenom->ymin - zPrenom->espaceBas - zTaille->espaceHaut;
	zTaille->ymin = zTaille->ymax - zTaille->hauteur;
	zTaille->saisie = false;	

 	zPoids->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPoids,"Poids : ");
	zPoids->espaceGauche = 10; 
	zPoids->espaceDroite = 10;
	zPoids->espaceHaut = 10;
	zPoids->xmin = zTaille->xmax + zTaille->espaceDroite + zPoids->espaceGauche +  tailleChaine(zPoids->texte, zPoids->hauteur);
	zPoids->xmax = zPoids->xmin + 150;
	zPoids->ymax = zPrenom->ymin - zPrenom->espaceBas - zPoids->espaceHaut;
	zPoids->ymin = zPoids->ymax - zPoids->hauteur;
	zPoids->saisie = false; 


	zCreerPatient->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zCreerPatient,"Creer patient");
	zCreerPatient->espaceGauche = 20; 
	zCreerPatient->espaceDroite = 5;
	zCreerPatient->longueur = tailleChaine(zCreerPatient->texte,zCreerPatient->hauteur);
	zCreerPatient->xmin = zNom->xmax + zNom->espaceDroite + zCreerPatient->espaceGauche;
	zCreerPatient->xmax = zCreerPatient->xmin + zCreerPatient->longueur;
	zCreerPatient->ymax = zNouveauPatient->ymax + 5; 
	zCreerPatient->ymin = zTaille->ymin - 5;

}

void afficheNouveauPatient(zone zNouveauPatient, zone zPrenom, char *prenom, zone zNom, char *nom, zone zTaille, char *taille, zone zPoids, char *poids, zone zCreerPatient)
{
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zNouveauPatient.texte, zNouveauPatient.hauteur, zNouveauPatient.xmin,zNouveauPatient.ymax - zNouveauPatient.hauteur);

	afficheChaine(zPrenom.texte, zPrenom.hauteur, zPrenom.xmin - tailleChaine(zPrenom.texte,zPrenom.hauteur),zPrenom.ymin);
	afficheChaine(zNom.texte, zNom.hauteur, zNom.xmin - tailleChaine(zNom.texte,zNom.hauteur),zNom.ymin);
	afficheChaine(zTaille.texte, zTaille.hauteur, zTaille.xmin - tailleChaine(zTaille.texte,zTaille.hauteur),zTaille.ymin);
	afficheChaine(zPoids.texte, zPoids.hauteur, zPoids.xmin - tailleChaine(zPoids.texte,zPoids.hauteur),zPoids.ymin);
	
	if(zPrenom.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zPrenom.xmin, zPrenom.ymin-5, zPrenom.xmax+10, zPrenom.ymax+5);

	if(zNom.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zNom.xmin, zNom.ymin-5, zNom.xmax+10, zNom.ymax+5);

	if(zTaille.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zTaille.xmin, zTaille.ymin-5, zTaille.xmax+10, zTaille.ymax+5);

	if(zPoids.saisie) couleurCourante(255,255,255);
	else couleurCourante(204,204,0);
	rectangle(zPoids.xmin, zPoids.ymin-5, zPoids.xmax+10, zPoids.ymax+5);


	couleurCourante(0,0,0);
	if(prenom != NULL)
		afficheChaine(prenom,zPrenom.hauteur, zPrenom.xmin+5,zPrenom.ymin+5);
	if(nom != NULL)
		afficheChaine(nom, zNom.hauteur, zNom.xmin+5,zNom.ymin+5);
	if(taille != NULL)
	{
		afficheChaine(taille, zTaille.hauteur, zTaille.xmin+5,zTaille.ymin+5);
	}
	if(poids != NULL)
	{
		afficheChaine(poids, zPoids.hauteur, zPoids.xmin+5,zPoids.ymin+5);
	}

	if((prenom != NULL && nom != NULL && taille != NULL && poids != NULL) && (strlen(prenom) && strlen(nom) && strlen(taille) && strlen(poids))) // On test si non null et non vide 
	{	
		couleurCourante(204,204,0);
		rectangle(zCreerPatient.xmin, zCreerPatient.ymin, zCreerPatient.xmax+20, zCreerPatient.ymax);
		couleurCourante(0,0,0);
		afficheChaine(zCreerPatient.texte, zCreerPatient.hauteur, zCreerPatient.xmin+10,zCreerPatient.ymin+25);
	}		
}

void redimensionneZoneNouveauPatient(zone zChargerPatient,zone *zNouveauPatient,zone *zPrenom,zone *zNom, zone *zTaille,zone *zPoids,zone *zCreerPatient)
{
	// contenant
	zNouveauPatient->longueur = largeurFenetre() - zNouveauPatient->espaceGauche - zNouveauPatient->espaceDroite ;
	zNouveauPatient->xmin = zNouveauPatient->espaceGauche;
	zNouveauPatient->xmax = zNouveauPatient->xmin + zNouveauPatient->longueur;
	zNouveauPatient->ymax = zChargerPatient.ymin - zNouveauPatient->espaceHaut; 
	zNouveauPatient->ymin = zNouveauPatient->ymax - 2*zNouveauPatient->hauteur -zNouveauPatient->espaceBas ;

	// zones interieures
	zPrenom->ymax = zNouveauPatient->ymax; 
	zPrenom->ymin = zNouveauPatient->ymax - zNouveauPatient->hauteur;

	zNom->ymax = zNouveauPatient->ymax;
	zNom->ymin = zNouveauPatient->ymax - zNouveauPatient->hauteur;

	zTaille->ymax = zPrenom->ymin - zPrenom->espaceBas - zTaille->espaceHaut;
	zTaille->ymin = zTaille->ymax - zTaille->hauteur;

	zPoids->ymax = zPrenom->ymin - zPrenom->espaceBas - zPoids->espaceHaut;
	zPoids->ymin = zPoids->ymax - zPoids->hauteur;

	zCreerPatient->ymax = zNouveauPatient->ymax + 5; 
	zCreerPatient->ymin = zTaille->ymin - 5;
}



// entre les caractères tapés au clavier dans la chaine
void recupereTexte(char **chaine)
{
	if(*chaine == NULL)
	{
		*chaine = (char *)malloc(sizeof(char));
		(*chaine)[0] = '\0';
	}
	int longueur_chaine = strlen(*chaine);
	char caractere = '\0';
	caractere = caractereClavier(); //recupere caractere tapé par utilisateur
	if(caractere != '\0')
	{
		if((int)caractere == 8 && longueur_chaine > 0) // touche effacer
		{
			if(longueur_chaine == 1)
				*chaine = (char *)realloc(*chaine, sizeof(char));
			else *chaine = (char *)realloc(*chaine, sizeof(char) * longueur_chaine-1);
			(*chaine)[longueur_chaine-1] = '\0';			
		}
		else
		{
			*chaine = (char *)realloc(*chaine, sizeof(char) * longueur_chaine+1);
			(*chaine)[longueur_chaine] = caractere;
			(*chaine)[longueur_chaine+1] = '\0';			
		}
	}
}

void recupereTexteEntiers(char **chaine)
{
	if(*chaine == NULL)
	{
		*chaine = (char *)malloc(sizeof(char));
		(*chaine)[0] = '\0';
	}
	int longueur_chaine = strlen(*chaine);
	char caractere = '\0';
	caractere = caractereClavier(); //recupere caractere tapé par utilisateur 
	if(caractere != '\0' && ((caractere>=48 && caractere <= 57) || caractere == 8)) // on verifie que le caractere entré est un chiffre
	{
		if((int)caractere == 8 && longueur_chaine > 0) // touche effacer
		{
			if(longueur_chaine == 1)
				*chaine = (char *)realloc(*chaine, sizeof(char));
			else *chaine = (char *)realloc(*chaine, sizeof(char) * longueur_chaine-1);
			(*chaine)[longueur_chaine-1] = '\0';			
		}
		else
		{
			*chaine = (char *)realloc(*chaine, sizeof(char) * longueur_chaine+1);
			(*chaine)[longueur_chaine] = caractere;
			(*chaine)[longueur_chaine+1] = '\0';			
		}
	}
}
