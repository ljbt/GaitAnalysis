#include "donneesPatients.h"


using namespace cv;

int creeFichePatient(char* nom, char* prenom, char* taille, char* poids)
{
	char dirpath[64];
	sprintf(dirpath, "%s/%s_%s", CHEMIN_DOSSIER_PATIENT, prenom, nom);
	mkdir(dirpath, ACCESSPERMS);
	
	char filepath[128];
	sprintf(filepath, "%s/patient.txt", dirpath); // On remple filepath avec le chemin et nom du fichier
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
	//fprintf(fiche, "Courbe:\n");
	//fprintf(fiche, "Marche:\n");
	//fprintf(fiche, "Videos:\n");
	
	fclose(fiche);
	return 1;
}

int lisFichePatient(char* nomBase, char* prenomBase, char* nomModif, char* prenomModif, char* tailleModif, char* poidsModif)
{
	char filepath[128];
	char buffer[256];
	sprintf(filepath, "%s/%s_%s/patient.txt", CHEMIN_DOSSIER_PATIENT, prenomBase, nomBase); // On remple filepath avec le chemin et nom du fichier
	FILE* fiche = fopen(filepath, "r");
	if (fiche == NULL)
	{
		fprintf(stderr, "Erreur lors de la lecture du fichier patient : %s", filepath);
		return -1;
	}
	
	while (fgets(buffer, 256, fiche) != NULL) // On parcours chaque ligne du fichier et on s'arrete quand il n'y a plus de ligne
	{
		char cle[256];
		char valeur[256];
		sscanf(buffer, "%[^:]:%s\n", cle, valeur); // On lit chaque ligne en suivant un pattern precis qui est cle:valeur\n
		
		if (strcmp(cle, "Prenom") == 0) // Si on est sur la ligne du prenom
		{
			strcpy(prenomModif, valeur); // On copie la valeur actuelle (donc le prenom) dans prenomModif
		}
		else if (strcmp(cle, "Nom") == 0) // Si on est sur la ligne du nom
		{
			strcpy(nomModif, valeur); // On copie la valeur actuelle (donc le nom) dans nomModif
		}
		else if (strcmp(cle, "Poids") == 0) // Si on est sur la ligne du nom
		{
			strcpy(poidsModif, valeur); // On copie la valeur actuelle (donc le nom) dans nomModif
		}
		else if (strcmp(cle, "Taille") == 0) // Si on est sur la ligne du nom
		{
			strcpy(tailleModif, valeur); // On copie la valeur actuelle (donc le nom) dans nomModif
		}
	}
	fclose(fiche);
	return 1;
}

