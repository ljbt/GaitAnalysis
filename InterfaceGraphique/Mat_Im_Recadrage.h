//fonctions pour tableaux
void afficheTab(int *tab, int taille);
int maxTab (int *tab, int taille); //retourne maximum du tableau d'entiers

//fonctions gestion matrices
int ** alloueMatrice(int h, int l);
void cree3Matrices(DonneesImageRGB * image, int *** bleu, int *** vert, int *** rouge);
void allocation4Matrices(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s);
void creeMatSeuil(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s);
void creeMatNG(DonneesImageRGB * image, int ** b, int ** v, int ** r, int ***s);

//fonctions gestion image
DonneesImageRGB *creeImage(int h, int l, int **r, int **v, int **b); //retourne une image à partir des matrice r v b
void seuillage(DonneesImageRGB * image, int ***s, int seuil); //seuille la matrice s au seuil donné

//fonctions gestion recadrage
void creePetiteMat(DonneesImageRGB * image,DonneesImageRGB **petite_image, int **s, int ***petite_s, zone *zPetiteI, int * petite_h, int *petite_l);//pr creer matrice a partir d'une forme noire de l'image seuillee

