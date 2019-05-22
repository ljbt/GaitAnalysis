#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>
#include <complex.h>

extern "C"
{
	#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
	#include "ESLib.h"	
	#include "OutilsLib.h"	
	#include "BmpLib.h"

	#include "definitions.h"
	#include "IHM.h"
	#include "fonctionsTraitementImage.h"
}

void monIHM(zone zQuit, zone zHome, zone zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, DonneesImageRGB *logo, int numpage)
{
	int ecart_bord = 10;
	effaceFenetre(233,233,233);

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
	couleurCourante(26,71,255); // Choix de la couleur
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
		couleurCourante(26,71,255);
		rectangle(zVoirFiche.xmin, zVoirFiche.ymin, zVoirFiche.xmax, zVoirFiche.ymax);
		couleurCourante(255,255,255);
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
	zCharger->espaceGauche = 15;
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
	else couleurCourante(179,193,255);
	rectangle(zPrenom.xmin, zPrenom.ymin-5, zPrenom.xmax+10, zPrenom.ymax+5);

	if(zNom.saisie) couleurCourante(255,255,255);
	else couleurCourante(179,193,255);
	rectangle(zNom.xmin, zNom.ymin-5, zNom.xmax+10, zNom.ymax+5);

	couleurCourante(0,0,0);
	if(prenom != NULL)
		afficheChaine(prenom,zPrenom.hauteur, zPrenom.xmin+5,zPrenom.ymin+5);
	if(nom != NULL)
		afficheChaine(nom, zNom.hauteur, zNom.xmin+5,zNom.ymin+5);

	if((prenom != NULL && nom != NULL) && (strlen(prenom) && strlen(nom))) // On test si non null et non vide
	{
		couleurCourante(26,71,255);
		rectangle(zCharger.xmin, zCharger.ymin, zCharger.xmax+20, zCharger.ymax);
		couleurCourante(255,255,255);
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
	else couleurCourante(179,193,255);
	rectangle(zPrenom.xmin, zPrenom.ymin-5, zPrenom.xmax+10, zPrenom.ymax+5);

	if(zNom.saisie) couleurCourante(255,255,255);
	else couleurCourante(179,193,255);
	rectangle(zNom.xmin, zNom.ymin-5, zNom.xmax+10, zNom.ymax+5);

	if(zTaille.saisie) couleurCourante(255,255,255);
	else couleurCourante(179,193,255);
	rectangle(zTaille.xmin, zTaille.ymin-5, zTaille.xmax+10, zTaille.ymax+5);

	if(zPoids.saisie) couleurCourante(255,255,255);
	else couleurCourante(179,193,255);
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
		couleurCourante(26,71,255);
		rectangle(zCreerPatient.xmin, zCreerPatient.ymin, zCreerPatient.xmax+20, zCreerPatient.ymax);
		couleurCourante(255,255,255);
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

void initZonesAnalyseVideo(zone zPatientActuel, zone * zAnalyseVideo, zone *zNomVideo, zone *zChargerVideo, zone *zVideoOrigine, zone *zAnalyser, zone *zAnalyseEnCours)
{
	// contenant
	zAnalyseVideo->espaceHaut = 50;
	zAnalyseVideo->espaceBas = 50;
	zAnalyseVideo->espaceGauche = 50;
	zAnalyseVideo->espaceDroite = 50;
	zAnalyseVideo->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zAnalyseVideo,"Nouvelle analyse : ");
	zAnalyseVideo->longueur = largeurFenetre()/2 - zAnalyseVideo->espaceGauche - zAnalyseVideo->espaceDroite ;
	zAnalyseVideo->xmin = largeurFenetre()/2 + zAnalyseVideo->espaceGauche;
	zAnalyseVideo->xmax = zAnalyseVideo->xmin + zAnalyseVideo->longueur;
	zAnalyseVideo->ymax = zPatientActuel.ymin - zAnalyseVideo->espaceHaut;
	zAnalyseVideo->ymin = zAnalyseVideo->espaceBas ;


	// zones interieures

	zNomVideo->hauteur = HAUTEUR_TEXTE;
	zNomVideo->espaceGauche = 10;
	zNomVideo->espaceDroite = 10;
	zNomVideo->espaceBas = 10;
	zNomVideo->espaceHaut = 15;
	zNomVideo->longueur = 300;
	//zNomVideo->xmin = (largeurFenetre() - zNomVideo->longueur)/2;
	zNomVideo->xmin = zAnalyseVideo->xmin;
	zNomVideo->xmax = zAnalyseVideo->xmin + zNomVideo->longueur;
	zNomVideo->ymax = zAnalyseVideo->ymax - zAnalyseVideo->hauteur - zNomVideo->espaceHaut;
	zNomVideo->ymin = zNomVideo->ymax - zNomVideo->hauteur;
	zNomVideo->saisie = false;

	zChargerVideo->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zChargerVideo,"Charger");
	zChargerVideo->espaceGauche = 10;
	zChargerVideo->espaceDroite = 10;
	zChargerVideo->longueur = tailleChaine(zChargerVideo->texte,zChargerVideo->hauteur);
	zChargerVideo->xmin = zNomVideo->xmax + zChargerVideo->espaceGauche;
	zChargerVideo->xmax = zChargerVideo->xmin + zChargerVideo->longueur;
	zChargerVideo->ymax = zNomVideo->ymax;
	zChargerVideo->ymin = zNomVideo->ymin;


	zVideoOrigine->hauteur = 281; // img dim
	zVideoOrigine->longueur = 500; // img dim
	zVideoOrigine->espaceGauche = 10;
	zVideoOrigine->espaceDroite = 10;
	zVideoOrigine->espaceBas = 10;
	zVideoOrigine->espaceHaut = 10;
	//zVideoOrigine->xmin = (largeurFenetre() - zVideoOrigine->longueur)/2;
	zVideoOrigine->xmin = zAnalyseVideo->xmin;
	zVideoOrigine->xmax = zAnalyseVideo->xmin + zVideoOrigine->longueur;
	zVideoOrigine->ymax = zNomVideo->ymin - zVideoOrigine->espaceHaut;
	zVideoOrigine->ymin = zVideoOrigine->ymax - zVideoOrigine->hauteur;


	zAnalyser->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zAnalyser,"Analyser");
	zAnalyser->espaceGauche = 10;
	zAnalyser->espaceDroite = 10;
	zAnalyser->espaceBas = 10;
	zAnalyser->espaceHaut = 10;
	zAnalyser->longueur = tailleChaine(zAnalyser->texte,zAnalyser->hauteur);
	zAnalyser->xmin =  zAnalyseVideo->xmin;
	zAnalyser->xmax = zAnalyseVideo->xmin + zAnalyser->longueur;
	zAnalyser->ymax = zVideoOrigine->ymin - zAnalyser->espaceHaut;
	zAnalyser->ymin = zAnalyser->ymax - zAnalyser->hauteur;

 	zAnalyseEnCours->hauteur = 50; // img dim
	zAnalyseEnCours->longueur = 50; // img dim
	zAnalyseEnCours->espaceGauche = 10;
	zAnalyseEnCours->espaceDroite = 10;
	zAnalyseEnCours->espaceBas = 10;
	zAnalyseEnCours->espaceHaut = 10;
	zAnalyseEnCours->xmin = zAnalyseVideo->xmin;
	zAnalyseEnCours->xmax = zAnalyseVideo->xmin + zAnalyseEnCours->longueur;
	zAnalyseEnCours->ymax = zAnalyser->ymin - zAnalyseEnCours->espaceHaut;
	zAnalyseEnCours->ymin = zAnalyseEnCours->ymax - zAnalyseEnCours->hauteur;
}

