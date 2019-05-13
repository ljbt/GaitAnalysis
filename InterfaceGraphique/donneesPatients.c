#include "donneesPatients.h"

int creeFichePatient(char* nom, char* prenom, char* taille, char* poids)
{
	char filepath[128];
	sprintf(filepath, "%s/%s_%s.txt", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier
	FILE* fiche = fopen(filepath, "w");
	if (fiche == NULL)
	{
		fprintf(stderr, "Erreur lors de la creation du fichier patient : %s", filepath);
		return -1;
	}
	
	// On cree les champs caracteristiques
	fprintf(fiche, "Prenom:%s\n", prenom);
	fprintf(fiche, "Nom:%s\n", nom);
	fprintf(fiche, "Taille:%s\n", taille);
	fprintf(fiche, "Poids:%s\n", poids);
	
	// On cree les futurs champs 
	fprintf(fiche, "Courbe:\n");
	fprintf(fiche, "Marche:\n");
	fprintf(fiche, "Videos:\n");
	
	fclose(fiche);
	return 1;
}

int lisFichePatient(char* nomBase, char* prenomBase, char* nomModif, char* prenomModif)
{
	char filepath[128];
	char buffer[128];
	sprintf(filepath, "%s/%s_%s.txt", CHEMIN_DOSSIER_PATIENT, prenomBase, nomBase); // On remple filepath avec le chemin et nom du fichier
	FILE* fiche = fopen(filepath, "r");
	if (fiche == NULL)
	{
		fprintf(stderr, "Erreur lors de la lecture du fichier patient : %s", filepath);
		return -1;
	}
	
	while (fgets(buffer, 128, fiche) != NULL) // On parcours chaque ligne du fichier et on s'arrete quand il n'y a plus de ligne
	{
		char cle[32];
		char valeur[32];
		sscanf(buffer, "%[^:]:%s\n", cle, valeur); // On lit chaque ligne en suivant un pattern precis qui est cle:valeur\n
		
		if (strcmp(cle, "Prenom") == 0) // Si on est sur la ligne du prenom
		{
			strcpy(prenomModif, valeur); // On copie la valeur actuelle (donc le prenom) dans prenomModif
		} else if (strcmp(cle, "Nom") == 0) // Si on est sur la ligne du nom
		{
			strcpy(nomModif, valeur); // On copie la valeur actuelle (donc le nom) dans nomModif
		}
	}
	fclose(fiche);
	return 1;
}
