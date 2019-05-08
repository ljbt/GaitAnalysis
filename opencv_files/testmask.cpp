#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
 
using namespace std;
using namespace cv;


int main(void)
{
    Mat image, hsv, mask;
    int Hmin = 0, Hmax = 179;
    int Smin = 0, Smax = 255;
    int Vmin = 0, Vmax = 255; 

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

    int image_num=0;
    string image_name;

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
        imshow("Original", image);


        // Check if image loaded successfully
        if( ! image.data ){
            cout << "Error loading image " << endl;
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
        
        imshow("Original", image);
        imshow("Mask", mask);
        
        // Press  ESC on keyboard to exit
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