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


    imgRef = imread("./learning_videos/courbe0rythme0boite0-1/01.bmp"); // pour récupérer la taille des images de la video
    if( ! imgRef.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( imgRef.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;
    vector<Point> posPiedsRouges, posPiedsBleus;

    int nbPiedRougeDown = 0;
    int nbPiedBleuDown = 0;
    vector<Point> footRedCycle, footBlueCycle;
    vector< vector<Point> > cyclesPiedRouge, cyclesPiedBleu;
    bool piedRougeFirstTimeDown = false;

    while(1)
    {
        if(mode == Play)
            image_num++;

        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = "./learning_videos/courbe0rythme0boited-1/0" + image_name + ".bmp";
        else image_name = "./learning_videos/courbe0rythme0boited-1/" + image_name + ".bmp";
        //printf("image name = %s\n", image_name.c_str());
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


        // On doit pouvoir comparer un cycle moyen avec un cycle moyen de reference
        // Donc d'abord il faut savoir enregistrer un cycle, pour ça on enregistre la position des deux pieds tant qu'on a pas fait un cycle
        // Puis on recommence
        footRedCycle.push_back(piedRouge);
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

        if(!posPiedsRouges.empty())
        {
            if(detectFootDown(posPiedsRouges, NB_IMAGES_DETECT_FOOT_DOWN) && !piedRougeFirstTimeDown) // enleve les premiers points avant le premier cycle
            {
                piedRougeFirstTimeDown = true;
                footRedCycle.clear();
            }
        }

        if(!posPiedsRouges.empty() && !posPiedsBleus.empty())
        {
            if( detectGaitCycle(&nbPiedRougeDown, posPiedsRouges, &nbPiedBleuDown,posPiedsBleus) )
            {
                cout<<"Cycle fait!\n"<<endl;
                cyclesPiedRouge.push_back(footRedCycle); // enregistre le cycle dans un tableau de tableaux de points
                cyclesPiedBleu.push_back(footBlueCycle);
                footRedCycle.clear(); // supprime le cycle pour en enregistrer un nouveau
                footBlueCycle.clear();
                cout << "nb cycles pbleu enregistrés: "<<cyclesPiedRouge.size()<<endl;
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


/*         Mat dessinfinal = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedRouge[i][j] ,2, Scalar(0,0,255), -1);
        }
        for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedBleu[i][j] ,2, Scalar(255,0,0), -1);
        }
    }
    
    imshow("tous les cycles rouges", dessinfinal);

    waitKey(0);
    destroyAllWindows(); */

    
    // Donc on peut faire la moyenne des cycles

    // pour faire cette moyenne il faut deja que tous les cycles aient le même point de depart,
    // donc on ramene le premier point de chaque cycle à 0 en x
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        // pour chaque cycle on calcule l'écart entre l'origine et le premier point 
        int ecart = cyclesPiedRouge[i][0].x;
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            // pour chaque point du cycle on enleve l'ecart
            cyclesPiedRouge[i][j].x -= ecart;
        }
        for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
        {
            // pour chaque point du cycle on enleve l'ecart
            cyclesPiedBleu[i][j].x -= ecart;
        }
    }

    Mat dessinfinal = Mat::zeros( imgRef.size(), CV_8UC3 );
    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedRouge[i][j] ,2, Scalar(0,0,255), -1);
        }
        for (size_t j = 0; j < cyclesPiedBleu[i].size(); j++)
        {
            circle(dessinfinal, cyclesPiedBleu[i][j] ,2, Scalar(255,0,0), -1);
        }
    }
    
    imshow("tous les cycles rouges", dessinfinal);

    waitKey(0);
    destroyAllWindows();    

    // ici on a les cycles ramenés à l'origine
    // on doit donc trouver le point moyen en y pour chaque x
    // il faut donc additionner les y de chaque cycle pour un x donné et le diviser par le nombre de y trouvé
    // le plus simple serait de stocker à l'indice x tous les y donc unvecteur de vecteurs de double
    vector < vector <double> > tabPointsCyclesRouges;

    for (size_t i = 0; i < cyclesPiedRouge.size(); i++)
    {
        for (size_t j = 0; j < cyclesPiedRouge[i].size(); j++)
        {
            // pour chaque point de chaque cycle on doit enregistrer, à l'indice x, l'ordonnee y 
            if((long unsigned int)cyclesPiedRouge[i][j].x+1 > tabPointsCyclesRouges.size() && cyclesPiedRouge[i][j].y >= 0 )
            {
                cout << "resize to "<< cyclesPiedRouge[i][j].x+1 <<endl;
                tabPointsCyclesRouges.resize((long unsigned int)cyclesPiedRouge[i][j].x+1);
            }
            if(cyclesPiedRouge[i][j].y >= 0)
            {
                cout << "enter y = "<<cyclesPiedRouge[i][j].y<<endl ;
                tabPointsCyclesRouges[ (long unsigned int)cyclesPiedRouge[i][j].x ].push_back( cyclesPiedRouge[i][j].y );
            }  
        }
    }
    


    // un cycle contient plusieurs points, ces points sont séparés, une ligne permet donc d'avoir tous les points
    // on doit enregistrer ces points dans les vecteurs pour avoir des cycles complets et pouvoir comparer en y

    return 0;
  }

 