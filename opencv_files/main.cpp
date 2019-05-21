//#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

#include "Mask.h"
#include "imageProcessing.h"
 
using namespace std;
using namespace cv;


int main(void)
{
    Mat image, hsv, mask;
    string image_name;
	string image_name_save; // Ajout pour test ecriture dossier
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

    enum {Play, Pause} mode = Play; 
    enum {Rouge,Vert,Bleu,Jaune} Masque = Rouge;


    image = imread("./learning_videos/courbe0rythme0boited-1/01.bmp");
    if( ! image.data )
    {
        cout << "Error loading image " << endl;
        return -1;
    } 
    Mat drawing2 = Mat::zeros( image.size(), CV_8UC3 );

    vector<Point> centresPastillesRougesImagePrecedente;

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
            return -1;
        } 
        imshow("Original", image);

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

        imshow("Courbes pastilles bleues", drawing3);
		
		// Ajout ecriture dossier test
		image_name_save = to_string(image_num);
		if(image_num < 10)
            image_name_save = "./test/courbe0rythme0boited-1/0" + image_name_save + ".bmp";
        else image_name_save = "./test/courbe0rythme0boited-1/" + image_name_save + ".bmp";
		imwrite(image_name_save, drawing3);
		// Fin de l'ajout
        
        
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

    return 0;
  }

 
