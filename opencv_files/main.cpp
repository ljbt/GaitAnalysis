//#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

#include "Mask.h"
#include "imageProcessing.h"
#include "statistics.h"
#include "definitions.h"

using namespace std;
using namespace cv;


int main(void)
{
    Mat imgRef, image, hsv, mask;
    string image_name;
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

    enum {Play, Pause} mode = Play; 
    enum {Rouge,Vert,Bleu,Jaune} Masque = Rouge;


    imgRef = imread("./learning_videos/000_1/01.bmp"); // pour récupérer la taille des images de la video
    if( ! imgRef.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( imgRef.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;
    vector<Point> posPiedsRouges, posPiedsBleus;

    int nbPiedRougeDown = 0;
    vector<Point> footRedCycle, footBlueCycle;
    vector< vector<Point> > cyclesPiedRouge, cyclesPiedBleu;
    bool piedBleuFirstTimeDown = false;
    bool piedRougeFirstTimeDown = false;

    double longueurJambeDroite;
    double sommeLongueurJambeDroite = 0; int nb_sommes_longueurJambeDroite = 0;
    double longueurBrasDroit;
    double sommeLongueurBrasDroit = 0; int nb_sommes_longueurBrasDroit = 0;

    int nbRougeBoite = 0;
	int nbBleuBoite = 0;
    while(1)
    {
        if(mode == Play)
            image_num++;

        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = "./learning_videos/10G_6/0" + image_name + ".bmp";
        else image_name = "./learning_videos/10G_6/" + image_name + ".bmp";
        
        image = imread(image_name);
        if( ! image.data )
        {
            cout << "Error loading image " << endl;
            break;
        } 
        imshow("Original", image);

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

        piedBleu = piedRouge = genouBleu = genouRouge = hanche = mainBleu = mainRouge = coudeBleu = coudeRouge = epauleRouge = tete = Point(-1,-1);

        // on associe chaque centre à son articulation et on trace leur trajectoire
        // on commence par la pastille verte
        Scalar color(0,199,0);
        if(!centrePastilleVerte.empty())
            ifPointinRange_drawLine(centrePastilleVerte[0],0,500,&hanche,&hanchePrecedent,&drawing2,color);
        Mat dessinCourbes = drawing2.clone();
        if(hanche.x != -1 && hanche.y != -1)
            writePointwithTexttoMat(hanche,"hip",&dessinCourbes,Scalar(0,255,0));

        // Puis la pastille jaune
        color = Scalar(0,199,199);
        if(!centrePastilleJaune.empty())
            ifPointinRange_drawLine(centrePastilleJaune[0],0,500,&tete,&tetePrecedent,&drawing2,color);
        if(tete.x != -1 && tete.y != -1)
            writePointwithTexttoMat(tete,"head",&dessinCourbes,Scalar(0,255,255));


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

        imshow("Courbes pastilles", dessinCourbes);


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

        imshow("Courbes pastilles", dessinCourbes);


        // Maintenant qu'on a toutes les articulations définies, on peut tracer le squelette

        Mat dessinSquelette = Mat::zeros( image.size(), CV_8UC3 );
        drawSqueletton(&dessinSquelette, piedRouge,piedBleu,genouRouge,genouBleu,hanche,mainRouge,mainBleu,coudeRouge,coudeBleu,epauleRouge,tete);
        imshow("Squelette", dessinSquelette);



        // On peut aussi calculer la longeur de membres
        longueurJambeDroite = longeurJambe(piedRouge, genouRouge, hanche);
        if(longueurJambeDroite != -1)
        {
            sommeLongueurJambeDroite += longueurJambeDroite;
            nb_sommes_longueurJambeDroite ++;
        }
        longueurBrasDroit = longeurJambe(mainRouge, coudeRouge, epauleRouge);
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

        Mat drawing4 = Mat::zeros( image.size(), CV_8UC3 );
        for (size_t i = 0; i < footRedCycle.size(); i++)
        {
            circle(drawing4, footRedCycle[i] ,2, Scalar(0,0,255), -1);
        }
        for (size_t i = 0; i < footBlueCycle.size(); i++)
        {
            circle(drawing4, footBlueCycle[i] ,2, Scalar(255,0,0), -1);
        }        

        imshow("points cycles", drawing4);



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
        
        
        char c=(char)waitKey(125); // waits 125ms to get a key value
        if(c==27) // echap
            break;
        else if(c == ' ') // space
        {
            if(mode == Play)
                mode = Pause;
            
            else if(mode == Pause)
                mode = Play; 
        }
        else if(c == 'Q') // left arroy
        {
            mode = Pause;
            image_num--;
        }
        else if(c == 'S') // right arroy
        {        
            mode = Pause;
            image_num++;
        }
    }
    destroyAllWindows();
    // A la fin du while, toute la video est passée


    cout << "longeur jambe droite moyenne = "<<sommeLongueurJambeDroite/nb_sommes_longueurJambeDroite<<endl; 
    cout << "longeur bras droit moyen = "<<sommeLongueurBrasDroit/nb_sommes_longueurBrasDroit<<endl; 

    if(cyclesPiedBleu.size() > cyclesPiedRouge.size())
        cyclesPiedBleu.resize(cyclesPiedRouge.size());


    Mat dessin = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            circle(dessin, cyclesPiedRouge[i][j] ,2, Scalar(0,0,255), -1);
        }
    }
    for (size_t i = 0; i < cyclesPiedBleu.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
        {
            circle(dessin, cyclesPiedBleu[i][j] ,2, Scalar(255,0,0), -1);
        }
    }
    
    imshow("Cycles", dessin);

    
    
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


    Mat dessinfinal = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedRouge[i][j] ,2, Scalar(0,0,255), -1);
        }
    }
    for (size_t i = 0; i < cyclesPiedBleu.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedBleu[i][j] ,2, Scalar(255,0,0), -1);
        }
    }
    
    imshow("Cycles superposés", dessinfinal);

  

    // ici on a les cycles ramenés à l'origine
    // on doit donc trouver le point moyen en y pour chaque x
    // il faut donc additionner les y de chaque cycle pour un x donné et le diviser par le nombre de y trouvé
    // le plus simple serait de stocker à l'indice x tous les y donc unvecteur de vecteurs de double




    vector < vector <double> > tabPointsCyclesRouges = pointsToDouble(cyclesPiedRouge);
    vector < vector <double> > tabPointsCyclesBleus = pointsToDouble(cyclesPiedBleu);
    
    vector<double> cycleRougeMoyen = meanVector(tabPointsCyclesRouges);
    vector<double> cycleBleuMoyen = meanVector(tabPointsCyclesBleus);

 
    Mat imgcyclemoy = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < cycleRougeMoyen.size(); i++)
    {
        if(cycleRougeMoyen[i] > 0)
            circle(imgcyclemoy, Point(i,cycleRougeMoyen[i]) ,2, Scalar(0,0,255), -1);
        
    }
    for (size_t i = 0; i < cycleBleuMoyen.size(); i++)
    {
        if(cycleBleuMoyen[i] > 0)
            circle(imgcyclemoy, Point(i,cycleBleuMoyen[i]) ,2, Scalar(255,0,0), -1);
        
    }
    imshow("Cycle moyen", imgcyclemoy); 
    
    // recuperation du cycle normal enregistré dans deux fichier
    vector<double> cycleRougeNormal = getNormalmeanVector("cycleRougeNormal.txt");
    vector<double> cycleBleuNormal = getNormalmeanVector("cycleBleuNormal.txt");

    // on convertit les tableaux de double en tableaux de points, soit les quelques points des courbes des cycles
    vector<Point> pointsCycleRougeNormal = doubleToPoints(cycleRougeNormal);
    vector<Point> pointsCycleBleuNormal = doubleToPoints(cycleBleuNormal);
    vector<Point> pointsCycleRougeMoyen = doubleToPoints(cycleRougeMoyen);
    vector<Point> pointsCycleBleuMoyen = doubleToPoints(cycleBleuMoyen);
    
    Mat imgpointscyclenorm = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < pointsCycleRougeNormal.size(); i++)
    {
        if(pointsCycleRougeNormal[i].y > 0)
            circle(imgpointscyclenorm, pointsCycleRougeNormal[i] ,2, Scalar(0,255,255), -1);
        
    }
    for (size_t i = 0; i < pointsCycleBleuNormal.size(); i++)
    {
        if(pointsCycleBleuNormal[i].y > 0)
            circle(imgpointscyclenorm, pointsCycleBleuNormal[i] ,2, Scalar(255,255,0), -1);
        
    }
    imshow("points cycle normal", imgpointscyclenorm);

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




    Mat imgpointscyclenorm2 = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < pointsCycleRougeNormal.size(); i++)
    {
        if(pointsCycleRougeNormal[i].y > 0)
            circle(imgpointscyclenorm2, pointsCycleRougeNormal[i] ,2, Scalar(0,255,255), -1);
        
    }
    for (size_t i = 0; i < pointsCycleBleuNormal.size(); i++)
    {
        if(pointsCycleBleuNormal[i].y > 0)
            circle(imgpointscyclenorm2, pointsCycleBleuNormal[i] ,2, Scalar(255,255,0), -1);
        
    }
    imshow("points cycle normal adapt", imgpointscyclenorm2);

  


    // creation des courbes complètes à partir des quelques points
   vector<Point> pointsCycleRougeNormalFull = fillVectorPoints(pointsCycleRougeNormal, imgRef);
   vector<Point> pointsCycleBleuNormalFull = fillVectorPoints(pointsCycleBleuNormal, imgRef);
   vector<Point> pointsCycleRougeMoyenFull = fillVectorPoints(pointsCycleRougeMoyen, imgRef);
   vector<Point> pointsCycleBleuMoyenFull = fillVectorPoints(pointsCycleBleuMoyen, imgRef);

    Mat imgfullpointscyclenorm = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < pointsCycleRougeNormalFull.size(); i++)
    {
        if(pointsCycleRougeNormalFull[i].y > 0)
            circle(imgfullpointscyclenorm, pointsCycleRougeNormalFull[i] ,2, Scalar(0,255,255), -1);
        
    }
    for (size_t i = 0; i < pointsCycleRougeMoyenFull.size(); i++)
    {
        if(pointsCycleRougeMoyenFull[i].y > 0)
            circle(imgfullpointscyclenorm, pointsCycleRougeMoyenFull[i] ,2, Scalar(0,0,255), -1);
        
    }
    for (size_t i = 0; i < pointsCycleBleuNormalFull.size(); i++)
    {
        if(pointsCycleBleuNormalFull[i].y > 0)
            circle(imgfullpointscyclenorm, pointsCycleBleuNormalFull[i] ,2, Scalar(255,255,0), -1);
        
    }
    for (size_t i = 0; i < pointsCycleBleuMoyenFull.size(); i++)
    {
        if(pointsCycleBleuMoyenFull[i].y > 0)
            circle(imgfullpointscyclenorm, pointsCycleBleuMoyenFull[i] ,2, Scalar(255,0,0), -1);
        
    }
    imshow("full points cycle normal", imgfullpointscyclenorm);

    
    
    
    double erreur_quad_rouge = mean_quadratic_error(pointsCycleRougeNormalFull,pointsCycleRougeMoyenFull);
    cout <<"erreur quad moyenne cycle rouge = "<< erreur_quad_rouge<<endl<<endl;
    double erreur_quad_bleue = mean_quadratic_error(pointsCycleBleuNormalFull,pointsCycleBleuMoyenFull);
    cout <<"erreur quad moyenne cycle bleu = "<< erreur_quad_bleue<<endl;

    if( erreur_quad_bleue > 25 || erreur_quad_rouge > 25)
    {
        cout << "anomalie dans la demarche"<<endl;
    }


	double ecartBoite = fabs(nbRougeBoite - nbBleuBoite); // Valeur absolue de la difference pour mesurer l'ecart de vitesse
	if (ecartBoite < 2.0) // Mesure d'apres quelques observations a remplacer peut etre par 3, 4 ,5 ...
	{
		//strcpy(boite, "No");
        cout << "Boite pas"<<endl;
	}
	else
	{
		if (nbRougeBoite > nbBleuBoite)
		{
			//strcpy(boite, "Left");
            cout<<"boite left"<<endl;
		}
		else
		{
			//strcpy(boite, "Right");
            cout << "boite right"<<endl;
		}
	}

    // un cycle contient plusieurs points, ces points sont séparés, une ligne permet donc d'avoir tous les points
    // on doit enregistrer ces points dans les vecteurs pour avoir des cycles complets et pouvoir comparer en y
    waitKey(0);
    destroyAllWindows();  
    return 0;
  }

 