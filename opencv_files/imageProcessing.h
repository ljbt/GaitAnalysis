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
    void drawSqueletton(Mat *mat, Point foot1, Point foot2, Point knee1, Point knee2, Point hip, Point hand1, Point hand2, Point elbow1, Point elbow2, Point shoulder, Point head);
    void managePointVector(Point newPoint, vector<Point> *v, size_t maxSize);
    bool detectFootDown(vector<Point> posFoot, size_t numElmts);
    bool detectGaitCycle(int *numFootRightDown, vector<Point> posFootRight, int *numFootLeftDown, vector<Point> posFootLeft);
    double longeurJambe (Point pied, Point genou, Point hanche);
    vector<vector<double>> pointsToDouble( vector<vector<Point>> tabPoints);
    vector<double> meanVector(vector<vector<double>> v);
    void savemeanVector(string filename,vector<double> meanVector);
    vector<double> getNormalmeanVector(string filename);
    vector<Point> doubleToPoints(vector<double> tabDouble);
    vector<Point> fillVectorPoints(vector<Point> vecNotFull, Mat supportMat);
    vector<Point> adaptSizeVector(vector<Point> v, size_t new_size);
    double mean_quadratic_error(vector<Point> normalVector, vector<Point> meanVector);
    void afficheTabPoints(vector<Point> tab);


#endif