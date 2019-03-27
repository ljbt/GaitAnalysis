#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
 
using namespace std;
using namespace cv;


int main(void)
{
    Mat image, hsv, mask;
    std::string image_name;
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

    while(1)
    {
        if(mode == Play)
            image_num++;

        image_name = std::to_string(image_num);
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
        
        imshow("Mask", mask);
        
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