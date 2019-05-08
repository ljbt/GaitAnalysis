//#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
 
using namespace std;
using namespace cv;



int main(void)
{
    Mat image, hsv, mask;
    string image_name;
    int image_num = 0;

  // Red mask
    int Hmin = 0, Hmax = 10;
    int Smin = 100, Smax = 255;
    int Vmin = 100, Vmax = 255;  

/*     // Green mask
    int Hmin = 70, Hmax = 90;
    int Smin = 100, Smax = 255;
    int Vmin = 30, Vmax = 255;  
 */
    int l_h ,u_h, l_s ,u_s ,l_v, u_v ;

    //trackbars for H, S and V
    namedWindow("Trackbars");
    createTrackbar("L - H", "Trackbars",&Hmin,Hmax); // trackbar at 0, can go to 179
    createTrackbar("U - H", "Trackbars",&Hmax,Hmax); // trackbar at 179, can go to 179
    createTrackbar("L - S", "Trackbars",&Smin,Smax); 
    createTrackbar("U - S", "Trackbars",&Smax,Smax);
    createTrackbar("L - V", "Trackbars",&Vmin,Vmax); 
    createTrackbar("U - V", "Trackbars",&Vmax,Vmax); 

    enum {Play, Pause} mode = Play; 


    RNG rng(12345);
    const char* source_window = "Mask";
    
    while(1)
    {
        if(mode == Play)
            image_num++;

        image_name = to_string(image_num);
        if(image_num < 10)
            image_name = "./learning_videos/courbe0rythme0boite0-1/0" + image_name + ".bmp";
        else image_name = "./learning_videos/courbe0rythme0boite0-1/" + image_name + ".bmp";
        //printf("image name = %s\n", image_name.c_str());
        image = imread(image_name);

        // Check if image loaded successfully
        if( ! image.data ){
            cout << "Error loading image " + image_name << endl;
            return -1;
        } 

        cvtColor(image, hsv ,COLOR_BGR2HSV);
       
        l_h = getTrackbarPos("L - H","Trackbars");
        u_h = getTrackbarPos("U - H","Trackbars");
        l_s = getTrackbarPos("L - S","Trackbars");
        u_s = getTrackbarPos("U - S","Trackbars");
        l_v = getTrackbarPos("L - V","Trackbars");
        u_v = getTrackbarPos("U - V","Trackbars");

        inRange(hsv, Scalar(l_h,l_s,l_v), Scalar(u_h,u_s,u_v), mask); 
        blur( mask, mask, Size(3,3) );       // ou GaussianBlur(mask,mask,Size(5,5),0); on peut aussichanger la taille de la fenetre de floutage
        
        namedWindow( source_window );
        imshow(source_window, mask);



        //createTrackbar("Aire box" , "Contours", &thresh_area, 1000);
        Mat canny_output;
        Canny( mask, canny_output, 100, 100*2 );

        /// Find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE ); //EXTERNAL pour un contour exterieur

        vector<Point> centers;
        for( size_t i = 0; i< contours.size(); i++ ) // on recupere les centres des contours
        {
            Moments m = moments(contours[i]);
            Point p = Point(m.m10/m.m00, m.m01/m.m00);
            // on verifie que le centre calculé est bien dans l'image
            Rect rect = Rect(0, 0, canny_output.size().width, canny_output.size().height);
            if( rect.contains(p) )
                centers.push_back(p);
            else 
            {
                contours.erase(contours.begin()+i);
                i--;
            }
        }

        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  /*       for( size_t i = 0; i< centers.size(); i++ )
            circle(drawing, centers[i] ,2, Scalar(255,255,255), -1);

        imshow( "Contours", drawing );

        waitKey(0); */

        vector<Rect> boitesEnglobantes;
        // il faut verifier que les centres ne sont pas trop proches, ce qui signifierait une pastille segmentée

        cout << endl;

        for (size_t i = 0; i < centers.size(); i++)
        {
            cout << "centre "<< i << " " << centers[i]<<endl;

            int cote = 20;
            Rect zone = Rect(centers[i].x - cote/2, centers[i].y - cote/2, cote, cote);
            rectangle(drawing, zone, Scalar(255,0,255));

            vector<Point> centresProches;
            vector<Rect> boites;
            boites.push_back(boundingRect(contours[i])); // calcule et enregistre la boite englobante du contour actuel
            cout << "boite du contour du centre actuel: "<<boundingRect(contours[i])<<endl;

            bool pastilleFractionnee = false;
            for (size_t j = 0; j < centers.size(); j++)
            {
                 if(j != i)
                {
                    if( zone.contains(centers[j]) ) // si la zone contient un autre centre c'est que la pastille est segmentee
                    {
                        cout << "centre "<< j<<" "<<centers[j]<<" est ds la zone"<<endl;
                        pastilleFractionnee = true;
                        boites.push_back(boundingRect(contours[j])); // enregistre les boites englobantes des autres fractions de la pastille
                        cout << "taille boites = " << boites.size()<<endl;
                        centresProches.push_back(centers[j]); // on enregistre leur centre dans le tableau de centres proches
                        cout << "taille centres proches = " << centresProches.size()<<endl;
                        centers.erase( centers.begin()+j); // et on supprime leur centre du tableau de centres
                    }
                }
            }
            if(pastilleFractionnee) // on doit calculer le centre des centres, et la boite englobant tous les contours
            {
                cout << "autres centres dans zone donc calcul du centre moyen" << endl;
                for (size_t l = 0; l < centresProches.size(); l++)
                {
                    centers[i] += centresProches[l];
                    //centers[i].y += centresProches[l].y;
                }
                centers[i].x /= centresProches.size()+1;
                centers[i].y /= centresProches.size()+1;

                cout << "et de la boite englobant les boites: ";
                Rect grosseBoite;
                for (size_t k = 0; k < boites.size(); k++)
                {
                    cout << boites[k]<<" ";
                    grosseBoite |= boites[k]; 
                }
                cout << "ce qui donne la grosse boite: "<<grosseBoite<<endl;
                boitesEnglobantes.push_back(grosseBoite);
            }
            else
            { 
                cout<< "pas d'autre centre ds zone donc enregistrement de la boite "<<*boites.begin()<<endl;
                boitesEnglobantes.push_back(*boites.begin());
            }

            //waitKey(0);
        }
        

        /// Draw contours
        for( size_t i = 0; i< centers.size(); i++ )
        {   

            circle(drawing, centers[i] ,2, Scalar(255,255,255), -1);
        }
        for( size_t i = 0; i< boitesEnglobantes.size(); i++ )
        {   
            rectangle(drawing, boitesEnglobantes[i], Scalar(0,0,255));
        }

        /// Show in a window
        imshow( "Contours", drawing );

         
        char c=(char)waitKey(125); // waits 125ms to get a key value
        if(c==27) // echap
            break;
        else if(c == ' ') // space
        {
            if(mode == Play)
            {
                printf("pause\n");
                mode = Pause;
            }
            else if(mode == Pause)
            {
                printf("play\n");
                mode = Play;
            }   
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

 