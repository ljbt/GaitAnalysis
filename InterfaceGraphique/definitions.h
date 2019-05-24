#define AFFICHAGE true
#define DELAY_SCREEN 3
#define HAUTEUR_TITRE_ACCUEIL 70
#define HAUTEUR_TITRE 40
#define HAUTEUR_TEXTE 20

typedef struct zone {
	int longueur, hauteur; 
	int espaceHaut, espaceBas; 
	int espaceGauche, espaceDroite; 
	int xmin, ymin, xmax, ymax;
	char *texte;
	bool saisie;
	}zone; 


typedef struct _zones{
	zone z1, z2, z3, z4, z5, z6, z7, z8, z9;
	}_zones;

typedef struct analyse {
	char poids[8];
	char taille[8];
	char courbe[8];
	char marcheReguliere[8];
	char boite[8];
	char longueurBras[8];
	char longueurJambe[8];
	char dateHeure[32];
	char video[128];
	} analyse;
