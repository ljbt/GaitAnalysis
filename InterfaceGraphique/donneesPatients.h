#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#define CHEMIN_DOSSIER_PATIENT "../opencv_files/patients"

int creeFichePatient(char* nom, char* prenom, char* taille, char* poids);
int lisFichePatient(char* nomBase, char* prenomBase, char* nomModif, char* prenomModif);
