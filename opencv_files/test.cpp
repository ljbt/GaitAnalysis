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

/*   // Red mask
    int Hmin = 0, Hmax = 10;
    int Smin = 100, Smax = 255;
    int Vmin = 100, Vmax = 255;   */

/*     // Green mask
    int Hmin = 70, Hmax = 90;
    int Smin = 100, Smax = 255;
    int Vmin = 30, Vmax = 255;  
 */
    // Blue mask
    int Hmin = 95, Hmax = 125;
    int Smin = 173, Smax = 255;
    int Vmin = 40, Vmax = 255;  

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


    //RNG rng(12345);
    const char* source_window = "Contours";
    
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
       
        l_h = getTrackbarPos("L - H","Trackbars");
        u_h = getTrackbarPos("U - H","Trackbars");
        l_s = getTrackbarPos("L - S","Trackbars");
        u_s = getTrackbarPos("U - S","Trackbars");
        l_v = getTrackbarPos("L - V","Trackbars");
        u_v = getTrackbarPos("U - V","Trackbars");

        inRange(hsv, Scalar(l_h,l_s,l_v), Scalar(u_h,u_s,u_v), mask); 
        blur( mask, mask, Size(3,3) );       // ou GaussianBlur(mask,mask,Size(5,5),0); on peut aussichanger la taille de la fenetre de floutage
        
        Mat canny_output;
        Canny( mask, canny_output, 100, 100*2 );

        namedWindow( source_window );
        imshow(source_window, canny_output);


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

        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for (size_t i = 0; i < centres.size(); i++)
            circle(drawing, centres[i] ,2, Scalar(255,255,255), -1);
         
        imshow( "Centres pas corrigés", drawing );


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
        
        Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
        for (size_t i = 0; i < centres.size(); i++)
            circle(drawing2, centres[i] ,2, Scalar(255,255,255), -1);
        imshow( "Bons centres", drawing2 );
        
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

 