void afficheAnalyseVideo(zone zAnalyseVideo, zone zNomVideo, zone zChargerVideo, char* nomVideo, zone zVideoOrigine, DonneesImageRGB* video, zone zAnalyser, zone zAnalyseEnCours, bool enCours, DonneesImageRGB* fleche)
{
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zAnalyseVideo.texte,zAnalyseVideo.hauteur, zAnalyseVideo.xmin,zAnalyseVideo.ymax - zAnalyseVideo.hauteur);

	if(zNomVideo.saisie) couleurCourante(255,255,255);
	else couleurCourante(179,193,255);
	rectangle(zNomVideo.xmin, zNomVideo.ymin-5, zNomVideo.xmax, zNomVideo.ymax+5);

	//couleurCourante(255,0,0);
	//rectangle(zVideoOrigine.xmin, zVideoOrigine.ymin, zVideoOrigine.xmax, zVideoOrigine.ymax); // video
	if (video != NULL)
	{
		afficheImage(zVideoOrigine, video);
		couleurCourante(26,71,255);
		rectangle(zAnalyser.xmin, zAnalyser.ymin, zAnalyser.xmax+20, zAnalyser.ymax + 5);
		couleurCourante(255,255,255);
		afficheChaine(zAnalyser.texte, zAnalyser.hauteur, zAnalyser.xmin+10,zAnalyser.ymin+5);
		if (enCours)
		{
			afficheImage(zAnalyseEnCours, fleche);
		}
	}

	
	couleurCourante(0,0,0);
	if(nomVideo != NULL)
		afficheChaine(nomVideo,zNomVideo.hauteur, zNomVideo.xmin+5,zNomVideo.ymin+5);

	if(nomVideo != NULL && strlen(nomVideo)) // On test si non null et non vide
	{
		couleurCourante(26,71,255);
		rectangle(zChargerVideo.xmin, zChargerVideo.ymin, zChargerVideo.xmax+20, zChargerVideo.ymax + 5);
		couleurCourante(255,255,255);
		afficheChaine(zChargerVideo.texte, zChargerVideo.hauteur, zChargerVideo.xmin+10,zChargerVideo.ymin+5);
	}
}

