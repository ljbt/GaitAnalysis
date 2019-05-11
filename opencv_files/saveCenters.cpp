//#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <string>

#include "Mask.h"
#include "imageProcessing.h"
 
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

    bool calculCentresDone = false;

    while(1)
    {
        if(calculCentresDone == false)
        {
            if(mode == Play)
            {
                calculCentresDone = false;
                image_num++;
            }

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
            
 /*           Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
             writePointstoMat(centresPastillesRouges,&drawing2, Scalar(0,0,255));
            writePointstoMat(centresPastillesBleues,&drawing2, Scalar(255,0,0));
            writePointstoMat(centrePastilleVerte,&drawing2, Scalar(0,255,0));
            writePointstoMat(centrePastilleJaune,&drawing2, Scalar(0,255,255));
            imshow( "Centres", drawing2 );  */
            calculCentresDone = true;
            

            // on doit mtn enregistrer chaque centre dans son fichier approprié
            for (size_t i = 0; i < centresPastillesRouges.size(); i++)
            {
                Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
                circle(drawing2, centresPastillesRouges[i] ,2, Scalar(0,0,255), -1);
                imshow( "Centre pastille rouge a enregistrer", drawing2 );
                waitKey(200);

                int choix;
                string nomFichier;
                cout << "A quelle aticulation correspond cette pastille rouge?"<<endl;
                do{
                    cout << "1 pour epaule, 2 pour coude, 3 pour main, 4 pour genou, 5 pour pied"<<endl;
                    cin >> choix;
                    if(choix == 1)
                        nomFichier = "shoulderRed.txt";
                    else if(choix == 2)
                        nomFichier = "elbowRed.txt";
                    else if(choix == 3)
                        nomFichier = "handRed.txt";
                    else if(choix == 4)
                        nomFichier = "kneeRed.txt";
                    else if(choix == 5)
                        nomFichier = "footRed.txt";
                }while (choix <1 || choix >5);
                
                // on verifie que le point n'existe pas deja dans le fichier ou on veut l'enregistrer
                ifstream myfiletoread (nomFichier);
                if (myfiletoread.is_open())
                {
                    string chaine;
                    int x,y;
                    do{
                        getline(myfiletoread,chaine);
                        istringstream iss(chaine);
                        iss >> x >> y;
                        cout << "x = "<<x<<" y = "<<y<<endl;
                        Point p(x,y);
                        if ( p == centresPastillesRouges[i])
                        {
                            cout << "point déjà enregstré"<<endl;
                            break;
                        }
                    }while ( !myfiletoread.eof() );

                    myfiletoread.close();
                }
                else cout << "Unable to open file "<<nomFichier<< " to read";                

                ofstream myfile (nomFichier, ios::app); // se place a la fin du ficher pour ecrire les nouveaux points
                if (myfile.is_open())
                {
                    myfile << centresPastillesRouges[i].x<<" "<<centresPastillesRouges[i].y<<"\n";
                    myfile.close();
                }
                else cout << "Unable to open file "<<nomFichier<< " to write";
            }        

            for (size_t i = 0; i < centresPastillesBleues.size(); i++)
            {
                Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
                circle(drawing2, centresPastillesBleues[i] ,2, Scalar(255,0,0), -1);
                imshow( "Centre pastille bleue a enregistrer", drawing2 );
                waitKey(200);

                int choix;
                string nomFichier;
                cout << "A quelle aticulation correspond cette pastille bleue?"<<endl;
                do{
                    cout << "2 pour coude, 3 pour main, 4 pour genou, 5 pour pied"<<endl;
                    cin >> choix;
                    if(choix == 2)
                        nomFichier = "elbowBlue.txt";
                    else if(choix == 3)
                        nomFichier = "handBlue.txt";
                    else if(choix == 4)
                        nomFichier = "kneeBlue.txt";
                    else if(choix == 5)
                        nomFichier = "footBlue.txt";
                }while (choix <2 || choix >5);
                
                // on verifie que le point n'existe pas deja dans le fichier ou on veut l'enregistrer
                ifstream myfiletoread (nomFichier);
                if (myfiletoread.is_open())
                {
                    string chaine;
                    int x,y;
                    do{
                        getline(myfiletoread,chaine);
                        istringstream iss(chaine);
                        iss >> x >> y;
                        cout << "x = "<<x<<" y = "<<y<<endl;
                        if ( x == centresPastillesBleues[i].x && y == centresPastillesBleues[i].y )
                        {
                            cout << "point déjà enregstré"<<endl;
                            break;
                        }
                    }while ( !myfiletoread.eof() );

                    myfiletoread.close();
                }
                else cout << "Unable to open file "<<nomFichier<< " to read";                

                ofstream myfile (nomFichier, ios::app); // se place a la fin du ficher pour ecrire les nouveaux points
                if (myfile.is_open())
                {
                    myfile << centresPastillesBleues[i].x<<" "<<centresPastillesBleues[i].y<<"\n";
                    myfile.close();
                }
                else cout << "Unable to open file "<<nomFichier<< " to write";
            }  
    }

        
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
            calculCentresDone = false;
            mode = Pause;
            image_num--;
        }
        else if(c == 'S') // right arroy
        {     
            calculCentresDone = false;   
            mode = Pause;
            image_num++;
        }

    }

    
    destroyAllWindows();

    return 0;
  }

 