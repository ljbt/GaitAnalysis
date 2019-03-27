#define AFFICHAGE true
#define DELAY_SCREEN 3

typedef struct zone {
	int longueur, hauteur; //sert à determiner les coordonnees, ou inversement
	int espaceHaut, espaceBas; //peut servir dans le redimmensionnement en hauteur
	int xmin, ymin, xmax, ymax; // coordonnees de la diagonale de la zone rectangulaire
	char texte[30];
		
	}zone; // définition d'une zone cliquable


typedef struct _zones{
	zone z1, z2, z3, z4, z5, z6, z7, z8, z9;
	}_zones;

