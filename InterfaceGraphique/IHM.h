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
void redimensionneZoneTitre(zone *zTitre);

void initZonesPatientActuel(zone zTitre, zone *zPatientActuel, zone *zPrenomPatientActuel, zone *zNomPatientActuel, zone *zVoirFiche);
void redimensionneZonePatientActuel(zone zTitre, zone *zPatientActuel, zone *zVoirFiche, zone *zPrenomPatientActuel, zone *zNomPatientActuel );
void gestionNomPrenomPatient(zone *zPrenom, zone *zNom, char **prenom, char **nom);
void affichePatientActuel(zone zPatientActuel, zone zPrenom, char *prenom, zone zNom,char *nom, zone zVoirFiche);

void initZonesChargerPatient(zone zPatientActuel, zone *zChargerPatient, zone *zPrenom, zone *zNom, zone *zCharger);
void afficheChargerPatient(zone zChargerPatient, zone zPrenom, char *prenom, zone zNom, char *nom, zone zCharger);
void redimensionneZoneChargerPatient(zone zPatientActuel,zone *zChargerPatient,zone *zPrenom,zone *zNom,zone *zCharger);

void initZonesNouveauPatient(zone zChargerPatient, zone *zNouveauPatient, zone *zPrenom, zone *zNom, zone *zTaille, zone *zPoids, zone *zCreerPatient);
void afficheNouveauPatient(zone zNouveauPatient, zone zPrenom, char *prenom, zone zNom, char *nom, zone zTaille, char *taille, zone zPoids, char *poids, zone zCreerPatient);
void redimensionneZoneNouveauPatient(zone zChargerPatient,zone *zNouveauPatient,zone *zPrenom,zone *zNom, zone *zTaille,zone *zPoids,zone *zCreerPatient);

void initZonesAnalyseVideo(zone zPatientActuel, zone * zAnalyseVideo, zone *zNomVideo, zone *zChargerVideo, zone *zVideoOrigine, zone *zAnalyser, zone *zAnalyseEnCours);
void afficheAnalyseVideo(zone zAnalyseVideo, zone zNomVideo, zone zChargerVideo, char* nomVideo, zone zVideoOrigine, DonneesImageRGB* video, zone zAnalyser, zone zAnalyseEnCours, bool enCours, DonneesImageRGB* fleche);
void redimensionneZoneAnalyseVideo(zone zPatientActuel, zone * zAnalyseVideo, zone *zNomVideo, zone *zChargerVideo, zone *zVideoOrigine, zone *zAnalyser, zone *zAnalyseEnCours);
void gestionVideoActuelle(zone *zVideoOrigine, char **nomVideo);

void initZonesAnalysesPrecedentes(zone zPatientActuel, zone * zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses);
void afficheAnalysesPrecedentes(zone zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses);
void redimensionneZoneAnalysesPrecedentes(zone zPatientActuel, zone * zAnalysesPrecedentes, zone* zBoutonsAnalysesPrecedentes, analyse* analysesPrecedentes, int nbAnalyses);

void initZonesAnalyse(zone zPatientActuel, zone *zAnalyse, zone *zVideoSquelette, zone *zGraph);
void afficheAnalyse(zone zAnalyse, zone zVideoSquelette, DonneesImageRGB* videoSquelette, zone zGraph, DonneesImageRGB* videoGraph);
void redimensionneZoneAnalyse(zone zPatientActuel, zone *zAnalyse, zone *zVideoSquelette, zone *zGraph);

void initZonesDonneesBio(zone zAnalyse, zone *zDonneesBio, zone *zTailleAnalyse, zone *zPoidsAnalyse, zone *zlongueurBrasAnalyse, zone *zlongueurJambeAnalyse);
void afficheDonneesBio(zone zDonneesBio, zone zTailleAnalyse, char* taille, zone zPoidsAnalyse, char* poids, zone zlongueurBrasAnalyse, char* longueurBras, zone zlongueurJambeAnalyse, char* longueurJambe);
void redimensionneZoneDonneesBio(zone zAnalyse, zone *zDonneesBio, zone *zTailleAnalyse, zone *zPoidsAnalyse, zone *zlongueurBrasAnalyse, zone *zlongueurJambeAnalyse);

void initZonesPathologies(zone zDonneesBio, zone *zPathologies, zone *zCourbe, zone *zBoite, zone* zMarcheReguliere);
void affichePathologies(zone zPathologies, zone zCourbe, char* courbe, zone zBoite, char* boite, zone zMarcheReguliere, char* marcheRegu);
void redimensionneZonePathologies(zone zDonneesBio, zone *zPathologies, zone *zCourbe, zone *zBoite, zone* zMarcheReguliere);

void initZonesFichePatient(zone *zFichePatient, zone *zFicheNom, zone *zFichePrenom, zone *zFicheTaille, zone *zFichePoids, zone *zMaj);
void afficheFichePatient(zone zFichePatient, zone zFicheNom, char* nom, zone zFichePrenom, char* prenom, zone zFicheTaille, char* taille, zone zFichePoids, char* poids, zone zMaj);
void redimensionneZoneFichePatient(zone *zFichePatient, zone *zFicheNom, zone *zFichePrenom, zone *zFicheTaille, zone *zFichePoids, zone *zMaj);
void gestionFichePatient(zone *zFichePrenom, zone *zFicheNom, zone *zFicheTaille, zone *zFichePoids, char **prenom, char **nom, char** taille, char** poids);

void recupereTexte(char **chaine);
void recupereTexteEntiers(char **chaine);