//void redimensionneZoneChargerPatient(zone zPatientActuel,zone *zChargerPatient,zone *zPrenom,zone *zNom,zone *zCharger)
void redimensionneZoneAnalyseVideo(zone zPatientActuel, zone * zAnalyseVideo, zone *zNomVideo, zone *zChargerVideo, zone *zVideoOrigine, zone *zAnalyser, zone *zAnalyseEnCours)
{
	//contenant

	zAnalyseVideo->longueur = largeurFenetre()/2 - zAnalyseVideo->espaceGauche - zAnalyseVideo->espaceDroite ;
	zAnalyseVideo->xmin = largeurFenetre()/2 + zAnalyseVideo->espaceGauche;
	zAnalyseVideo->xmax = zAnalyseVideo->xmin + zAnalyseVideo->longueur;
	zAnalyseVideo->ymax = zPatientActuel.ymin - zAnalyseVideo->espaceHaut;
	zAnalyseVideo->ymin = zAnalyseVideo->espaceBas ;

	// zones interieures

	zNomVideo->xmin = zAnalyseVideo->xmin;
	zNomVideo->xmax = zAnalyseVideo->xmin + zNomVideo->longueur;
	zNomVideo->ymax = zAnalyseVideo->ymax - zAnalyseVideo->hauteur - zNomVideo->espaceHaut;
	zNomVideo->ymin = zNomVideo->ymax - zNomVideo->hauteur;

	zChargerVideo->longueur = tailleChaine(zChargerVideo->texte,zChargerVideo->hauteur);
	zChargerVideo->xmin = zNomVideo->xmax + zChargerVideo->espaceGauche;
	zChargerVideo->xmax = zChargerVideo->xmin + zChargerVideo->longueur;
	zChargerVideo->ymax = zNomVideo->ymax;
	zChargerVideo->ymin = zNomVideo->ymin;

	zVideoOrigine->xmin = zAnalyseVideo->xmin;
	zVideoOrigine->xmax = zAnalyseVideo->xmin + zVideoOrigine->longueur;
	zVideoOrigine->ymax = zNomVideo->ymin - zVideoOrigine->espaceHaut;
	zVideoOrigine->ymin = zVideoOrigine->ymax - zVideoOrigine->hauteur;

	zAnalyser->longueur = tailleChaine(zAnalyser->texte,zAnalyser->hauteur);
	zAnalyser->xmin =  zAnalyseVideo->xmin;
	zAnalyser->xmax = zAnalyseVideo->xmin + zAnalyser->longueur;
	zAnalyser->ymax = zVideoOrigine->ymin - zAnalyser->espaceHaut;
	zAnalyser->ymin = zAnalyser->ymax - zAnalyser->hauteur;

	zAnalyseEnCours->xmin = zAnalyseVideo->xmin;
	zAnalyseEnCours->xmax = zAnalyseVideo->xmin + zAnalyseEnCours->longueur;
	zAnalyseEnCours->ymax = zAnalyser->ymin - zAnalyseEnCours->espaceHaut;
	zAnalyseEnCours->ymin = zAnalyseEnCours->ymax - zAnalyseEnCours->hauteur;
}


// recupere la video actuelle et l'enregistre dans la variable
void gestionVideoActuelle(zone *zVideoOrigine, char **nomVideo)
{
	if( *nomVideo != NULL)
	{
		// Rien si on veut une taille fixe pour le nom de la video centre
		//zVideoOrigine->longueur = tailleChaine(*nomVideo, zVideoOrigine->hauteur);
		//zVideoOrigine->longueur = tailleChaine(*nomVideo, zVideoOrigine->hauteur);
	}
}

