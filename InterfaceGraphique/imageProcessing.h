#if !defined(IMAGEPROCESSING_H) 
    #define IMAGEPROCESSING_H 

    #include "opencv2/imgcodecs.hpp"   
    using namespace std;
    using namespace cv;

    void writeLinestoMat(const vector<Point> pointsA, const vector<Point> pointsB, Mat *mat, const Scalar color);
    void writePointstoMat(const vector<Point> points, Mat *mat, const Scalar color);
    void writePointwithTexttoMat(Point point, string text, Mat *mat, const Scalar color);
    Mat createMatfromPointVector(vector<Point> points, Scalar pointsColor);
    void ifPointinRange_drawLine(Point p, int ymin, int ymax, Point *articulation, Point *articulationPrecedente, Mat *mat, Scalar color);
    void identifyArticulationsfromBluePoints(vector<Point> bluePoints, int yminFoot,int ymaxFoot,Point *foot,Point *previousfoot, int yminKnee,int ymaxKnee,Point *knee,Point *previousknee, int yminHand,int ymaxHand,Point *hand,Point *previoushand, int yminElbow,int ymaxElbow,Point *elbow,Point *previouselbow, Mat *mat, Scalar color);
    void identifyArticulationsfromRedPoints(vector<Point> redPoints, int yminFoot,int ymaxFoot,Point *foot,Point *previousfoot, int yminKnee,int ymaxKnee,Point *knee,Point *previousknee, int yminHand,int ymaxHand,Point *hand,Point *previoushand, int yminElbow,int ymaxElbow,Point *elbow,Point *previouselbow, int yminShoulder,int ymaxShoulder,Point *shoulder,Point *previousshoulder, Mat *mat, Scalar color);


#endif