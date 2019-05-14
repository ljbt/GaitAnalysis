#if !defined(IMAGEPROCESSING_H) 
    #define IMAGEPROCESSING_H 

    #include "opencv2/imgcodecs.hpp"   
    #if !defined(NN_DEF) 
        #define NN_DEF 
        #include "NNdefinitions.h"
    #endif
    using namespace std;
    using namespace cv;

    void writeLinestoMat(const vector<Point> pointsA, const vector<Point> pointsB, Mat *mat, const Scalar color);
    void writePointstoMat(const vector<Point> points, Mat *mat, const Scalar color);
    void writePointwithTexttoMat(Point point, string text, Mat *mat, const Scalar color);
    void enterPointintoNeuralNet(Point p, RESEAU *neuralNet);
    void enterFloatintoNeuralNet(float f, RESEAU *neuralNet);
    Mat createMatfromPointVector(vector<Point> points, Scalar pointsColor);

#endif