//#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

#include "Mask.h"
//#include "NNdefinitions.h"
#include "imageProcessing.h"
#include "fonctions.h"
 
using namespace std;
using namespace cv;


int main(void)
{
    Mat image, hsv, mask;
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


    enum {Play, Pause} mode = Play; 
    enum {Rouge,Vert,Bleu,Jaune} Masque = Rouge;

    // Variables pour les deux réseaux de neurones déterminant les articulations rouges et bleues
    int nb_entrees = 1;
    int nb_sorties_rouge = 5, nb_sorties_bleu = 4;

    MODELE modeleRouge = init_modele(nb_entrees,nb_sorties_rouge);
    MODELE modeleBleu = init_modele(nb_entrees,nb_sorties_bleu);

    // le nombre de couches et neurones par couche est défini dans NNdefinitions.h
    RESEAU reseauRouge = init_reseau(modeleRouge);
    RESEAU reseauBleu = init_reseau(modeleBleu);
    
    recupere_biais_et_poids_enregistres(&reseauRouge, "../textFiles/paramsNNRed.txt");
    affiche_reseau(reseauRouge);
    //recupere_biais_et_poids_enregistres(&reseauBleu, "../textFiles/paramsNNBlue.txt");
    //affiche_reseau(reseauBleu);

    waitKey(0);

    while(1)
    {
        if(mode == Play)
            image_num++;

        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = "../learning_videos/courbe0rythme0boited-1/0" + image_name + ".bmp";
        else image_name = "../learning_videos/courbe0rythme0boited-1/" + image_name + ".bmp";
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
            
            
            Canny( mask, canny_output, 100, 100*2 );


            /// Find contours
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


        Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
        if(!centresPastillesRouges.empty())
        {
            for (size_t i = 0; i < centresPastillesRouges.size(); i++)
            {
                //enterPointintoNeuralNet(centresPastillesRouges[i], &reseauRouge);
                enterFloatintoNeuralNet(centresPastillesRouges[i].y, &reseauRouge);
                propagation_avant(&reseauRouge);
                CLASSEMENT meilleur_ressemblance = cree_et_retourne_classement_ressemblance(reseauRouge); 
                cout <<"Meilleure ressemblance: "<< meilleur_ressemblance.chiffre << " : "<< meilleur_ressemblance.pourcentage_ressemblance<<endl;
                if(meilleur_ressemblance.chiffre == 0)
                    writePointwithTexttoMat(centresPastillesRouges[i],"foot",&drawing2, Scalar(0,0,255));
                else if(meilleur_ressemblance.chiffre == 1)
                    writePointwithTexttoMat(centresPastillesRouges[i],"knee",&drawing2, Scalar(0,0,255));
                else if(meilleur_ressemblance.chiffre == 2)
                    writePointwithTexttoMat(centresPastillesRouges[i],"hand",&drawing2, Scalar(0,0,255));
                else if(meilleur_ressemblance.chiffre == 3)
                    writePointwithTexttoMat(centresPastillesRouges[i],"elbow",&drawing2, Scalar(0,0,255));
                else if(meilleur_ressemblance.chiffre == 4)
                    writePointwithTexttoMat(centresPastillesRouges[i],"shoulder",&drawing2, Scalar(0,0,255));
            }
        }

/*         
        writePointstoMat(centresPastillesRouges,&drawing2, Scalar(0,0,255));
        writePointstoMat(centresPastillesBleues,&drawing2, Scalar(255,0,0));
        writePointstoMat(centrePastilleVerte,&drawing2, Scalar(0,255,0));
        writePointstoMat(centrePastilleJaune,&drawing2, Scalar(0,255,255)); */
        imshow( "Centres", drawing2 ); 

        
// ici on a les bons centres des pastilles
         
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

 