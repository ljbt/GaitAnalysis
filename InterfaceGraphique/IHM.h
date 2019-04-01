int longueurChaineEnPixels(char *chaine, int hauteur_chaine);
int xminChaineCentree(char *chaine, int hauteur_chaine);

 //affiche éléments principaux de l'IHM (boutons quitter retour home, logo)
void monIHM(zone zQuit, zone zHome, zone zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, DonneesImageRGB *logo, int numpage);

void afficheTitre(zone zTitre, int epaisseur_trait); 
void afficheAcceuil(zone zTitre);

void afficheImage(zone zITest, DonneesImageRGB *image);//affiche image dans zone


void changeTexteZone(zone *z, char *chaine);

void initZoneTitre(zone *zTitre, char *titre);
void initZones(zone *zQuit, zone *zHome, zone *zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix);
void redimensionneZones(zone *zQuit, zone *zHome, zone *zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix);
void changeZoneTitre(zone *zTitre,int numPage);
void redimmensionneZoneTitre(zone *zTitre);

void initZonesPatientActuel(zone zTitre, zone *zPatientActuel, zone *zPrenomPatientActuel, zone *zNomPatientActuel, zone *zVoirFiche);
void redimmensionneZonePatientActuel(zone *zPatientActuel, zone zTitre);
void gestionNomPrenomPatient(zone *zPrenom, zone *zNom, char **prenom, char **nom);
 void affichePatientActuel(zone zPatientActuel, zone zPrenom, char *prenom, zone zNom,char *nom, zone zVoirFiche);
