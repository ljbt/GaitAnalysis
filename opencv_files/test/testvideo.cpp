#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace std;
using namespace cv;
 


int main(){
 
  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
   VideoCapture cap("./test.mp4"); 
    
  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  } 

/*   Mat image = imread("25.bmp", CV_LOAD_IMAGE_COLOR);
  // Check if image loaded successfully
  if( ! image.data ){
    cout << "Error loading image" << endl;
    return -1;
  } 

  imshow("image",image); */

/*  int Hmin = 0, Hmax = 179;
  int Smin = 0, Smax = 255;
  int Vmin = 0, Vmax = 255; */
  int Hmin = 0, Hmax = 10;
  int Smin = 100, Smax = 255;
  int Vmin = 100, Vmax = 255; 

  //trackbars for H, S and V
  namedWindow("Trackbars");
  createTrackbar("L - H", "Trackbars",&Hmin,Hmax); // trackbar at 0, can go to 179
  createTrackbar("U - H", "Trackbars",&Hmax,Hmax); // trackbar at 179, can go to 179
  createTrackbar("L - S", "Trackbars",&Smin,Smax); 
  createTrackbar("U - S", "Trackbars",&Smax,Smax);
  createTrackbar("L - V", "Trackbars",&Vmin,Vmax); 
  createTrackbar("U - V", "Trackbars",&Vmax,Vmax); 

  

  while(1){
 
     Mat frame;
    // Capture frame-by-frame
    cap >> frame;
  
    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    Mat hsv;
    cvtColor(frame, hsv ,COLOR_BGR2HSV);
/*     Mat hsv;
    cvtColor(image, hsv ,COLOR_BGR2HSV);   */
    int l_h = getTrackbarPos("L - H","Trackbars");
    int u_h = getTrackbarPos("U - H","Trackbars");
    int l_s = getTrackbarPos("L - S","Trackbars");
    int u_s = getTrackbarPos("U - S","Trackbars");
    int l_v = getTrackbarPos("L - V","Trackbars");
    int u_v = getTrackbarPos("U - V","Trackbars");

    Mat mask;
    inRange(hsv, Scalar(l_h,l_s,l_v), Scalar(u_h,u_s,u_v), mask);
    imshow("mask", mask);
    
    // Display the resulting frame
    //imshow( "Frame",  frame);

/*     vector<Mat> channels;
    split(hsv, channels);   
    imshow( "Hue",  channels[0]); */

    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }
  
  // When everything done, release the video capture object
  cap.release();
 
  // Closes all the frames
  destroyAllWindows();
     
  return 0;
}