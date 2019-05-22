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
int creeAnalysePatient(char* nom, char* prenom, char* taille, char* poids, char* courbe, char* claudification, char* marcheRegu, char* video, int nbImages, char* dateHeure)
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
	
	// On cree les champs caracteristiques
	
	// On cree les futurs champs
	fprintf(fiche, "Taille:%s\n", taille);
	fprintf(fiche, "Poids:%s\n", poids);
	fprintf(fiche, "Courbe:%s\n", courbe);
	fprintf(fiche, "Claudification:%s\n", claudification);
	fprintf(fiche, "Marche:%s\n", marcheRegu);
	fprintf(fiche, "Video:%s\n", video);
	
	fclose(fiche);
	
	int extract = extraitCourbesSquelettesDossier(video, nbImages, dateChaine);
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
		else if (strcmp(cle, "Courbe") == 0)
		{
			strcpy(a.courbe, valeur); 
		}
		else if (strcmp(cle, "Claudification") == 0) 
		{
			strcpy(a.boite, valeur); 
		}
		else if (strcmp(cle, "Marche") == 0) 
		{
			strcpy(a.marcheReguliere, valeur); 
		}
		else if (strcmp(cle, "Video") == 0) 
		{
			strcpy(a.video, valeur); 
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

int extraitCourbesDossier(char* nomDossier, int nbImages, char* dateHeure)
{
	Mat image, hsv, mask;
    string image_name;
	string image_name_save; // Ajout pour test ecriture dossier
	string nomDossierString(nomDossier);
	string dateHeureString(dateHeure);
	string cheminDossierCourbes(CHEMIN_DOSSIER_COURBES);
	string cheminDossierVideos(CHEMIN_DOSSIER_VIDEO);
	string pathDir = cheminDossierCourbes + "/" + dateHeureString;
	mkdir(pathDir.c_str(), ACCESSPERMS);
    int image_num = 0;

    // Red mask - front side
    MaskHSV RedMask = MaskHSV(Mask(0,10), Mask(100,255), Mask(100,255));

    // Green mask - hanche
    MaskHSV GreenMask = MaskHSV(Mask(70,90), Mask(100,255), Mask(30,255));

    // Blue mask - hidden side
    MaskHSV BlueMask = MaskHSV(Mask(95,125), Mask(173,255), Mask(40,255));

    // Yellow mask - Head
    MaskHSV YellowMask = MaskHSV(Mask(15,30), Mask(150,255), Mask(70,255));


    
    int l_h ,u_h, l_s ,u_s ,l_v, u_v ;

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

	image = imread(cheminDossierVideos + "/" + nomDossierString + "/01.bmp");
    if( ! image.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( image.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;

    for (image_num =1; image_num<=nbImages; image_num++)
    {
        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = cheminDossierVideos + "/" + nomDossierString + "/0" + image_name + ".bmp";
        else image_name = cheminDossierVideos + "/" + nomDossierString + "/" + image_name + ".bmp";
        //printf("image name = %s\n", image_name.c_str());
        image = imread(image_name);
        if( ! image.data )
        {
            cout << "Error loading image " << cheminDossierVideos << endl << image_name << endl;
			//rmdir(pathDir.c_str());
            //return -1;
            continue; // On met continue car sinon, la video dans le dossier declenche le return -1. A modifier si besoin (et temps)
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

        // on associe chaque centre à son articulation et on trace leur trajectoire
        Scalar color(0,0,199);
        identifyArticulationsfromRedPoints(centresPastillesRouges,yminPiedRouge,ymaxPiedRouge,&piedRouge,&piedRougePrecedent,yminGenouRouge,ymaxGenouRouge,&genouRouge,&genouRougePrecedent, yminMainRouge,ymaxMainRouge,&mainRouge,&mainRougePrecedent, yminCoudeRouge,ymaxCoudeRouge,&coudeRouge,&coudeRougePrecedent, yminEpauleRouge,ymaxEpauleRouge,&epauleRouge,&epauleRougePrecedent,&drawing2,color);



        // Puis on affiche les articulations sur l'image

        Mat drawing3 = drawing2.clone();
        if(piedRouge.x != -1 && piedRouge.y != -1)
            writePointwithTexttoMat(piedRouge,"pied",&drawing3,Scalar(0,0,255));

        if(genouRouge.x != -1 && genouRouge.y != -1)
            writePointwithTexttoMat(genouRouge,"genou",&drawing3,Scalar(0,0,255));

        if(mainRouge.x != -1 && mainRouge.y != -1)
            writePointwithTexttoMat(mainRouge,"main",&drawing3,Scalar(0,0,255));
        
        if(coudeRouge.x != -1 && coudeRouge.y != -1)
            writePointwithTexttoMat(coudeRouge,"coude",&drawing3,Scalar(0,0,255));

        if(epauleRouge.x != -1 && epauleRouge.y != -1)
            writePointwithTexttoMat(epauleRouge,"epaule",&drawing3,Scalar(0,0,255));

        //imshow("Courbes pastilles rouges", drawing3);
        

        // on associe chaque centre à son articulation et on trace leur trajectoire
        color = Scalar(199,0,0);
        identifyArticulationsfromBluePoints(centresPastillesBleues,yminPiedBleu,ymaxPiedBleu,&piedBleu,&piedBleuPrecedent,yminGenouBleu,ymaxGenouBleu,&genouBleu,&genouBleuPrecedent, yminMainBleu,ymaxMainBleu,&mainBleu,&mainBleuPrecedent, yminCoudeBleu,ymaxCoudeBleu,&coudeBleu,&coudeBleuPrecedent,&drawing2,color);

        // Puis on affiche les articulations sur l'image

       // Mat drawing3 = drawing2.clone();
        if(piedBleu.x != -1 && piedBleu.y != -1)
            writePointwithTexttoMat(piedBleu,"pied",&drawing3,Scalar(255,0,0));

        if(genouBleu.x != -1 && genouBleu.y != -1)
            writePointwithTexttoMat(genouBleu,"genou",&drawing3,Scalar(255,0,0));

        if(mainBleu.x != -1 && mainBleu.y != -1)
            writePointwithTexttoMat(mainBleu,"main",&drawing3,Scalar(255,0,0));
        
        if(coudeBleu.x != -1 && coudeBleu.y != -1)
            writePointwithTexttoMat(coudeBleu,"coude",&drawing3,Scalar(255,0,0));
		
		
		// Ajout ecriture dossier test
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/0" + image_name_save + ".bmp";
        else image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/" + image_name_save + ".bmp";
		imwrite(image_name_save, drawing3);
		// Fin de l'ajout
	}
	return 1;
}

int extraitCourbesSquelettesDossier(char* nomDossier, int nbImages, char* dateHeure)
{
	Mat image, hsv, mask;
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

    // Green mask - hanche
    MaskHSV GreenMask = MaskHSV(Mask(70,90), Mask(100,255), Mask(30,255));

    // Blue mask - hidden side
    MaskHSV BlueMask = MaskHSV(Mask(95,125), Mask(173,255), Mask(40,255));

    // Yellow mask - Head
    MaskHSV YellowMask = MaskHSV(Mask(15,30), Mask(150,255), Mask(70,255));


    
    int l_h ,u_h, l_s ,u_s ,l_v, u_v ;

    Point hanchePrecedent(-1,-1), hanche(-1,-1);
    Point tetePrecedent(-1,-1), tete(-1,-1);

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


    image = imread(cheminDossierVideos + "/" + nomDossierString + "/01.bmp");
    if( ! image.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( image.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;

    for (image_num =1; image_num<=nbImages; image_num++)
    {
        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = cheminDossierVideos + "/" + nomDossierString + "/0" + image_name + ".bmp";
        else image_name = cheminDossierVideos + "/" + nomDossierString + "/" + image_name + ".bmp";
        //printf("image name = %s\n", image_name.c_str());
        image = imread(image_name);
        if( ! image.data )
        {
            cout << "Error loading image " << endl;
            return -1;
        } 
        //imshow("Original", image);

        // Check if image loaded successfully
        if( ! image.data ){
            cout << "Error loading image " + image_name << endl;
            return -1;
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

        piedBleu = piedRouge = genouBleu = genouRouge = hanche = mainBleu = mainRouge = coudeBleu = coudeRouge = epauleRouge = tete = Point(-1,-1);

        // on associe chaque centre à son articulation et on trace leur trajectoire
        // on commence par la pastille verte
        Scalar color(0,199,0);
        if(!centrePastilleVerte.empty())
            ifPointinRange_drawLine(centrePastilleVerte[0],0,500,&hanche,&hanchePrecedent,&drawing2,color);
        Mat dessinCourbes = drawing2.clone();
        if(hanche.x != -1 && hanche.y != -1)
            writePointwithTexttoMat(hanche,"hanche",&dessinCourbes,Scalar(0,255,0));

        // Puis la pastille jaune
        color = Scalar(0,199,199);
        if(!centrePastilleJaune.empty())
            ifPointinRange_drawLine(centrePastilleJaune[0],0,500,&tete,&tetePrecedent,&drawing2,color);
        if(tete.x != -1 && tete.y != -1)
            writePointwithTexttoMat(tete,"tete",&dessinCourbes,Scalar(0,255,255));


        //Puis les pastilles rouges
        color = Scalar(0,0,199);
        if(!centresPastillesRouges.empty())
            identifyArticulationsfromRedPoints(centresPastillesRouges,yminPiedRouge,ymaxPiedRouge,&piedRouge,&piedRougePrecedent,yminGenouRouge,ymaxGenouRouge,&genouRouge,&genouRougePrecedent, yminMainRouge,ymaxMainRouge,&mainRouge,&mainRougePrecedent, yminCoudeRouge,ymaxCoudeRouge,&coudeRouge,&coudeRougePrecedent, yminEpauleRouge,ymaxEpauleRouge,&epauleRouge,&epauleRougePrecedent,&drawing2,color);

        // On affiche les articulations rouges sur l'image

        if(piedRouge.x != -1 && piedRouge.y != -1)
            writePointwithTexttoMat(piedRouge,"pied",&dessinCourbes,Scalar(0,0,255));

        if(genouRouge.x != -1 && genouRouge.y != -1)
            writePointwithTexttoMat(genouRouge,"genou",&dessinCourbes,Scalar(0,0,255));

        if(mainRouge.x != -1 && mainRouge.y != -1)
            writePointwithTexttoMat(mainRouge,"main",&dessinCourbes,Scalar(0,0,255));
        
        if(coudeRouge.x != -1 && coudeRouge.y != -1)
            writePointwithTexttoMat(coudeRouge,"coude",&dessinCourbes,Scalar(0,0,255));

        if(epauleRouge.x != -1 && epauleRouge.y != -1)
            writePointwithTexttoMat(epauleRouge,"epaule",&dessinCourbes,Scalar(0,0,255));

        //imshow("Courbes pastilles rouges", dessinCourbes);
        

        // Puis les articulation correspondant aux pastilles bleues, et on trace leur trajectoire
        color = Scalar(199,0,0);
        if(!centresPastillesBleues.empty())
            identifyArticulationsfromBluePoints(centresPastillesBleues,yminPiedBleu,ymaxPiedBleu,&piedBleu,&piedBleuPrecedent,yminGenouBleu,ymaxGenouBleu,&genouBleu,&genouBleuPrecedent, yminMainBleu,ymaxMainBleu,&mainBleu,&mainBleuPrecedent, yminCoudeBleu,ymaxCoudeBleu,&coudeBleu,&coudeBleuPrecedent,&drawing2,color);

        // Puis on affiche les articulations sur l'image
        
       // Mat dessinCourbes = drawing2.clone();
        if(piedBleu.x != -1 && piedBleu.y != -1)
            writePointwithTexttoMat(piedBleu,"pied",&dessinCourbes,Scalar(255,0,0));

        if(genouBleu.x != -1 && genouBleu.y != -1)
            writePointwithTexttoMat(genouBleu,"genou",&dessinCourbes,Scalar(255,0,0));

        if(mainBleu.x != -1 && mainBleu.y != -1)
            writePointwithTexttoMat(mainBleu,"main",&dessinCourbes,Scalar(255,0,0));
        
        if(coudeBleu.x != -1 && coudeBleu.y != -1)
            writePointwithTexttoMat(coudeBleu,"coude",&dessinCourbes,Scalar(255,0,0));


		// Ajout ecriture dossier test
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/0" + image_name_save + ".bmp";
        else image_name_save = cheminDossierCourbes + "/" + dateHeureString + "/" + image_name_save + ".bmp";
		imwrite(image_name_save, dessinCourbes);
		// Fin de l'ajout
		
		
		 // Maintenant qu'on a toutes les articulations définies, on peut tracer le squelette
        Mat dessinSquelette = Mat::zeros( image.size(), CV_8UC3 );
        drawSqueletton(&dessinSquelette, piedRouge,piedBleu,genouRouge,genouBleu,hanche,mainRouge,mainBleu,coudeRouge,coudeBleu,epauleRouge,tete);
        //imshow("Squelette", dessinSquelette);
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = cheminDossierSquelettes + "/" + dateHeureString + "/0" + image_name_save + ".bmp";
        else image_name_save = cheminDossierSquelettes + "/" + dateHeureString + "/" + image_name_save + ".bmp";
		imwrite(image_name_save, dessinSquelette);
	}
	return 1;
}
