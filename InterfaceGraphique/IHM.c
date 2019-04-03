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

	if(prenom != NULL && nom != NULL)
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
	caractere = caractereClavier();
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

void arreteSaisiesTexte(zone *zSaisie1, zone *zSaisie2)
{
	zSaisie1->saisie = zSaisie2->saisie = false;
}