void initZonesAnalysesPrecedentes(zone zPatientActuel, zone * zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses)
{
	int i =0;
	float coeffEspace = 2.5;
	int contourRectangle = 10;
	
	
	zAnalysesPrecedentes->espaceHaut = 50;
	zAnalysesPrecedentes->espaceBas = 25;
	zAnalysesPrecedentes->espaceGauche = 50;
	zAnalysesPrecedentes->espaceDroite = 50;
	zAnalysesPrecedentes->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zAnalysesPrecedentes,"Analyses precedentes :");
	
	zAnalysesPrecedentes->longueur = largeurFenetre()/2 - zAnalysesPrecedentes->espaceGauche - zAnalysesPrecedentes->espaceDroite ;
	zAnalysesPrecedentes->xmin = zAnalysesPrecedentes->espaceGauche;
	zAnalysesPrecedentes->xmax = zAnalysesPrecedentes->xmin + zAnalysesPrecedentes->longueur;
	zAnalysesPrecedentes->ymax = zPatientActuel.ymin - zAnalysesPrecedentes->espaceHaut;
	zAnalysesPrecedentes->ymin = zAnalysesPrecedentes->espaceBas ;
	
	for (i=0; i<nbAnalyses;i++)
	{
		int iymax = zAnalysesPrecedentes->ymax - (coeffEspace *(i+1)*HAUTEUR_TEXTE);
		zBoutonsAnalysesPrecedentes[i].xmin = zAnalysesPrecedentes->xmin;
		zBoutonsAnalysesPrecedentes[i].xmax = zAnalysesPrecedentes->xmin + tailleChaine(analysesPrecedentes[i].dateHeure, HAUTEUR_TEXTE) + 20;
		zBoutonsAnalysesPrecedentes[i].ymin = iymax - HAUTEUR_TEXTE - contourRectangle;
		zBoutonsAnalysesPrecedentes[i].ymax = iymax + contourRectangle;
	}
	
}

void afficheAnalysesPrecedentes(zone zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses)
{
	int i = 0;
	couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	afficheChaine(zAnalysesPrecedentes.texte, zAnalysesPrecedentes.hauteur, zAnalysesPrecedentes.xmin,zAnalysesPrecedentes.ymax - zAnalysesPrecedentes.hauteur);
	float coeffEspace = 2.5;
	int contourRectangle = 10;
	
	if(nbAnalyses > 0 && analysesPrecedentes != NULL)
		for(i=0; i< nbAnalyses; i++)
		{
			//int iymax = zAnalysesPrecedentes.ymax - (coeffEspace *(i+1)*HAUTEUR_TEXTE);
			couleurCourante(26,71,255);
			// On calcule les ymax pour chaque i, le ymin est juste le ymax - la HAUTEUR_TEXTE. On fait -3 et +3 pour avoir un rectangle jaune + grand que le texte
			//rectangle(zAnalysesPrecedentes.xmin, iymax - HAUTEUR_TEXTE - contourRectangle, zAnalysesPrecedentes.xmin + tailleChaine(analysesPrecedentes[i].dateHeure, HAUTEUR_TEXTE) + 20, iymax + contourRectangle);
			rectangle(zBoutonsAnalysesPrecedentes[i].xmin, zBoutonsAnalysesPrecedentes[i].ymin, zBoutonsAnalysesPrecedentes[i].xmax, zBoutonsAnalysesPrecedentes[i].ymax);
			couleurCourante(255,255,255);
			//afficheChaine(analysesPrecedentes[i].dateHeure, HAUTEUR_TEXTE, zAnalysesPrecedentes.xmin+10, zAnalysesPrecedentes.ymax - zAnalysesPrecedentes.espaceBas -(i+2)*(HAUTEUR_TEXTE+5));
			afficheChaine(analysesPrecedentes[i].dateHeure, HAUTEUR_TEXTE, zBoutonsAnalysesPrecedentes[i].xmin+10, zBoutonsAnalysesPrecedentes[i].ymin + contourRectangle);
		}
}

void redimensionneZoneAnalysesPrecedentes(zone zPatientActuel, zone * zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses)
{
	int i = 0;
	float coeffEspace = 2.5;
	int contourRectangle = 10;
	zAnalysesPrecedentes->longueur = largeurFenetre()/2 - zAnalysesPrecedentes->espaceGauche - zAnalysesPrecedentes->espaceDroite ;
	zAnalysesPrecedentes->xmin = zAnalysesPrecedentes->espaceGauche;
	zAnalysesPrecedentes->xmax = zAnalysesPrecedentes->xmin + zAnalysesPrecedentes->longueur;
	zAnalysesPrecedentes->ymax = zPatientActuel.ymin - zAnalysesPrecedentes->espaceHaut;
	zAnalysesPrecedentes->ymin = zAnalysesPrecedentes->espaceBas ;
	
	for (i=0; i<nbAnalyses;i++)
	{
		int iymax = zAnalysesPrecedentes->ymax - (coeffEspace *(i+1)*HAUTEUR_TEXTE);
		zBoutonsAnalysesPrecedentes[i].xmin = zAnalysesPrecedentes->xmin;
		zBoutonsAnalysesPrecedentes[i].xmax = zAnalysesPrecedentes->xmin + tailleChaine(analysesPrecedentes[i].dateHeure, HAUTEUR_TEXTE) + 20;
		zBoutonsAnalysesPrecedentes[i].ymin = iymax - HAUTEUR_TEXTE - contourRectangle;
		zBoutonsAnalysesPrecedentes[i].ymax = iymax + contourRectangle;
	}
}


