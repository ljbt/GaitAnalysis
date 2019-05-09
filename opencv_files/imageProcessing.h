#if !defined(IMAGEPROCESSING_H) 
    #define IMAGEPROCESSING_H 

    #include "opencv2/imgcodecs.hpp"    
    using namespace std;
    using namespace cv;

    void writePointstoMat(const vector<Point> points, Mat *mat, const Scalar color);

#endif