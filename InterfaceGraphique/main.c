#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>		// Pour pouvoir utiliser les librairies mathematiques
#include <complex.h>
#include <unistd.h>
#include <time.h>

#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
#include "ESLib.h"	
#include "OutilsLib.h"	
#include "BmpLib.h"


#include "definitions.h"
#include "IHM.h"
#include "fonctionsTraitementImage.h"

static char titre [] = "Gait Analysis";

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
static int LargeurFenetre = 800;
static int HauteurFenetre = 600;

//sert à lier GfxLib.c et main.c pour modifier LargeurFenetre et HauteurFenetre  (redimensionnement)
extern int varLargeurFenetre;
extern int varHauteurFenetre;

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
   des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);


int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);

	prepareFenetreGraphique(titre, LargeurFenetre, HauteurFenetre);

	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
	   qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();

	return 0;
}


/* La fonction de gestion des evenements, appelee automatiquement par le systeme
   des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static int numPage = 1;

	static DonneesImageRGB *retour, *home, *croix, *logo; 
	static zone zQuit, zRetour, zHome, zTitre;

	static zone zPatientActuel, zPrenomPatientActuel, zNomPatientActuel, zVoirFiche;
	static char *prenom, *nom;
	static zone zChargerPatient,zPrenomChargerPatient,zNomChargerPatient,zCharger;
	static char *nouveau_prenom,*nouveau_nom;

	static time_t start_time, actual_time;

	switch (evenement)
	{
		case Initialisation:

			prenom = nom = nouveau_prenom = nouveau_nom = NULL;

			retour = lisBMPRGB("retour.bmp");
			home = lisBMPRGB("home.bmp");
			croix = lisBMPRGB("croix.bmp");
			logo = lisBMPRGB("logo.bmp");

			if(retour == NULL || home == NULL || croix == NULL || logo == NULL)
			{
				perror("problem reading images");
				libereDonneesImageRGB(&retour); libereDonneesImageRGB(&home); libereDonneesImageRGB(&croix); libereDonneesImageRGB(&logo);
				exit(EXIT_FAILURE);
			}

			demandeTemporisation(20);
			initZones(&zQuit,&zHome,&zRetour,retour,home,croix);
			initZoneTitre(&zTitre,titre);
			initZonesPatientActuel(zTitre,&zPatientActuel,&zPrenomPatientActuel,&zNomPatientActuel, &zVoirFiche);
			initZonesChargerPatient(zPatientActuel,&zChargerPatient,&zPrenomChargerPatient,&zNomChargerPatient,&zCharger);

			start_time = time(NULL); // gets system time
			break;

		case Temporisation:
			switch(numPage)
			{
				case 1: // page d'acceuil avec delai
					actual_time = time(NULL);
					if(difftime(actual_time, start_time) > DELAY_SCREEN)
						numPage = 2;
					break;

				case 2: // page choix patient
					changeTexteZone(&zTitre,"Choose patient");
					gestionNomPrenomPatient(&zPrenomPatientActuel, &zNomPatientActuel,&prenom,&nom);
					break;

					case 21: // page fiche patient
						changeTexteZone(&zTitre,"Fiche patient");
						break;

				default:
					break;
			}
			changeZoneTitre(&zTitre,numPage);
			redimensionneFenetre(largeurFenetre(),hauteurFenetre());

			// A chaque mise a jour il faut redessiner la fenetre :
			rafraichisFenetre();
			break;

		case Affichage:
			// On part d'un fond d'ecran blanc
			effaceFenetre (255, 255, 255);

			switch(numPage)
			{
				case 1: // page delai accueil
					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo, numPage);
					afficheAcceuil(zTitre);
					break;
				
				case 2: // page choix patient
					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre,3);
					affichePatientActuel(zPatientActuel,zPrenomPatientActuel,prenom, zNomPatientActuel,nom,zVoirFiche);
					afficheChargerPatient(zChargerPatient,zPrenomChargerPatient,nouveau_prenom,zNomChargerPatient,nouveau_nom,zCharger);

					break;
					
					case 21: // page fiche patient
						monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
						afficheTitre(zTitre,3);
						break;

			}
			break;

		case Clavier:
			if(zPrenomChargerPatient.saisie)
			{
				recupereTexte(&nouveau_prenom);
			}
			else{
				switch (caractereClavier())
				{
					case 'q':
					case 'Q':
						termineBoucleEvenements();
						break;
				}
			}
			rafraichisFenetre();
			
			break;

		case ClavierSpecial:
			break;

		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye) 
			{
				if (zQuit.xmin<abscisseSouris() && abscisseSouris()<zQuit.xmax && zQuit.ymin<ordonneeSouris() && ordonneeSouris()<zQuit.ymax)
					termineBoucleEvenements();

				//bouton HOME
				else if ((numPage>2) && (zHome.xmin<abscisseSouris() && abscisseSouris()<zHome.xmax && zHome.ymin<ordonneeSouris() && ordonneeSouris()<zHome.ymax))
				{
					arreteSaisiesTexte(&zPrenomChargerPatient, &zNomChargerPatient);
					numPage=2;  //reviens à la page d'accueil
				}

				//bouton RETOUR 				
				else if ((numPage>2) && (zRetour.xmin<abscisseSouris() && abscisseSouris()<zRetour.xmax && zRetour.ymin<ordonneeSouris() && ordonneeSouris()<zRetour.ymax))
				{
					arreteSaisiesTexte(&zPrenomChargerPatient, &zNomChargerPatient);
					if(numPage > 2) 
						numPage=2;
				}
				
				// bouton voir fiche patient
				else if(prenom != NULL && nom != NULL && numPage >=2 && zVoirFiche.xmin<abscisseSouris() && abscisseSouris()<zVoirFiche.xmax && zVoirFiche.ymin<ordonneeSouris() && ordonneeSouris()<zVoirFiche.ymax)
				{
					arreteSaisiesTexte(&zPrenomChargerPatient, &zNomChargerPatient);
					numPage = 21;
				}

				else if(numPage == 2 && zPrenomChargerPatient.xmin<abscisseSouris() && abscisseSouris()<zPrenomChargerPatient.xmax && zPrenomChargerPatient.ymin<ordonneeSouris() && ordonneeSouris()<zPrenomChargerPatient.ymax)
				{
					// désactive  toutes les zones de saisie et active celle en question
					arreteSaisiesTexte(&zPrenomChargerPatient, &zNomChargerPatient);
					zPrenomChargerPatient.saisie = true;
				}
				else // clic sur toute autre zone de l'ihm
				{
					arreteSaisiesTexte(&zPrenomChargerPatient, &zNomChargerPatient);
				}
				

				rafraichisFenetre();
			}
			break;

		case Souris: // Si la souris est deplacee
			break;

		case Inactivite: // Quand aucun message n'est disponible
			break;

		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			// Donc le systeme nous en informe
			redimensionneZones(&zQuit,&zHome,&zRetour,retour,home,croix);
			redimensionneZoneTitre(&zTitre);
			switch(numPage)
			{
				case 2:		
					redimensionneZonePatientActuel(zTitre, &zPatientActuel, &zVoirFiche, &zPrenomPatientActuel, &zNomPatientActuel);
					redimensionneZoneChargerPatient(zPatientActuel,&zChargerPatient,&zPrenomChargerPatient,&zNomChargerPatient,&zCharger);
					break;
				default:
					break;
			}
			


			rafraichisFenetre();
			break;
	}
}