void initZonesAnalyse(zone zPatientActuel, zone *zAnalyse, zone *zVideoSquelette, zone *zGraph)
{
    // contenant
	zAnalyse->espaceHaut = 50;
	zAnalyse->espaceBas = 15;
	zAnalyse->espaceGauche = 15;
	zAnalyse->espaceDroite = 15;
	zAnalyse->hauteur = 281; // hauteur image
	//changeTexteZone(zAnalyse,"Charger video : ");
	zAnalyse->longueur = largeurFenetre() - zAnalyse->espaceGauche - zAnalyse->espaceDroite ;
	zAnalyse->xmin = zAnalyse->espaceGauche;
	zAnalyse->xmax = zAnalyse->xmin + zAnalyse->longueur;
	zAnalyse->ymax = zPatientActuel.ymin - zAnalyse->espaceHaut;
	zAnalyse->ymin = zAnalyse->ymax - zAnalyse->hauteur - zAnalyse->espaceBas ;

	// zones interieures

	zVideoSquelette->hauteur = 281;
	zVideoSquelette->espaceGauche = 0;
	zVideoSquelette->espaceDroite = 10;
	zVideoSquelette->espaceBas = 10;
	zVideoSquelette->espaceHaut = 0;
	zVideoSquelette->longueur = 500;
	zVideoSquelette->xmin = zAnalyse->xmin + zVideoSquelette->espaceGauche;
	zVideoSquelette->xmax = zVideoSquelette->xmin + zVideoSquelette->longueur;
	zVideoSquelette->ymax = zAnalyse->ymax - zVideoSquelette->espaceHaut;
	zVideoSquelette->ymin = zVideoSquelette->ymax - zVideoSquelette->hauteur;
	
	zGraph->hauteur = 281; // img dim
	zGraph->longueur = 500; // img dim
	zGraph->espaceGauche = 20;
	zGraph->espaceDroite = 10;
	zGraph->espaceBas = 10;
	zGraph->espaceHaut = 0;
	//zGraph->xmin = zVideoSquelette->xmax + zVideoSquelette->espaceDroite + zGraph->espaceGauche;
	zGraph->xmin = largeurFenetre() / 2 + zGraph->espaceGauche;
	zGraph->xmax = zGraph->xmin + zGraph->longueur;
	zGraph->ymax = zAnalyse->ymax - zGraph->espaceHaut;
	zGraph->ymin = zGraph->ymax - zGraph->hauteur;
}

void afficheAnalyse(zone zAnalyse, zone zVideoSquelette, DonneesImageRGB* videoSquelette, zone zGraph, DonneesImageRGB* videoGraph)
{
    couleurCourante(0,0,0);
	epaisseurDeTrait(2);

	if (videoSquelette != NULL)
	{
		afficheImage(zVideoSquelette, videoSquelette);
	}
	
	if (videoGraph != NULL)
	{
		afficheImage(zGraph, videoGraph);
	}

}
void redimensionneZoneAnalyse(zone zPatientActuel, zone *zAnalyse, zone *zVideoSquelette, zone *zGraph)
{
    // contenant
	zAnalyse->longueur = largeurFenetre() - zAnalyse->espaceGauche - zAnalyse->espaceDroite ;
	zAnalyse->xmin = zAnalyse->espaceGauche;
	zAnalyse->xmax = zAnalyse->xmin + zAnalyse->longueur;
	zAnalyse->ymax = zPatientActuel.ymin - zAnalyse->espaceHaut;
	zAnalyse->ymin = zAnalyse->ymax - zAnalyse->hauteur - zAnalyse->espaceBas ;

	// zones interieures

	zVideoSquelette->xmin = zAnalyse->xmin + zVideoSquelette->espaceGauche;
	zVideoSquelette->xmax = zVideoSquelette->xmin + zVideoSquelette->longueur;
	zVideoSquelette->ymax = zAnalyse->ymax - zVideoSquelette->espaceHaut;
	zVideoSquelette->ymin = zVideoSquelette->ymax - zVideoSquelette->hauteur;

	//zGraph->xmin = zVideoSquelette->xmax + zVideoSquelette->espaceDroite + zGraph->espaceGauche;
	zGraph->xmin = largeurFenetre() / 2 + zGraph->espaceGauche;
	zGraph->xmax = zGraph->xmin + zGraph->longueur;
	zGraph->ymax = zAnalyse->ymax - zGraph->espaceHaut;
	zGraph->ymin = zGraph->ymax - zGraph->hauteur;
}