int ajouteElementTableau(char* nom, char* prenom, char* cleTab, char* valTab)
{
	char filepath[128];
	char filepathTmp[128];
	char buffer[256];
	sprintf(filepath, "%s/%s_%s/patient.txt", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier
	sprintf(filepathTmp, "%s/%s_%s/patient.tmp", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier temporaire
	FILE* ficheActuelle = fopen(filepath, "r"); // Ouverture en lecture du fichier actuel
	FILE* ficheNew = fopen(filepathTmp, "w"); // Ouverture en ecriture du fichier temporaire
	
	if (ficheActuelle == NULL)
	{
		fprintf(stderr, "Erreur lors de la lecture du fichier patient : %s", filepath);
		return -1;
	}
	while (fgets(buffer, 256, ficheActuelle) != NULL) // On parcours chaque ligne du fichier et on s'arrete quand il n'y a plus de ligne
	{
		fprintf(ficheNew, "%s", buffer); // On copie la ligne actuelle dans le nouveau fichier
		
		char cle[256];
		char valeur[256];
		sscanf(buffer, "%[^:]:%s\n", cle, valeur); // On lit chaque ligne en suivant un pattern precis qui est cle:valeur\n
		
		if (strcmp(cle, cleTab) == 0) // Si on est sur la ligne de la bonne cle
		{
			fseek(ficheNew, -1, SEEK_CUR); // Decale le pointeur de fichier un cran avant la fin de la ligne actuellement lue
			fprintf(ficheNew, "%s,\n", valTab); // Ecris la valeur a ajouter a la position actuelle du pointeur de fichier
		}
	}
	
	fclose(ficheActuelle);
	fclose(ficheNew);
	
	remove(filepath); // Suppression de l'ancienne fiche
	rename(filepathTmp, filepath); // Renommage de la nouvelle fiche
	return 1;
}

int modifieChamp(char* nom, char* prenom, char* cleChamp, char* valChamp)
{
	char filepath[128];
	char filepathTmp[128];
	char buffer[256];
	sprintf(filepath, "%s/%s_%s/patient.txt", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier
	sprintf(filepathTmp, "%s/%s_%s/patient.tmp", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier temporaire
	FILE* ficheActuelle = fopen(filepath, "r"); // Ouverture en lecture du fichier actuel
	FILE* ficheNew = fopen(filepathTmp, "w"); // Ouverture en ecriture du fichier temporaire
	
	if (ficheActuelle == NULL)
	{
		fprintf(stderr, "Erreur lors de la lecture du fichier patient : %s", filepath);
		return -1;
	}
	while (fgets(buffer, 256, ficheActuelle) != NULL) // On parcours chaque ligne du fichier et on s'arrete quand il n'y a plus de ligne
	{
		fprintf(ficheNew, "%s", buffer); // On copie la ligne actuelle dans le nouveau fichier
		
		char cle[256] = "";
		char valeur[256] = "";
		sscanf(buffer, "%[^:]:%s\n", cle, valeur); // On lit chaque ligne en suivant un pattern precis qui est cle:valeur\n
		
		if (strcmp(cle, cleChamp) == 0) // Si on est sur la ligne de la bonne cle
		{
			printf("\n**%s:%d**\n", valeur, strlen(valeur));
			
			fseek(ficheNew, -(1+strlen(valeur)), SEEK_CUR); // Decale le pointeur de fichier apres les :
			fprintf(ficheNew, "%s\n", valChamp); // Ecris la valeur a ajouter a la position actuelle du pointeur de fichier
		}
	}
	
	fclose(ficheActuelle);
	fclose(ficheNew);
	
	remove(filepath); // Suppression de l'ancienne fiche
	rename(filepathTmp, filepath); // Renommage de la nouvelle fiche
	return 1;
}

// cree une analyse en fichier txt et lance la fonction d'extraction des courbes. Replie egalement la variable dateHeure avec le timestamp genere de la forme dd-mm-YYYY_HH:MM:SS
int creeAnalysePatient(char* nom, char* prenom, char* taille, char* poids, char **stringlongueurBras,char **stringlongueurJambe, char* claudification, char* marcheNorm, char* video, int nbImages, char* dateHeure )
{	
	char filepath[128];
	time_t timestamp; 
    struct tm *t; 
  
    time(&timestamp); 
    t = localtime(&timestamp); 
    
    char dateChaine[32];
    //sprintf(dateChaine, "%d-%d-%d_%d:%d:%d", t->tm_mday, t->tm_mon, t->tm_year, t->tm_hour, t->tm_min, t->tm_sec);
    strftime(dateChaine, sizeof(dateChaine), "%d-%m-%Y_%X", t); 
	sprintf(filepath, "%s/%s_%s/%s.txt", CHEMIN_DOSSIER_PATIENT, prenom, nom, dateChaine); // On remple filepath avec le chemin et nom du fichier
	strcpy(dateHeure, dateChaine);
	FILE* fiche = fopen(filepath, "w"); // Ouverture en lecture du fichier actuel
	if (fiche == NULL)
	{
		fprintf(stderr, "Erreur lors de la creation de l'analyse : %s", filepath);
		return -1;
	}
	


	double longueurBras,longueurJambe;
	bool marcheNormale;
	int extract = extraitCourbesSquelettesDossier(video, nbImages, dateChaine, &longueurBras, &longueurJambe, &marcheNormale,  claudification);
    
    ostringstream strs1,strs2;
    strs1 << longueurBras;
    string str = strs1.str();
    *stringlongueurBras = (char *)malloc(sizeof(char) * 8);
    strcpy(*stringlongueurBras,str.c_str());

    strs2 << longueurJambe;
    str = strs2.str();
    *stringlongueurJambe = (char *)malloc(sizeof(char) * 8);
    strcpy(*stringlongueurJambe,str.c_str());

    if(marcheNormale)
    {
        strcpy(marcheNorm, "Yes");
    }
    else strcpy(marcheNorm, "No");
    
	// On cree les champs caracteristiques
	
	// On cree les futurs champs
	fprintf(fiche, "Taille:%s\n", taille);
	fprintf(fiche, "Poids:%s\n", poids);
	fprintf(fiche, "Claudification:%s\n", claudification);
	fprintf(fiche, "Marche:%s\n", marcheNorm);
	fprintf(fiche, "Video:%s\n", video);
    fprintf(fiche, "Bras:%s\n", *stringlongueurBras);
	fprintf(fiche, "Jambe:%s\n", *stringlongueurJambe);

	fclose(fiche);
	
	return 1;
}

// Lis un fichier d'analyse et remplie un structure analyse qui est retournee
analyse lisAnalysePatient(char* nom, char* prenom, char* nomFichier)
{
	char filepath[128];
	char buffer[256];
    analyse a;
	sprintf(filepath, "%s/%s_%s/%s", CHEMIN_DOSSIER_PATIENT, prenom, nom, nomFichier); // On remple filepath avec le chemin et nom du fichier
	FILE* fichierAnalyse = fopen(filepath, "r"); // Ouverture en lecture du fichier actuel
	if (fichierAnalyse == NULL)
	{
		fprintf(stderr, "Erreur lors de la lecture de l'analyse : %s", filepath);
		return a;
	}
	printf("\nChargement fichier %s ok\n", filepath);
	sscanf(nomFichier, "%[^.txt]", a.dateHeure);
	printf("\nsscanf ok %s \n", a.dateHeure);
	while (fgets(buffer, 256, fichierAnalyse) != NULL) // On parcours chaque ligne du fichier et on s'arrete quand il n'y a plus de ligne
	{
		char cle[256];
		char valeur[256];
		sscanf(buffer, "%[^:]:%s\n", cle, valeur); // On lit chaque ligne en suivant un pattern precis qui est cle:valeur\n
		
		if (strcmp(cle, "Taille") == 0) 
		{
			strcpy(a.taille, valeur); 
		}
		else if (strcmp(cle, "Poids") == 0) 
		{
			strcpy(a.poids, valeur); 
		}
		else if (strcmp(cle, "Claudification") == 0) 
		{
			strcpy(a.boite, valeur); 
		}
		else if (strcmp(cle, "Marche") == 0) 
		{
			strcpy(a.marcheNormale, valeur); 
		}
		else if (strcmp(cle, "Video") == 0) 
		{
			strcpy(a.video, valeur); 
		}
		else if (strcmp(cle, "Bras") == 0) 
		{
			strcpy(a.longueurBras, valeur); 
		}
		else if (strcmp(cle, "Jambe") == 0) 
		{
			strcpy(a.longueurJambe, valeur); 
		}
	}	
	fclose(fichierAnalyse);
	return a;
}


// Lis le dossier correspondant au nom du patient et renvoie la liste des analyses. Remplie egalement la variable nbAnalyses avec le nombre d'analyses trouvees
analyse* chargeAnalysesPatient(char* nom, char* prenom, int* nbAnalyses)
{
	char cheminPatient[128];
	int i = 0;
	int j = 0;
	struct dirent** listeFichiers;
	analyse* analyses;
	char buffer[256];
	sprintf(cheminPatient, "%s/%s_%s", CHEMIN_DOSSIER_PATIENT, prenom, nom); // On remple filepath avec le chemin et nom du fichier
	int nbFichiers = scandir(cheminPatient, &listeFichiers, NULL, alphasort);
	printf("nbFichiers : %d\n", nbFichiers);
	if (nbFichiers == -1)
	{
		return NULL;
	}
	else
	{
		*nbAnalyses = 0;
		// Premier parcours pour compter le bon nombre de fichiers (on ne compte pas . .. et patient.txt)
		for (i=0; i<nbFichiers;i++)
		{
			struct dirent* fichier = listeFichiers[i];
			if (strcmp("..", fichier->d_name) != 0 && strcmp(".", fichier->d_name) != 0 && strcmp("patient.txt", fichier->d_name) != 0) 
			{
				*nbAnalyses = *nbAnalyses + 1;
			}
		}
		printf("nbAnalyses: %d\n", *nbAnalyses);
		// Deuxieme parcours avec un malloc a la bonne taille
		analyses = (analyse*) malloc(*nbAnalyses * sizeof(analyse));
		
		for (i=0; i<nbFichiers;i++)
		{
			struct dirent* fichier = listeFichiers[i];
			if (strcmp("..", fichier->d_name) != 0 && strcmp(".", fichier->d_name) != 0 && strcmp("patient.txt", fichier->d_name) != 0) 
			{
				analyses[j] = lisAnalysePatient(nom, prenom, fichier->d_name);
				j++;
			}
		}
	}
	return analyses;
}


// fonction qui lit l'image courante d'un tableau trie en ordre alphabetique et se sert de folderPath pour aller lire l'image.
// retourne NULL si erreur.
// enregistre le nom de l'image dans nomImageVideo.
DonneesImageRGB *lisImageCouranteAlphabetique(struct dirent *lecture, char *folderPath, char **nomImageVideo)
{
	DonneesImageRGB *image_courante;
	char imagePath [512];
	if( lecture != NULL)
	{
		if (strcmp("..", lecture->d_name) != 0 && strcmp(".", lecture->d_name) != 0) // prend pas en compte le dossier parent et les fichiers chachés
		{
			strcpy(*nomImageVideo, lecture->d_name);
			strcpy(imagePath,folderPath);
			strcat(imagePath, "/");
			strcat(imagePath, *nomImageVideo);
			image_courante = lisBMPRGB(imagePath);
			return image_courante;
		}
	}
	return NULL;
}


int extraitCourbesSquelettesDossier(char* nomDossier, int nbImages, char* dateHeure, double *longueurBras, double *longueurJambe, bool *marcheNormale, char *boite)
{
	Mat imgRef,image, hsv, mask;
    string image_name;
	string image_name_save; // Ajout pour test ecriture dossier
	string nomDossierString(nomDossier);
	string dateHeureString(dateHeure);
	string cheminDossierCourbes(CHEMIN_DOSSIER_COURBES);
	string cheminDossierVideos(CHEMIN_DOSSIER_VIDEO);
	string cheminDossierSquelettes(CHEMIN_DOSSIER_SQUELETTES);
	
	string pathDirCourbes = cheminDossierCourbes + "/" + dateHeureString;
	mkdir(pathDirCourbes.c_str(), ACCESSPERMS);
	
	string pathDirSquelettes = cheminDossierSquelettes + "/" + dateHeureString;
	mkdir(pathDirSquelettes.c_str(), ACCESSPERMS);
	
    int image_num = 0;

    // Red mask - front side
    MaskHSV RedMask = MaskHSV(Mask(0,10), Mask(100,255), Mask(100,255));

    // Green mask - hip
    MaskHSV GreenMask = MaskHSV(Mask(70,90), Mask(100,255), Mask(30,255));

    // Blue mask - hidden side
    MaskHSV BlueMask = MaskHSV(Mask(95,125), Mask(173,255), Mask(40,255));

    // Yellow mask - Head
    MaskHSV YellowMask = MaskHSV(Mask(15,30), Mask(150,255), Mask(70,255));


    
    int l_h ,u_h, l_s ,u_s ,l_v, u_v ;

    Point hanchePrecedent(-1,-1), hip(-1,-1);
    Point tetePrecedent(-1,-1), head(-1,-1);

    Point piedRougePrecedent(-1,-1), piedRouge;     Point piedBleuPrecedent(-1,-1), piedBleu;
    Point genouRougePrecedent(-1,-1), genouRouge;   Point genouBleuPrecedent(-1,-1), genouBleu;
    Point mainRougePrecedent(-1,-1), mainRouge;     Point mainBleuPrecedent(-1,-1), mainBleu;
    Point coudeRougePrecedent(-1,-1), coudeRouge;   Point coudeBleuPrecedent(-1,-1), coudeBleu;
    Point epauleRougePrecedent(-1,-1), epauleRouge;
    
    
    int yminPiedRouge = 200, ymaxPiedRouge = 230;    int yminPiedBleu = 200, ymaxPiedBleu = 230; 
    int yminGenouRouge = 170, ymaxGenouRouge = 200;  int yminGenouBleu = 160, ymaxGenouBleu = 190;
    int yminMainRouge = 130, ymaxMainRouge = 170;    int yminMainBleu = 130, ymaxMainBleu = 160;
    int yminCoudeRouge = 105, ymaxCoudeRouge = 130;  int yminCoudeBleu = 105, ymaxCoudeBleu = 130;
    int yminEpauleRouge = 50, ymaxEpauleRouge = 105;

    enum {Rouge,Vert,Bleu,Jaune} Masque = Rouge;


    imgRef = imread(cheminDossierVideos + "/" + nomDossierString + "/01.bmp");
    if( ! imgRef.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( imgRef.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;
    double longueurJambeDroite;
    double sommeLongueurJambeDroite = 0; int nb_sommes_longueurJambeDroite = 0;
    double longueurBrasDroit;
    double sommeLongueurBrasDroit = 0; int nb_sommes_longueurBrasDroit = 0;
    
    vector<Point> posPiedsRouges, posPiedsBleus;
	int nbPiedRougeDown = 0;
    vector<Point> footRedCycle, footBlueCycle;
    vector< vector<Point> > cyclesPiedRouge, cyclesPiedBleu;
    bool piedBleuFirstTimeDown = false;
    bool piedRougeFirstTimeDown = false;

    int nbRougeBoite = 0;
	int nbBleuBoite = 0;

    for (image_num =1; image_num<=nbImages; image_num++)
    {
        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = cheminDossierVideos + "/" + nomDossierString + "/0" + image_name + ".bmp";
        else image_name = cheminDossierVideos + "/" + nomDossierString + "/" + image_name + ".bmp";
        //printf("image name = %s\n", image_name.c_str());
        image = imread(image_name);

        // Check if image loaded successfully
        if( ! image.data ){
            cout << "Error loading image " + image_name << endl;
            break;
        } 

        cvtColor(image, hsv ,COLOR_BGR2HSV);

        vector<Point> centresPastillesRouges,centrePastilleVerte,centresPastillesBleues,centrePastilleJaune;
        MaskHSV MaskForLoop = RedMask;
        Mat canny_output;
        do
        {
            l_h = MaskForLoop.maskH().min();
            u_h = MaskForLoop.maskH().max();
            l_s = MaskForLoop.maskS().min();
            u_s = MaskForLoop.maskS().max();
            l_v = MaskForLoop.maskV().min();
            u_v = MaskForLoop.maskV().max();

            inRange(hsv, Scalar(l_h,l_s,l_v), Scalar(u_h,u_s,u_v), mask); 
            blur( mask, mask, Size(3,3) );       // ou GaussianBlur(mask,mask,Size(5,5),0); en changeant la taille de la fenetre de floutage
       
            /// Find contours
            Canny( mask, canny_output, 100, 100*2 );
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE ); //EXTERNAL pour un contour exterieur

            vector<Point> centres;
            Rect rect = Rect(0, 0, canny_output.size().width, canny_output.size().height);
            for( size_t i = 0; i< contours.size(); i++ ) // on recupere les centres des contours
            {
                Moments m = moments(contours[i]);
                Point p = Point(m.m10/m.m00, m.m01/m.m00);
                // on verifie que le centre calculé est bien dans l'image
                if( rect.contains(p) )
                    centres.push_back(p);
            }



    // ici on a les centres des contours, mais il peut y avoir deux contours pour une meme pastille, donc 2 centres tres proches
    // on veut obtenir un tableau de centres rectifié

            vector<Point> bons_centres;
            for (size_t i = 0; i < centres.size(); i++)
            {
                int cote = 20;
                Rect zone = Rect(centres[i].x - cote/2, centres[i].y - cote/2, cote, cote);

                // on doit verifier que la zone ne contient pas d'autres centres
                // si il y a un autre centre on le supprime
                vector<Point> centres_proches;
                for (size_t j = 0; j < centres.size(); j++)
                {
                    if(j != i)
                    {
                        if( zone.contains(centres[j]) )
                        {
                            centres_proches.push_back(centres[j]);
                            centres.erase(centres.begin()+j);
                        }
                    }
                }
                if( !centres_proches.empty() )
                {
                    for (size_t l = 0; l < centres_proches.size(); l++)
                        centres[i] += centres_proches[l];
                    
                    centres[i].x /= centres_proches.size()+1;
                    centres[i].y /= centres_proches.size()+1;
                }
            }
            

            vector<Point> *centresPastilles;
            if(Masque == Rouge) 
            {
                centresPastilles = &centresPastillesRouges;
                MaskForLoop = BlueMask;
                Masque = Bleu;
            }
            else if(Masque == Bleu) 
            {
                centresPastilles = &centresPastillesBleues;
                MaskForLoop = GreenMask;
                Masque = Vert;
            }
            else if(Masque == Vert) 
            {
                centresPastilles = &centrePastilleVerte;
                MaskForLoop = YellowMask;
                Masque = Jaune;
            }
            else if(Masque == Jaune) 
            {
                centresPastilles = &centrePastilleJaune;
                MaskForLoop = RedMask;
                Masque = Rouge;
            }
            for (size_t i = 0; i < centres.size(); i++) // enregistre les centres des pastilles de chaque couleur
            {
                centresPastilles->push_back(centres[i]);
            }
            
        } while (Masque != Rouge);


        // ici on a les bons centres des pastilles 

        piedBleu = piedRouge = genouBleu = genouRouge = hip = mainBleu = mainRouge = coudeBleu = coudeRouge = epauleRouge = head = Point(-1,-1);

        // on associe chaque centre à son articulation et on trace leur trajectoire
        // on commence par la pastille verte
        Scalar color(0,199,0);
        if(!centrePastilleVerte.empty())
            ifPointinRange_drawLine(centrePastilleVerte[0],0,500,&hip,&hanchePrecedent,&drawing2,color);
        Mat dessinCourbes = drawing2.clone();
        if(hip.x != -1 && hip.y != -1)
            writePointwithTexttoMat(hip,"hip",&dessinCourbes,Scalar(0,255,0));

        // Puis la pastille jaune
        color = Scalar(0,199,199);
        if(!centrePastilleJaune.empty())
            ifPointinRange_drawLine(centrePastilleJaune[0],0,500,&head,&tetePrecedent,&drawing2,color);
        if(head.x != -1 && head.y != -1)
            writePointwithTexttoMat(head,"head",&dessinCourbes,Scalar(0,255,255));


        //Puis les pastilles rouges
        color = Scalar(0,0,199);
        if(!centresPastillesRouges.empty())
            identifyArticulationsfromRedPoints(centresPastillesRouges,yminPiedRouge,ymaxPiedRouge,&piedRouge,&piedRougePrecedent,yminGenouRouge,ymaxGenouRouge,&genouRouge,&genouRougePrecedent, yminMainRouge,ymaxMainRouge,&mainRouge,&mainRougePrecedent, yminCoudeRouge,ymaxCoudeRouge,&coudeRouge,&coudeRougePrecedent, yminEpauleRouge,ymaxEpauleRouge,&epauleRouge,&epauleRougePrecedent,&drawing2,color);

        // On affiche les articulations rouges sur l'image

        if(piedRouge.x != -1 && piedRouge.y != -1)
            writePointwithTexttoMat(piedRouge,"foot",&dessinCourbes,Scalar(0,0,255));

        if(genouRouge.x != -1 && genouRouge.y != -1)
            writePointwithTexttoMat(genouRouge,"knee",&dessinCourbes,Scalar(0,0,255));

        if(mainRouge.x != -1 && mainRouge.y != -1)
            writePointwithTexttoMat(mainRouge,"hand",&dessinCourbes,Scalar(0,0,255));
        
        if(coudeRouge.x != -1 && coudeRouge.y != -1)
            writePointwithTexttoMat(coudeRouge,"elbow",&dessinCourbes,Scalar(0,0,255));

        if(epauleRouge.x != -1 && epauleRouge.y != -1)
            writePointwithTexttoMat(epauleRouge,"shoulder",&dessinCourbes,Scalar(0,0,255));

        //imshow("Courbes pastilles rouges", dessinCourbes);
        

        // Puis les articulation correspondant aux pastilles bleues, et on trace leur trajectoire
        color = Scalar(199,0,0);
        if(!centresPastillesBleues.empty())
            identifyArticulationsfromBluePoints(centresPastillesBleues,yminPiedBleu,ymaxPiedBleu,&piedBleu,&piedBleuPrecedent,yminGenouBleu,ymaxGenouBleu,&genouBleu,&genouBleuPrecedent, yminMainBleu,ymaxMainBleu,&mainBleu,&mainBleuPrecedent, yminCoudeBleu,ymaxCoudeBleu,&coudeBleu,&coudeBleuPrecedent,&drawing2,color);

        // Puis on affiche les articulations sur l'image
        
       // Mat dessinCourbes = drawing2.clone();
        if(piedBleu.x != -1 && piedBleu.y != -1)
            writePointwithTexttoMat(piedBleu,"foot",&dessinCourbes,Scalar(255,0,0));

        if(genouBleu.x != -1 && genouBleu.y != -1)
            writePointwithTexttoMat(genouBleu,"knee",&dessinCourbes,Scalar(255,0,0));

        if(mainBleu.x != -1 && mainBleu.y != -1)
            writePointwithTexttoMat(mainBleu,"hand",&dessinCourbes,Scalar(255,0,0));
        
        if(coudeBleu.x != -1 && coudeBleu.y != -1)
            writePointwithTexttoMat(coudeBleu,"elbow",&dessinCourbes,Scalar(255,0,0));


		// Ajout ecriture dossier test
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/0" + image_name_save + ".bmp";
        else image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/" + image_name_save + ".bmp";
		imwrite(image_name_save, dessinCourbes);
		// Fin de l'ajout
		
		
		 // Maintenant qu'on a toutes les articulations définies, on peut tracer le squelette
        Mat dessinSquelette = Mat::zeros( image.size(), CV_8UC3 );
        drawSqueletton(&dessinSquelette, piedRouge,piedBleu,genouRouge,genouBleu,hip,mainRouge,mainBleu,coudeRouge,coudeBleu,epauleRouge,head);
        //imshow("Squelette", dessinSquelette);
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = cheminDossierSquelettes + "/" + dateHeureString + "/0" + image_name_save + ".bmp";
        else image_name_save = cheminDossierSquelettes + "/" + dateHeureString + "/" + image_name_save + ".bmp";
		imwrite(image_name_save, dessinSquelette);


        // On peut aussi calculer la longeur de membres
        longueurJambeDroite = longeurMembre(piedRouge, genouRouge, hip);
        if(longueurJambeDroite != -1)
        {
            sommeLongueurJambeDroite += longueurJambeDroite;
            nb_sommes_longueurJambeDroite ++;
        }
        longueurBrasDroit = longeurMembre(mainRouge, coudeRouge, epauleRouge);
        if(longueurBrasDroit != -1)
        {
            sommeLongueurBrasDroit += longueurBrasDroit;
            nb_sommes_longueurBrasDroit ++;
        }

        // On doit pouvoir comparer un cycle moyen avec un cycle moyen de reference
        // Donc d'abord il faut savoir enregistrer un cycle, pour ça on enregistre la position des deux pieds tant qu'on a pas fait un cycle
        // Puis on recommence
        if(piedRouge.x != -1 && piedRouge.y != -1)
            footRedCycle.push_back(piedRouge);
        if(piedBleu.x != -1 && piedBleu.y != -1)
            footBlueCycle.push_back(piedBleu);


        // Une fois qu'on a enregistré les cycles,il faut en faire un cycle moyen, qu'on peut comparer avec le cycle moyen de ref

        // pour detecter un cycle, il faut savoir detecter un pied a terre, donc recuperer la position d'un pied dans plusieurs images
        managePointVector(piedRouge,&posPiedsRouges, NB_IMAGES_DETECT_FOOT_DOWN);
        managePointVector(piedBleu,&posPiedsBleus, NB_IMAGES_DETECT_FOOT_DOWN);

		// Detection boitage
		if (piedRougeFirstTimeDown)
		{
			if(!posPiedsRouges.empty())
			{
				if(detectFootDown(posPiedsRouges, NB_IMAGES_DETECT_FOOT_DOWN))
				{
					nbRougeBoite++;
				}
			}
			
			if(!posPiedsBleus.empty())
			{
				if(detectFootDown(posPiedsBleus, NB_IMAGES_DETECT_FOOT_DOWN))
				{
					nbBleuBoite++;
				}
			}
		}
		cout << endl << "Bleu pose : " << nbBleuBoite;
		cout << " | Rouge pose :" << nbRougeBoite << endl;
		
		// Fin detection boitage


        if( !posPiedsRouges.empty() && !posPiedsBleus.empty())
        {
            // d'abord on cherche la pose du pied rouge, et qd il s'est posé on cherche la pose du pied bleu
            // puis on recommence
            if(nbPiedRougeDown == 0 && piedRouge.x != -1 && piedRouge.y != -1) 
            {
                cout <<"cherche pose pied rouge"<<endl;
                if( detectFootDown(posPiedsRouges, NB_IMAGES_DETECT_FOOT_DOWN) )
                {
                    cout<<"foot red down"<<endl;
                    (nbPiedRougeDown) ++;
                    if(piedRougeFirstTimeDown)
                        cyclesPiedRouge.push_back(footRedCycle);
                    else piedRougeFirstTimeDown = true;
                    footRedCycle.clear(); 
                    posPiedsRouges.clear();
                }
            }
            if(nbPiedRougeDown == 1 && piedBleu.x != -1 && piedBleu.y != -1)
            {
                cout <<"cherche pose pied bleu "<<posPiedsBleus.size()<<endl;
                if( detectFootDown(posPiedsBleus, NB_IMAGES_DETECT_FOOT_DOWN) )
                {
                    cout<<"foot blue down\n"<<endl;
                    nbPiedRougeDown = 0;
                    if(piedBleuFirstTimeDown)
                        cyclesPiedBleu.push_back(footBlueCycle);
                    else piedBleuFirstTimeDown = true;
                    footBlueCycle.clear(); 
                    posPiedsBleus.clear();
                }
            } 
        }
	}

    *longueurBras = sommeLongueurBrasDroit/nb_sommes_longueurBrasDroit;
    *longueurJambe = sommeLongueurJambeDroite/nb_sommes_longueurJambeDroite;
    cout << "longeur jambe droite moyenne = "<< *longueurJambe <<endl; 
    cout << "longeur bras droit moyen = "<< *longueurBras <<endl; 

	if(cyclesPiedBleu.size() > cyclesPiedRouge.size())
        cyclesPiedBleu.resize(cyclesPiedRouge.size());

    // Donc on peut faire la moyenne des cycles

    // pour faire cette moyenne il faut deja que tous les cycles aient le même point de depart,
    // donc on ramene le premier point de chaque cycle à 0 en x
    //cyclesPiedRouge.shrink_to_fit();
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        // pour chaque cycle on calcule l'écart entre l'origine et le premier point 
        if(!cyclesPiedRouge[i].empty())
        {   
            int ecart = cyclesPiedRouge[i][0].x;
            for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
            {
                // pour chaque point du cycle on enleve l'ecart
                cyclesPiedRouge[i][j].x -= ecart;
            }
            if(i < cyclesPiedBleu.size())
            {
                for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
                {
                    // pour chaque point du cycle on enleve l'ecart
                    cyclesPiedBleu[i][j].x -= ecart;
                    if(cyclesPiedBleu[i][j].x < 0)
                        cyclesPiedBleu[i].erase(cyclesPiedBleu[i].begin() + j);
                }
            }
        }
    }

    // ici on a les cycles ramenés à l'origine
    // on doit donc trouver le point moyen en y pour chaque x
    // il faut donc additionner les y de chaque cycle pour un x donné et le diviser par le nombre de y trouvé
    // le plus simple serait de stocker à l'indice x tous les y donc unvecteur de vecteurs de double

    vector < vector <double> > tabPointsCyclesRouges = pointsToDouble(cyclesPiedRouge);
    vector < vector <double> > tabPointsCyclesBleus = pointsToDouble(cyclesPiedBleu);
    
    vector<double> cycleRougeMoyen = meanVector(tabPointsCyclesRouges);
    vector<double> cycleBleuMoyen = meanVector(tabPointsCyclesBleus);

    // recuperation du cycle normal enregistré dans deux fichier
    vector<double> cycleRougeNormal = getNormalmeanVector("../opencv_files/cycleRougeNormal.txt");
    vector<double> cycleBleuNormal = getNormalmeanVector("../opencv_files/cycleBleuNormal.txt");

    // on convertit les tableaux de double en tableaux de points, soit les quelques points des courbes des cycles
    vector<Point> pointsCycleRougeNormal = doubleToPoints(cycleRougeNormal);
    vector<Point> pointsCycleBleuNormal = doubleToPoints(cycleBleuNormal);
    vector<Point> pointsCycleRougeMoyen = doubleToPoints(cycleRougeMoyen);
    vector<Point> pointsCycleBleuMoyen = doubleToPoints(cycleBleuMoyen);
    
     // Quand on a les quelques points caracterisant le cycle normal et le celui de la video,
    // il faut qu'ils aient la même longueur pour être comparés
    // soit si le cycle moyen fait 30 points, le cycle normal doit aussi faire 30 points
    if(pointsCycleRougeNormal.size() != pointsCycleRougeMoyen.size())
    {
        cout << "taille cycle rouge diff: "<<pointsCycleRougeNormal.size() <<" "<< pointsCycleRougeMoyen.size()<<endl;
        pointsCycleRougeNormal = adaptSizeVector(pointsCycleRougeNormal,pointsCycleRougeMoyen.size());

    }
    else cout << "meme longueur cycle rouge "<<endl;

    if(pointsCycleBleuNormal.size() != pointsCycleBleuMoyen.size())
    {
        cout << "taille cycle bleu diff: "<<pointsCycleBleuNormal.size() <<" "<< pointsCycleBleuMoyen.size()<<endl;
        pointsCycleBleuNormal = adaptSizeVector(pointsCycleBleuNormal,pointsCycleBleuMoyen.size());
    }
    else cout << "meme longueur cycle bleu "<<endl;


    // creation des courbes complètes à partir des quelques points
   vector<Point> pointsCycleRougeNormalFull = fillVectorPoints(pointsCycleRougeNormal, imgRef);
   vector<Point> pointsCycleBleuNormalFull = fillVectorPoints(pointsCycleBleuNormal, imgRef);
   vector<Point> pointsCycleRougeMoyenFull = fillVectorPoints(pointsCycleRougeMoyen, imgRef);
   vector<Point> pointsCycleBleuMoyenFull = fillVectorPoints(pointsCycleBleuMoyen, imgRef);

    double erreur_quad_rouge = mean_quadratic_error(pointsCycleRougeNormalFull,pointsCycleRougeMoyenFull);
    cout <<"erreur quad moyenne cycle rouge = "<< erreur_quad_rouge<<endl<<endl;
    double erreur_quad_bleue = mean_quadratic_error(pointsCycleBleuNormalFull,pointsCycleBleuMoyenFull);
    cout <<"erreur quad moyenne cycle bleu = "<< erreur_quad_bleue<<endl;

    if( erreur_quad_bleue > 25 || erreur_quad_rouge > 25)
	{
        cout << "anomalie dans la demarche"<<endl;
		*marcheNormale = false;
	}
	else
	{
        cout << "demarche normale par comparaison de cycles"<<endl;
		*marcheNormale = true;
	}
	
	double ecartBoite = fabs(nbRougeBoite - nbBleuBoite); // Valeur absolue de la difference pour mesurer l'ecart de vitesse
	if (ecartBoite < 2.0) // Mesure d'apres quelques observations a remplacer peut etre par 3, 4 ,5 ...
	{
		strcpy(boite, "No");
        cout << "Boite pas"<<endl;
	}
	else
	{
		if (nbRougeBoite > nbBleuBoite)
		{
			strcpy(boite, "Left");
            cout<<"boite left"<<endl;
		}
		else
		{
			strcpy(boite, "Right");
            cout << "boite right"<<endl;
		}
	}
    

	return 1;
}

int changeNomDossier(char* nomActuel, char* prenomActuel, char* modifNom, char *modifPrenom)
{
	char filepath[128];
	sprintf(filepath, "%s/%s_%s", CHEMIN_DOSSIER_PATIENT, prenomActuel, nomActuel); // On remple filepath avec le chemin et nom du fichier
	if (modifNom == NULL || modifPrenom == NULL || strcmp(modifNom, "") == 0 || strcmp(modifPrenom, "") == 0)
		return -1;
		
	char filepath_new[128];
	sprintf(filepath_new, "%s/%s_%s", CHEMIN_DOSSIER_PATIENT, modifPrenom, modifNom); // On remple filepath avec le chemin et nom du fichier
	
	return rename(filepath, filepath_new);
}

void evaluePathologies(char* nomVideo, char* courbe, char* marcheRegu, char* boite)
{
	char c = '0';
	char mr = '0';
	char b = '0';
	
	sscanf(nomVideo, "video%c%c%c_", &c, &mr, &b);
	
	if (c == '1')
		strcpy(courbe, "Yes");
	else
		strcpy(courbe, "No");
		
	if (mr == '1') // MarcheRegu = Inverse de rythme
		strcpy(marcheRegu, "No");
	else
		strcpy(marcheRegu, "Yes");
		
	if (b == '1')
		strcpy(boite, "Yes");
	else
		strcpy(boite, "No");
}
