#define CHEMIN_DOSSIER_PATIENT "../opencv_files/patients"
#define CHEMIN_DOSSIER_VIDEO "../opencv_files/learning_videos"
#define CHEMIN_DOSSIER_COURBES "../opencv_files/courbes"
#define CHEMIN_DOSSIER_SQUELETTES "../opencv_files/squelettes"

#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include "Mask.h"
#include "imageProcessing.h"

extern "C"
{
	#include "BmpLib.h"
	#include "definitions.h"
}


int creeFichePatient(char* nom, char* prenom, char* taille, char* poids);
int lisFichePatient(char* nomBase, char* prenomBase, char* nomModif, char* prenomModif, char* tailleModif, char* poidsModif);

//Ajoute un element a un tableau cle dans le fichier txt du patient, separe par une virgule car c'est un tableau
int ajouteElementTableau(char* nom, char* prenom, char* cleTab, char* valTab);

//Modifie la valeur d'un champ cle dans le fichier txt du patient 
int modifieChamp(char* nom, char* prenom, char* cleChamp, char* valChamp);
int changeNomDossier(char* nomActuel, char* prenomActuel, char* modifNom, char *modifPrenom);

int creeAnalysePatient(char* nom, char* prenom, char* taille, char* poids, char* courbe, char* claudification, char* marcheRegu, char* video, int nbImages, char* dateHeure);
analyse lisAnalysePatient(char* nom, char* prenom, char* nomFichier);
analyse* chargeAnalysesPatient(char* nom, char* prenom, int* nbAnalyses);
DonneesImageRGB *lisImageCouranteAlphabetique(struct dirent *lecture, char *folderPath, char **nomImageVideo);
int extraitCourbesDossier(char* nomDossier, int nbImages, char* dateHeure);
int extraitCourbesSquelettesDossier(char* nomDossier, int nbImages, char* dateHeure);
