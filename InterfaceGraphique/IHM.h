
void initZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, int LargeurFenetre, int HauteurFenetre, char *titre);
void redimensionneZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, int LargeurFenetre, int HauteurFenetre);

void monIHM(zone zQuit, zone zHome, zone zRetour, int numpage, int LargeurFenetre); //affiche éléments principaux de l'IHM (boutons quitter retour home, logo)

void afficheTitre(zone zTitre); //affiche chaine en grand dans la zone titre
void afficheAcceuil(zone zTitre);

void afficheMenu(zone zP1); //affiche chaine dans rectangle

void afficheImage(zone zITest, DonneesImageRGB *image);//affiche image dans zone