void initZonesDonneesBio(zone zAnalyse, zone *zDonneesBio, zone *zTailleAnalyse, zone *zPoidsAnalyse)
{
    // contenant
	zDonneesBio->espaceHaut = 15;
	zDonneesBio->espaceBas = 50;
	zDonneesBio->espaceGauche = 50;
	zDonneesBio->espaceDroite = 50;
	zDonneesBio->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zDonneesBio,"Donnees biometriques");
	zDonneesBio->longueur = largeurFenetre()/2 - zDonneesBio->espaceGauche - zDonneesBio->espaceDroite ;
	zDonneesBio->xmin = zDonneesBio->espaceGauche;
	zDonneesBio->xmax = zDonneesBio->xmin + zDonneesBio->longueur;
	zDonneesBio->ymax = zAnalyse.ymin - zDonneesBio->espaceHaut;
	zDonneesBio->ymin = zDonneesBio->espaceBas ;

	// zones interieures

	zTailleAnalyse->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zTailleAnalyse,"Taille : ");
	zTailleAnalyse->espaceGauche = 0;
	zTailleAnalyse->espaceDroite = 10;
	zTailleAnalyse->espaceBas = 10;
	zTailleAnalyse->espaceHaut = 25;
	zTailleAnalyse->longueur = tailleChaine(zTailleAnalyse->texte,zTailleAnalyse->hauteur);
	zTailleAnalyse->xmin = zDonneesBio->xmin + zTailleAnalyse->espaceGauche;
	zTailleAnalyse->xmax = zTailleAnalyse->xmin + zTailleAnalyse->longueur;
	zTailleAnalyse->ymax = zDonneesBio->ymax - zDonneesBio->hauteur - zTailleAnalyse->espaceHaut;
	zTailleAnalyse->ymin = zTailleAnalyse->ymax - zTailleAnalyse->hauteur;

	zPoidsAnalyse->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPoidsAnalyse,"Poids : ");
	zPoidsAnalyse->espaceGauche = 0;
	zPoidsAnalyse->espaceDroite = 10;
	zPoidsAnalyse->espaceBas = 10;
	zPoidsAnalyse->espaceHaut = 10;
	zPoidsAnalyse->longueur = tailleChaine(zPoidsAnalyse->texte,zPoidsAnalyse->hauteur);
	zPoidsAnalyse->xmin = zDonneesBio->xmin + zPoidsAnalyse->espaceGauche;
	zPoidsAnalyse->xmax = zPoidsAnalyse->xmin + zPoidsAnalyse->longueur;
	zPoidsAnalyse->ymax = zTailleAnalyse->ymin - zPoidsAnalyse->espaceHaut;
	zPoidsAnalyse->ymin = zPoidsAnalyse->ymax - zPoidsAnalyse->hauteur;
}

void afficheDonneesBio(zone zDonneesBio, zone zTailleAnalyse, char* taille, zone zPoidsAnalyse, char* poids)
{
	
    couleurCourante(0,0,0);
	epaisseurDeTrait(2);
	
	afficheChaine(zDonneesBio.texte, zDonneesBio.hauteur, zDonneesBio.xmin,zDonneesBio.ymax - zDonneesBio.hauteur);

	char tailleBase[32] = "Taille : ";
    char poidsBase[32] = "Poids : ";
    
    

	if(taille != NULL)
    {
        strcat(tailleBase, taille);
        afficheChaine(tailleBase,zTailleAnalyse.hauteur, zTailleAnalyse.xmin+5,zTailleAnalyse.ymin+5);
    }

    if(poids != NULL)
    {
        strcat(poidsBase, poids);
        afficheChaine(poidsBase,zPoidsAnalyse.hauteur, zPoidsAnalyse.xmin+5,zPoidsAnalyse.ymin+5);
    }
    // etc si d'autres donnees
}

