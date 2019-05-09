#include "imageProcessing.h"
#include "opencv2/imgproc.hpp"

void writePointstoMat(const vector<Point> points, Mat *mat, const Scalar color)
{
    for (size_t i = 0; i < points.size(); i++)
        circle(*mat, points[i] ,2, color, -1);
}