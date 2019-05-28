#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>		// Pour pouvoir utiliser les librairies mathematiques
#include <complex.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include "donneesPatients.h"

	
extern "C"
{
	#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
	#include "ESLib.h"	
	#include "OutilsLib.h"	
	#include "BmpLib.h"

	#include "IHM.h"
	#include "fonctionsTraitementImage.h"
	
}

static char titre [] = "Gait Analysis";

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
static int LargeurFenetre = 1050;
static int HauteurFenetre = 700;

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

	static DonneesImageRGB *retour, *home, *croix, *logo, *fleche;
	static zone zQuit, zRetour, zHome, zTitre;

	static zone zPatientActuel, zPrenomPatientActuel, zNomPatientActuel, zVoirFiche;
	static char *prenom, *nom;
	static zone zChargerPatient,zPrenomChargerPatient,zNomChargerPatient,zCharger;
	static char *prenom_a_charger,*nom_a_charger;
	static zone zNouveauPatient,zPrenomNouveauPatient,zNomNouveauPatient, zTailleNouveauPatient, zPoidsNouveauPatient, zCreerPatient;
	static char *nouveau_prenom,*nouveau_nom;
	//static int taille_nouveau_patient, poids_nouveau_patient;
	static char *chaine_taille_nouveau_patient, *chaine_poids_nouveau_patient;

	static zone zChargerVideo, zVideoOrigine, zAnalyser, zAnalyseEnCours, zAnalyseVideo, zNomVideo, zAnalysesPrecedentes;
	static zone* zBoutonsAnalysesPrecedentes;
	static char *cheminVideoActuelle, *nomImageVideo;
	static DonneesImageRGB *imageVideo, *imageVideoCourbes, *imageVideoSquelettes;
	static bool enCours, enCoursAffichage;
	static int compteurImageAlpha;
	static int nbImageAlpha, lecture;
	static struct dirent** listeImageAlpha;
	static zone zAnalyse, zVideoSquelette, zGraph, zDonneesBio, zTailleAnalyse, zPoidsAnalyse,zlongueurBrasAnalyse,zlongueurJambeAnalyse ,zPathologies, zCourbe, zBoite,ZMarcheNormale;
	static analyse* analyses = NULL;
	static int nbAnalyses = 0;
	static char *poids, *taille, *longueurBras,*longueurJambe, *courbe, *boite, *marcheNorm;
	static char date[32];
	static bool resetPageAnalyse;
	static int compteurImageAlphaCourbes, compteurImageAlphaSquelettes;
	static int nbImageAlphaCourbes, lectureCourbes, nbImageAlphaSquelettes, lectureSquelettes;
	static struct dirent** listeImageAlphaCourbes, **listeImageAlphaSquelettes;
	static int rapiditeAffichage = 3, compteurRapiditeAffichage = 0;
	
	static char* modifPrenom, *modifNom, *modifTaille, *modifPoids;
	static bool resetPageFichePatient;
	static int numPagePrecedente;
	static zone zFichePatient, zFicheNom, zFichePrenom, zFicheTaille, zFichePoids, zFicheMaj;

	static time_t start_time, actual_time;

	switch (evenement)
	{
		case Initialisation:

			prenom = nom = prenom_a_charger = nom_a_charger = poids = taille = longueurBras = longueurJambe = NULL;
			longueurBras = longueurJambe = NULL;
			nouveau_prenom = nouveau_nom = chaine_taille_nouveau_patient = chaine_poids_nouveau_patient = NULL;
			compteurImageAlpha = 0;
			lecture = 0;
			compteurImageAlphaSquelettes = 0;
			lectureCourbes = 0;
			compteurImageAlphaCourbes = 0;
			lectureSquelettes = 0;
			resetPageAnalyse = true;
			resetPageFichePatient = true;
			enCours = false;
			enCoursAffichage = false;
			nomImageVideo = (char *)malloc(sizeof(char) * 256);
			boite = (char*)malloc(sizeof(char) * 8);
			marcheNorm = (char*)malloc(sizeof(char) * 8);
			longueurBras = (char*)malloc(sizeof(char) * 8);
			longueurJambe = (char*)malloc(sizeof(char) * 8);
			
			retour = lisBMPRGB("retour.bmp");
			home = lisBMPRGB("home.bmp");
			croix = lisBMPRGB("croix.bmp");
			logo = lisBMPRGB("logo.bmp");
			fleche = lisBMPRGB("fleche.bmp");

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
			initZonesNouveauPatient(zChargerPatient, &zNouveauPatient, &zPrenomNouveauPatient, &zNomNouveauPatient, &zTailleNouveauPatient, &zPoidsNouveauPatient, &zCreerPatient);

			initZonesAnalyseVideo(zPatientActuel, &zAnalyseVideo, &zNomVideo, &zChargerVideo, &zVideoOrigine, &zAnalyser, &zAnalyseEnCours);
			//initZonesAnalysesPrecedentes(zPatientActuel, &zAnalysesPrecedentes);

            initZonesAnalyse(zPatientActuel, &zAnalyse, &zVideoSquelette, &zGraph);
            initZonesDonneesBio(zAnalyse, &zDonneesBio, &zTailleAnalyse, &zPoidsAnalyse, &zlongueurBrasAnalyse,&zlongueurJambeAnalyse);
            initZonesPathologies(zDonneesBio, &zPathologies, &zCourbe, &zBoite, &ZMarcheNormale);
            
            initZonesFichePatient(&zFichePatient, &zFicheNom, &zFichePrenom, &zFicheTaille, &zFichePoids, &zFicheMaj);

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
				case 3: // page analyse video
					changeTexteZone(&zTitre,"Video analysis");
					gestionNomPrenomPatient(&zPrenomPatientActuel, &zNomPatientActuel,&prenom,&nom);
					gestionVideoActuelle(&zNomVideo, &cheminVideoActuelle);
					char fullPath[256];
					if (enCoursAffichage)
					{
						sleep(2);
						lecture = 0;
						enCours = false;
						enCoursAffichage = false;
						resetPageAnalyse = true;
						numPage = 4;
						//evaluePathologies(cheminVideoActuelle, courbe, marcheNorm, boite);
						creeAnalysePatient(nom, prenom, taille, poids,&longueurBras,&longueurJambe, boite, marcheNorm, cheminVideoActuelle, nbImageAlpha, date);
					}
					else if(cheminVideoActuelle != NULL && strcmp(cheminVideoActuelle, "") != 0 && lecture != 0 && compteurImageAlpha < nbImageAlpha)
					{
						libereDonneesImageRGB(&imageVideo);
						sprintf(fullPath, "%s/%s", CHEMIN_DOSSIER_VIDEO, cheminVideoActuelle);
						//printf("\nLecture de %s\n", fullPath);
						imageVideo = lisImageCouranteAlphabetique(listeImageAlpha[compteurImageAlpha], fullPath, &nomImageVideo); //listeImageAlpha[compteurImageAlpha] l'image que l'on veut lire
						if(imageVideo == NULL)
						{
							perror("erreur lecture image video");
							libereDonneesImageRGB(&imageVideo);
							compteurImageAlpha++;
						}
						else // image bien lue donc on peut detecter les lignes
						{
							compteurImageAlpha++;
						}
					}
					if(cheminVideoActuelle != NULL && strcmp(cheminVideoActuelle, "") != 0 && lecture != 0 && compteurImageAlpha == nbImageAlpha)
					{
						compteurImageAlpha = 0;
					}
					break;
                case 4: // page analyse
					changeTexteZone(&zTitre,"Analysis");
					gestionNomPrenomPatient(&zPrenomPatientActuel, &zNomPatientActuel,&prenom,&nom);
					char fullPathCourbes[256];
					char fullPathSquelettes[256];
					sprintf(fullPathCourbes, "%s/%s", CHEMIN_DOSSIER_COURBES, date);
					sprintf(fullPathSquelettes, "%s/%s", CHEMIN_DOSSIER_SQUELETTES, date);
					if (resetPageAnalyse) // On vient d'arriver sur la page, on declenche les fonctions pour la 1ere fois
					{
						resetPageAnalyse = false;
						compteurImageAlphaCourbes = 0;
						compteurImageAlphaSquelettes = 0;
						nbImageAlphaCourbes = scandir(fullPathCourbes, &listeImageAlphaCourbes, NULL, alphasort);
						nbImageAlphaSquelettes = scandir(fullPathSquelettes, &listeImageAlphaSquelettes, NULL, alphasort);
						printf("courbes : %d\n", nbImageAlphaCourbes);
						printf("squelettes : %d\n", nbImageAlphaSquelettes);
						if (nbImageAlphaCourbes == -1)
						{
							lectureCourbes = 0;
						}
						else
						{
							lectureCourbes = 1;
						}
						if (nbImageAlphaSquelettes == -1)
						{
							lectureSquelettes = 0;
						}
						else
						{
							lectureSquelettes = 1;
						}
					}
					if (compteurRapiditeAffichage < rapiditeAffichage)
					{
						compteurRapiditeAffichage++;
					}
					else
					{
						compteurRapiditeAffichage = 0;
						
						libereDonneesImageRGB(&imageVideoCourbes);
						imageVideoCourbes = lisImageCouranteAlphabetique(listeImageAlphaCourbes[compteurImageAlphaCourbes], fullPathCourbes, &nomImageVideo); //listeImageAlpha[compteurImageAlpha] l'image que l'on veut lire
						if(imageVideoCourbes == NULL)
						{
							perror("erreur lecture image video");
							libereDonneesImageRGB(&imageVideoCourbes);
							compteurImageAlphaCourbes++;
						}
						else // image bien lue donc on peut detecter les lignes
						{
							compteurImageAlphaCourbes++;
						}
						
						libereDonneesImageRGB(&imageVideoSquelettes);
						imageVideoSquelettes = lisImageCouranteAlphabetique(listeImageAlphaSquelettes[compteurImageAlphaSquelettes], fullPathSquelettes, &nomImageVideo); //listeImageAlpha[compteurImageAlpha] l'image que l'on veut lire
						if(imageVideoSquelettes == NULL)
						{
							perror("erreur lecture image video");
							libereDonneesImageRGB(&imageVideoSquelettes);
							compteurImageAlphaSquelettes++;
						}
						else // image bien lue donc on peut detecter les lignes
						{
							compteurImageAlphaSquelettes++;
						}
					}
					
					if(lectureCourbes != 0 && compteurImageAlphaCourbes == nbImageAlphaCourbes)
					{
						compteurImageAlphaCourbes = 0;
					}
					if(lectureSquelettes != 0 && compteurImageAlphaSquelettes == nbImageAlphaSquelettes)
					{
						compteurImageAlphaSquelettes = 0;
					}
					break;
				case 11: // page fiche patient
					changeTexteZone(&zTitre,"Patient record");
					if (resetPageFichePatient)
					{
						resetPageFichePatient = false;
						modifPrenom = (char *)malloc(sizeof(char) * 32);
						modifNom = (char *)malloc(sizeof(char) * 32);
						modifTaille = (char *)malloc(sizeof(char) * 32);
						modifPoids = (char *)malloc(sizeof(char) * 32);
						strcpy(modifPrenom, prenom);
						strcpy(modifNom, nom);
						strcpy(modifTaille, taille);
						strcpy(modifPoids, poids);
					}
					gestionFichePatient(&zFichePrenom, &zFicheNom, &zFicheTaille, &zFichePoids, &modifPrenom, &modifNom, &modifTaille, &modifPoids);
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
					afficheChargerPatient(zChargerPatient,zPrenomChargerPatient,prenom_a_charger,zNomChargerPatient,nom_a_charger,zCharger);

					//rectangle(zNouveauPatient.xmin, zNouveauPatient.ymin, zNouveauPatient.xmax, zNouveauPatient.ymax);
					afficheNouveauPatient(zNouveauPatient, zPrenomNouveauPatient, nouveau_prenom, zNomNouveauPatient, nouveau_nom, zTailleNouveauPatient, chaine_taille_nouveau_patient, zPoidsNouveauPatient, chaine_poids_nouveau_patient, zCreerPatient);

					break;
				case 3:
					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre,3);
					affichePatientActuel(zPatientActuel,zPrenomPatientActuel,prenom, zNomPatientActuel,nom,zVoirFiche);
					afficheAnalyseVideo(zAnalyseVideo, zNomVideo, zChargerVideo, cheminVideoActuelle, zVideoOrigine, imageVideo, zAnalyser, zAnalyseEnCours, enCours, fleche);
					
					afficheAnalysesPrecedentes(zAnalysesPrecedentes, zBoutonsAnalysesPrecedentes, analyses, nbAnalyses);
					
					if (enCours)
					{
						enCoursAffichage = true;
					}
					break;
                case 4: // page analyse
					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre,3);
					affichePatientActuel(zPatientActuel,zPrenomPatientActuel,prenom, zNomPatientActuel,nom,zVoirFiche);
					afficheAnalyse(zAnalyse, zVideoSquelette, imageVideoSquelettes, zGraph, imageVideoCourbes);
					afficheDonneesBio(zDonneesBio, zTailleAnalyse, taille, zPoidsAnalyse, poids, zlongueurBrasAnalyse, longueurBras, zlongueurJambeAnalyse, longueurJambe);
					affichePathologies(zPathologies, zCourbe, courbe, zBoite, boite, ZMarcheNormale, marcheNorm);
					break;

				case 11: // page fiche patient
					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre,3);
					afficheFichePatient(zFichePatient, zFicheNom, modifNom, zFichePrenom, modifPrenom, zFicheTaille, modifTaille, zFichePoids, modifPoids, zFicheMaj);
					break;

			}
			break;

		case Clavier:
			if(zPrenomChargerPatient.saisie)
			{
				recupereTexte(&prenom_a_charger);
			}
			else if(zNomChargerPatient.saisie)
			{
				recupereTexte(&nom_a_charger);
			}
			else if(zPrenomNouveauPatient.saisie)
			{
				recupereTexte(&nouveau_prenom);
			}
			else if(zNomNouveauPatient.saisie)
			{
				recupereTexte(&nouveau_nom);
			}
			else if(zTailleNouveauPatient.saisie)
			{
				recupereTexteEntiers(&chaine_taille_nouveau_patient);
			}
			else if(zPoidsNouveauPatient.saisie)
			{
				recupereTexteEntiers(&chaine_poids_nouveau_patient);
			}
			else if(zNomVideo.saisie)
			{
				recupereTexte(&cheminVideoActuelle);
			}
			else if(zFichePrenom.saisie)
			{
				recupereTexte(&modifPrenom);
			}
			else if(zFicheNom.saisie)
			{
				recupereTexte(&modifNom);
			}
			else if(zFicheTaille.saisie)
			{
				recupereTexteEntiers(&modifTaille);
			}
			else if(zFichePoids.saisie)
			{
				recupereTexteEntiers(&modifPoids);
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
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					numPage=2;  //reviens à la page d'accueil
				}

				//bouton RETOUR
				else if ((numPage>2) && (zRetour.xmin<abscisseSouris() && abscisseSouris()<zRetour.xmax && zRetour.ymin<ordonneeSouris() && ordonneeSouris()<zRetour.ymax))
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					if (numPage == 11) // Page voir fiche
					{
						numPage = numPagePrecedente; // Retour a la page precendte
					}
					else if(numPage > 2)
						numPage--;
				}

				// bouton voir fiche patient
				else if(prenom != NULL && nom != NULL && numPage >=2 && zVoirFiche.xmin<abscisseSouris() && abscisseSouris()<zVoirFiche.xmax && zVoirFiche.ymin<ordonneeSouris() && ordonneeSouris()<zVoirFiche.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					numPagePrecedente = numPage;
					numPage = 11;
				}

				else if(numPage == 2 && zPrenomChargerPatient.xmin<abscisseSouris() && abscisseSouris()<zPrenomChargerPatient.xmax && zPrenomChargerPatient.ymin<ordonneeSouris() && ordonneeSouris()<zPrenomChargerPatient.ymax)
				{
					// désactive  toutes les zones de saisie et active celle en question
					zPrenomChargerPatient.saisie = true;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
				}

				else if(numPage == 2 && zNomChargerPatient.xmin<abscisseSouris() && abscisseSouris()<zNomChargerPatient.xmax && zNomChargerPatient.ymin<ordonneeSouris() && ordonneeSouris()<zNomChargerPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = true;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
				}

				else if(numPage == 2 && zPrenomNouveauPatient.xmin<abscisseSouris() && abscisseSouris()<zPrenomNouveauPatient.xmax && zPrenomNouveauPatient.ymin<ordonneeSouris() && ordonneeSouris()<zPrenomNouveauPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = true;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
				}

				else if(numPage == 2 && zNomNouveauPatient.xmin<abscisseSouris() && abscisseSouris()<zNomNouveauPatient.xmax && zNomNouveauPatient.ymin<ordonneeSouris() && ordonneeSouris()<zNomNouveauPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = true;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
				}

				else if(numPage == 2 && zTailleNouveauPatient.xmin<abscisseSouris() && abscisseSouris()<zTailleNouveauPatient.xmax && zTailleNouveauPatient.ymin<ordonneeSouris() && ordonneeSouris()<zTailleNouveauPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = true;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
				}

				else if(numPage == 2 && zPoidsNouveauPatient.xmin<abscisseSouris() && abscisseSouris()<zPoidsNouveauPatient.xmax && zPoidsNouveauPatient.ymin<ordonneeSouris() && ordonneeSouris()<zPoidsNouveauPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = true;
					zNomVideo.saisie = false;
				}
				else if(numPage == 2 && zCreerPatient.xmin<abscisseSouris() && abscisseSouris()<zCreerPatient.xmax && zCreerPatient.ymin<ordonneeSouris() && ordonneeSouris()<zCreerPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					int creationPatient = creeFichePatient(nouveau_nom, nouveau_prenom, chaine_taille_nouveau_patient, chaine_poids_nouveau_patient);
					if (creationPatient == 1)
					{
						printf("\nCreation du patient reussie\n");
						prenom = (char *)malloc(sizeof(char) * strlen(nouveau_prenom));
						nom = (char *)malloc(sizeof(char) * strlen(nouveau_nom));
						taille = (char *)malloc(sizeof(char) * strlen(chaine_taille_nouveau_patient));
						poids = (char *)malloc(sizeof(char) * strlen(chaine_poids_nouveau_patient));
						
						strcpy(prenom, nouveau_prenom);
						strcpy(nom, nouveau_nom);
						strcpy(taille, chaine_taille_nouveau_patient);
						strcpy(poids, chaine_poids_nouveau_patient);
						
						
						numPage=3;
						
						nbAnalyses = 0;
						// Pas d'analyses actuelles
						analyses = NULL;
						zBoutonsAnalysesPrecedentes = NULL;
						initZonesAnalysesPrecedentes(zPatientActuel, &zAnalysesPrecedentes, zBoutonsAnalysesPrecedentes, analyses, nbAnalyses);
					}
					else
					{
						printf("\nCreation du patient impossible\n");
					}
				}
				else if(numPage == 2 && zChargerPatient.xmin<abscisseSouris() && abscisseSouris()<zChargerPatient.xmax && zChargerPatient.ymin<ordonneeSouris() && ordonneeSouris()<zChargerPatient.ymax)
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					char nomTemp[32];
					char prenomTemp[32];
					char tailleTemp[32];
					char poidsTemp[32];
					int chargementPatient = lisFichePatient(nom_a_charger, prenom_a_charger, nomTemp, prenomTemp, tailleTemp, poidsTemp);
					if (chargementPatient == 1)
					{
						printf("\nChargement du patient reussi\n");
						prenom = (char *)malloc(sizeof(char) * strlen(prenomTemp));
						nom = (char *)malloc(sizeof(char) * strlen(nomTemp));
						taille = (char *)malloc(sizeof(char) * strlen(tailleTemp));
						poids = (char *)malloc(sizeof(char) * strlen(poidsTemp));
						strcpy(prenom, prenomTemp);
						strcpy(nom, nomTemp);
						strcpy(taille, tailleTemp);
						strcpy(poids, poidsTemp);
						numPage=3;
						
						// Chargement des analyses precedentes s'il y en a
						analyses = chargeAnalysesPatient(nom, prenom, &nbAnalyses);
						zBoutonsAnalysesPrecedentes = (zone*) malloc(nbAnalyses * sizeof(zone)); // On alloue le tableau avec le nombre d'analyses
						initZonesAnalysesPrecedentes(zPatientActuel, &zAnalysesPrecedentes, zBoutonsAnalysesPrecedentes, analyses, nbAnalyses);
						//int test1 = ajouteElementTableau(nom, prenom, "Videos", "TESTAJOUTVIDEO"); // Ajoute la video TESTAJOUTEVIDEO au fichier txt patient
						//int test2 = modifieChamp(nom, prenom, "Courbe", "Non"); // Change la valeur de Courbe dans le fichir txt du patient
					}
					else
					{
						printf("\nChargement du patient impossible\n");
					}
				}
				else if(numPage == 3 && zNomVideo.xmin<abscisseSouris() && abscisseSouris()<zNomVideo.xmax && zNomVideo.ymin<ordonneeSouris() && ordonneeSouris()<zNomVideo.ymax)
				{
					printf("\nsaisie video\n");
					zNomVideo.saisie = true;
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
				}
				
				else if(numPage == 3 && cheminVideoActuelle != NULL && zChargerVideo.xmin<abscisseSouris() && abscisseSouris()<zChargerVideo.xmax && zChargerVideo.ymin<ordonneeSouris() && ordonneeSouris()<zChargerVideo.ymax)
				{
					printf("\nLance video\n");
					//clicChargerVideo(&zSaisieTexteChargeVideo,&dossier_existe,&numPage,&cheminDossierVideo, &videoDirectory, &cheminDossierImagesTraitees, &cheminDossierImagesACorriger);
					compteurImageAlpha = 0;
					char fullPath[256];
					sprintf(fullPath, "%s/%s", CHEMIN_DOSSIER_VIDEO, cheminVideoActuelle);
					nbImageAlpha = scandir(fullPath, &listeImageAlpha, NULL, alphasort);
					printf("%d\n", nbImageAlpha);
					if (nbImageAlpha == -1)
					{
						lecture = 0;
					}
					else
					{
						lecture = 1;
					}
				}
				else if(numPage == 3 && cheminVideoActuelle != NULL && lecture == 1 && zAnalyser.xmin<abscisseSouris() && abscisseSouris()<zAnalyser.xmax && zAnalyser.ymin<ordonneeSouris() && ordonneeSouris()<zAnalyser.ymax)
				{
					enCours = true;
				}
				else if(numPage == 3 && nbAnalyses >0 && analyses != NULL && zAnalysesPrecedentes.xmin<abscisseSouris() && abscisseSouris()<zAnalysesPrecedentes.xmax && zAnalysesPrecedentes.ymin<ordonneeSouris() && ordonneeSouris()<zAnalysesPrecedentes.ymax)
				{
					int p = 0;
					for (p=0; p<nbAnalyses;p++)
					{
						// SI on clic sur un bouton de la liste
						if (zBoutonsAnalysesPrecedentes[p].xmin<abscisseSouris() && abscisseSouris()<zBoutonsAnalysesPrecedentes[p].xmax && zBoutonsAnalysesPrecedentes[p].ymin<ordonneeSouris() && ordonneeSouris()<zBoutonsAnalysesPrecedentes[p].ymax)
						{
							// MEme chose que quand on lance une analyse sauf qu'on a pas a cree l'analyse car elle existe deja
							lecture = 0;
							enCours = false;
							enCoursAffichage = false;
							resetPageAnalyse = true;
							
							//On doit changer date par la date sur laquelle on a clique, pareil pour poids et taille et le reste
							strcpy(date, analyses[p].dateHeure);
							strcpy(poids, analyses[p].poids);
							strcpy(taille, analyses[p].taille);
							strcpy(boite, analyses[p].boite);
							strcpy(marcheNorm, analyses[p].marcheNormale);
							strcpy(longueurBras, analyses[p].longueurBras);
							strcpy(longueurJambe, analyses[p].longueurJambe);
							numPage = 4;
						}
					}
				}
				else if(numPage == 11 && zFichePrenom.xmin<abscisseSouris() && abscisseSouris()<zFichePrenom.xmax && zFichePrenom.ymin<ordonneeSouris() && ordonneeSouris()<zFichePrenom.ymax)
				{
					zFichePrenom.saisie = true;
					zFicheNom.saisie = false;
					zFichePoids.saisie = false;
					zFicheTaille.saisie = false;
				}
				else if(numPage == 11 && zFicheNom.xmin<abscisseSouris() && abscisseSouris()<zFicheNom.xmax && zFicheNom.ymin<ordonneeSouris() && ordonneeSouris()<zFicheNom.ymax)
				{
					zFichePrenom.saisie = false;
					zFicheNom.saisie = true;
					zFichePoids.saisie = false;
					zFicheTaille.saisie = false;
				}
				else if(numPage == 11 && zFichePoids.xmin<abscisseSouris() && abscisseSouris()<zFichePoids.xmax && zFichePoids.ymin<ordonneeSouris() && ordonneeSouris()<zFichePoids.ymax)
				{
					zFichePrenom.saisie = false;
					zFicheNom.saisie = false;
					zFichePoids.saisie = true;
					zFicheTaille.saisie = false;
				}
				else if(numPage == 11 && zFicheTaille.xmin<abscisseSouris() && abscisseSouris()<zFicheTaille.xmax && zFicheTaille.ymin<ordonneeSouris() && ordonneeSouris()<zFicheTaille.ymax)
				{
					zFichePrenom.saisie = false;
					zFicheNom.saisie = false;
					zFichePoids.saisie = false;
					zFicheTaille.saisie = true;
				}
				else if(numPage == 11 && zFicheMaj.xmin<abscisseSouris() && abscisseSouris()<zFicheMaj.xmax && zFicheMaj.ymin<ordonneeSouris() && ordonneeSouris()<zFicheMaj.ymax)
				{
					zFichePrenom.saisie = false;
					zFicheNom.saisie = false;
					zFichePoids.saisie = false;
					zFicheTaille.saisie = false;
					
					
					int modifOk = changeNomDossier(nom, prenom, modifNom, modifPrenom);
					if (modifOk != -1)
					{
						printf("\nMise a jour fiche ok\n");
						if (strcmp(modifPrenom, "") != 0)
						{
							modifieChamp(modifNom, modifPrenom, "Prenom", modifPrenom);
							prenom = (char *)malloc(sizeof(char) * strlen(modifPrenom));
							strcpy(prenom, modifPrenom);
							printf("\nPrenom modifie\n");
						}
						if (strcmp(modifNom, "") != 0)
						{
							modifieChamp(modifNom, modifPrenom, "Nom", modifNom);
							nom = (char *)malloc(sizeof(char) * strlen(modifNom));
							strcpy(nom, modifNom);
							printf("\nNom modifie\n");
						}
						if (strcmp(modifTaille, "") != 0)
						{
							modifieChamp(modifNom, modifPrenom, "Taille", modifTaille);
							taille = (char *)malloc(sizeof(char) * strlen(modifTaille));
							strcpy(taille, modifTaille);
							printf("\nPTaille modifie\n");
						}
						if (strcmp(modifPoids, "") != 0)
						{
							modifieChamp(modifNom, modifPrenom, "Poids", modifPoids);
							poids = (char *)malloc(sizeof(char) * strlen(modifPoids));
							strcpy(poids, modifPoids);
							printf("\nPoids modifie\n");
						}
						numPage = numPagePrecedente;
					}
					else
					{
						printf("\nERREUR lors de Mise a jour fiche\n");
					}
				}
				else // clic sur toute autre zone de l'ihm
				{
					zPrenomChargerPatient.saisie = false;
					zNomChargerPatient.saisie = false;
					zPrenomNouveauPatient.saisie = false;
					zNomNouveauPatient.saisie = false;
					zTailleNouveauPatient.saisie = false;
					zPoidsNouveauPatient.saisie = false;
					zNomVideo.saisie = false;
					zFichePrenom.saisie = false;
					zFicheNom.saisie = false;
					zFichePoids.saisie = false;
					zFicheTaille.saisie = false;
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
					redimensionneZoneNouveauPatient(zChargerPatient,&zNouveauPatient, &zPrenomNouveauPatient, &zNomNouveauPatient, &zTailleNouveauPatient, &zPoidsNouveauPatient, &zCreerPatient);
					break;
				case 3:
					redimensionneZonePatientActuel(zTitre, &zPatientActuel, &zVoirFiche, &zPrenomPatientActuel, &zNomPatientActuel);
					redimensionneZoneAnalyseVideo(zPatientActuel, &zAnalyseVideo, &zNomVideo, &zChargerVideo, &zVideoOrigine, &zAnalyser, &zAnalyseEnCours);
					redimensionneZoneAnalysesPrecedentes(zPatientActuel, &zAnalysesPrecedentes, zBoutonsAnalysesPrecedentes, analyses, nbAnalyses);
					break;
                case 4:
					redimensionneZonePatientActuel(zTitre, &zPatientActuel, &zVoirFiche, &zPrenomPatientActuel, &zNomPatientActuel);
					redimensionneZoneAnalyse(zPatientActuel, &zAnalyse, &zVideoSquelette, &zGraph);
                    redimensionneZoneDonneesBio(zAnalyse, &zDonneesBio, &zTailleAnalyse, &zPoidsAnalyse, &zlongueurBrasAnalyse, &zlongueurJambeAnalyse);
                    redimensionneZonePathologies(zDonneesBio, &zPathologies, &zCourbe, &zBoite, &ZMarcheNormale);
					break;
				case 11:
					redimensionneZoneFichePatient(&zFichePatient, &zFicheNom, &zFichePrenom, &zFicheTaille, &zFichePoids, &zFicheMaj);
					break;
				default:
					break;
			}
			rafraichisFenetre();
			break;
	}
}