void redimensionneZoneDonneesBio(zone zAnalyse, zone *zDonneesBio, zone *zTailleAnalyse, zone *zPoidsAnalyse)
{
    // contenant
	zDonneesBio->longueur = largeurFenetre()/2 - zDonneesBio->espaceGauche - zDonneesBio->espaceDroite ;
	zDonneesBio->xmin = zDonneesBio->espaceGauche;
	zDonneesBio->xmax = zDonneesBio->xmin + zDonneesBio->longueur;
	zDonneesBio->ymax = zAnalyse.ymin - zDonneesBio->espaceHaut;
	zDonneesBio->ymin = zDonneesBio->espaceBas ;

	// zones interieures

	zTailleAnalyse->longueur = tailleChaine(zTailleAnalyse->texte,zTailleAnalyse->hauteur);
	zTailleAnalyse->xmin = zDonneesBio->xmin + zTailleAnalyse->espaceGauche;
	zTailleAnalyse->xmax = zTailleAnalyse->xmin + zTailleAnalyse->longueur;
	zTailleAnalyse->ymax = zDonneesBio->ymax - zDonneesBio->hauteur - zTailleAnalyse->espaceHaut;
	zTailleAnalyse->ymin = zTailleAnalyse->ymax - zTailleAnalyse->hauteur;

	zPoidsAnalyse->longueur = tailleChaine(zPoidsAnalyse->texte,zPoidsAnalyse->hauteur);
	zPoidsAnalyse->xmin = zDonneesBio->xmin + zPoidsAnalyse->espaceGauche;
	zPoidsAnalyse->xmax = zPoidsAnalyse->xmin + zPoidsAnalyse->longueur;
	zPoidsAnalyse->ymax = zTailleAnalyse->ymin - zPoidsAnalyse->espaceHaut;
	zPoidsAnalyse->ymin = zPoidsAnalyse->ymax - zPoidsAnalyse->hauteur;
}

void initZonesPathologies(zone zDonneesBio, zone *zPathologies, zone *zCourbe, zone *zBoite, zone* zMarcheReguliere)
{
    // contenant
	zPathologies->espaceHaut = 15;
	zPathologies->espaceBas = 50;
	zPathologies->espaceGauche = 50;
	zPathologies->espaceDroite = 50;
	zPathologies->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zPathologies,"Pathologies");
	zPathologies->longueur = largeurFenetre()/2 - zPathologies->espaceGauche - zPathologies->espaceDroite ;
	zPathologies->xmin = zDonneesBio.xmax + zDonneesBio.espaceDroite + zPathologies->espaceGauche;
	zPathologies->xmax = zPathologies->xmin + zPathologies->longueur;
	zPathologies->ymax = zDonneesBio.ymax;
	zPathologies->ymin = zPathologies->espaceBas ;

	// zones interieures

	zCourbe->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zCourbe,"Courbe : ");
	zCourbe->espaceGauche = 0;
	zCourbe->espaceDroite = 10;
	zCourbe->espaceBas = 10;
	zCourbe->espaceHaut = 25;
	zCourbe->longueur = tailleChaine(zCourbe->texte,zCourbe->hauteur);
	zCourbe->xmin = zPathologies->xmin + zCourbe->espaceGauche;
	zCourbe->xmax = zCourbe->xmin + zCourbe->longueur;
	zCourbe->ymax = zPathologies->ymax - zPathologies->hauteur - zCourbe->espaceHaut;
	zCourbe->ymin = zCourbe->ymax - zCourbe->hauteur;

	zBoite->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zBoite,"Boite : ");
	zBoite->espaceGauche = 0;
	zBoite->espaceDroite = 10;
	zBoite->espaceBas = 10;
	zBoite->espaceHaut = 10;
	zBoite->longueur = tailleChaine(zBoite->texte,zBoite->hauteur);
	zBoite->xmin = zPathologies->xmin + zBoite->espaceGauche;
	zBoite->xmax = zBoite->xmin + zBoite->longueur;
	zBoite->ymax = zCourbe->ymin - zBoite->espaceHaut;
	zBoite->ymin = zBoite->ymax - zBoite->hauteur;

	zMarcheReguliere->hauteur = HAUTEUR_TEXTE;
	changeTexteZone(zMarcheReguliere,"Marche reguliere : ");
	zMarcheReguliere->espaceGauche = 0;
	zMarcheReguliere->espaceDroite = 10;
	zMarcheReguliere->espaceBas = 10;
	zMarcheReguliere->espaceHaut = 10;
	zMarcheReguliere->longueur = tailleChaine(zMarcheReguliere->texte,zMarcheReguliere->hauteur);
	zMarcheReguliere->xmin = zPathologies->xmin + zMarcheReguliere->espaceGauche;
	zMarcheReguliere->xmax = zMarcheReguliere->xmin + zMarcheReguliere->longueur;
	zMarcheReguliere->ymax = zBoite->ymin - zMarcheReguliere->espaceHaut;
	zMarcheReguliere->ymin = zMarcheReguliere->ymax - zMarcheReguliere->hauteur;
}

void affichePathologies(zone zPathologies, zone zCourbe, char* courbe, zone zBoite, char* boite, zone zMarcheReguliere, char* marcheRegu)
{
    couleurCourante(0,0,0);
	epaisseurDeTrait(2);

	afficheChaine(zPathologies.texte, zPathologies.hauteur, zPathologies.xmin,zPathologies.ymax - zPathologies.hauteur);

    char courbeBase[32] = "Courbe : ";
    char boiteBase[32] = "Boite : ";
    char marcheReguBase[32] = "Marche reguliere : ";


	if(courbe != NULL)
	{
        strcat(courbeBase, courbe);
		afficheChaine(courbeBase,zCourbe.hauteur, zCourbe.xmin+5,zCourbe.ymin+5);
	}

    if(boite != NULL)
	{
        strcat(boiteBase, boite);
		afficheChaine(boiteBase,zBoite.hauteur, zBoite.xmin+5,zBoite.ymin+5);
	}

	if(marcheRegu != NULL)
	{
        strcat(marcheReguBase, marcheRegu);
		afficheChaine(marcheReguBase,zMarcheReguliere.hauteur, zMarcheReguliere.xmin+5,zMarcheReguliere.ymin+5);
	}

    // etc si d'autres donnees
}
void redimensionneZonePathologies(zone zDonneesBio, zone *zPathologies, zone *zCourbe, zone *zBoite, zone* zMarcheReguliere)
{
     // contenant
	zPathologies->longueur = largeurFenetre()/2 - zPathologies->espaceGauche - zPathologies->espaceDroite ;
	zPathologies->xmin = zDonneesBio.xmax + zDonneesBio.espaceDroite + zPathologies->espaceGauche;
	zPathologies->xmax = zPathologies->xmin + zPathologies->longueur;
	zPathologies->ymax = zDonneesBio.ymax;
	zPathologies->ymin = zPathologies->espaceBas ;

	// zones interieures

	zCourbe->longueur = tailleChaine(zCourbe->texte,zCourbe->hauteur);
	zCourbe->xmin = zPathologies->xmin + zCourbe->espaceGauche;
	zCourbe->xmax = zCourbe->xmin + zCourbe->longueur;
	zCourbe->ymax = zPathologies->ymax - zPathologies->hauteur - zCourbe->espaceHaut;
	zCourbe->ymin = zCourbe->ymax - zCourbe->hauteur;

	zBoite->longueur = tailleChaine(zBoite->texte,zBoite->hauteur);
	zBoite->xmin = zPathologies->xmin + zBoite->espaceGauche;
	zBoite->xmax = zBoite->xmin + zBoite->longueur;
	zBoite->ymax = zCourbe->ymin - zBoite->espaceHaut;
	zBoite->ymin = zBoite->ymax - zBoite->hauteur;

	zMarcheReguliere->longueur = tailleChaine(zMarcheReguliere->texte,zMarcheReguliere->hauteur);
	zMarcheReguliere->xmin = zPathologies->xmin + zMarcheReguliere->espaceGauche;
	zMarcheReguliere->xmax = zMarcheReguliere->xmin + zMarcheReguliere->longueur;
	zMarcheReguliere->ymax = zBoite->ymin - zMarcheReguliere->espaceHaut;
	zMarcheReguliere->ymin = zMarcheReguliere->ymax - zMarcheReguliere->hauteur;
}

void initZonesFichePatient(zone zPatientActuel, zone *zFichePatient, zone *zFicheNom, zone *zFichePrenom, zone *zFicheTaille, zone *zFichePoids, zone *zFicheCourbe, zone *zFicheBoite, zone *zFicheMarcheRegu, zone *zFicheVideos)
{

}
void afficheFichePatient(zone zFichePatient, zone zFicheNom, char* nom, zone zFichePrenom, char* prenom, zone zFicheTaille, char* taille, zone zFichePoids, char* poids,
                         zone zFicheCourbe, char* courbe, zone zFicheBoite, char* boite, zone zFicheMarcheRegu, char* marcheRegu, zone zFicheVideos, char** videos)
{

}
void redimensionneZoneFichePatient(zone zPatientActuel, zone *zFichePatient, zone *zFicheNom, zone *zFichePrenom, zone *zFicheTaille, zone *zFichePoids, zone *zFicheCourbe, zone *zFicheBoite, zone *zFicheMarcheRegu, zone *zFicheVideos)
{

}
