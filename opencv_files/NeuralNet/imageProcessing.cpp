#include "imageProcessing.h"
#include "opencv2/imgproc.hpp"

void writeLinestoMat(const vector<Point> pointsA, const vector<Point> pointsB, Mat *mat, const Scalar color)
{
	for (size_t i = 0; i < pointsA.size(); i++)
        line( *mat, pointsA[i], pointsB[i], color);		
}


void writePointstoMat(const vector<Point> points, Mat *mat, const Scalar color)
{
    for (size_t i = 0; i < points.size(); i++)
        circle(*mat, points[i] ,2, color, -1);
}

void writePointwithTexttoMat(Point point, string text, Mat *mat, const Scalar color)
{
    putText(*mat, text, point, FONT_HERSHEY_SIMPLEX, 0.3, color,1);
    circle(*mat, point ,2, color, -1);
}

void enterPointintoNeuralNet(Point p, RESEAU *neuralNet)
{
	if(AFFICHAGE)
		printf("Entre coordonnees point dans reseau\n");
	for(int i = 0; i < neuralNet->couches[0].nb_neurones; i++)
	{
		neuralNet->couches[0].neurones[i].entrees[0].x = p.x;
		neuralNet->couches[0].neurones[i].entrees[1].x = p.y;
	}
}

void enterFloatintoNeuralNet(float f, RESEAU *neuralNet)
{
	if(AFFICHAGE)
		printf("Entre flottant dans reseau\n");
	for(int i = 0; i < neuralNet->couches[0].nb_neurones; i++)
		neuralNet->couches[0].neurones[i].entrees[0].x = f;
}


Mat createMatfromPointVector(vector<Point> points, Scalar pointsColor)
{
//	Rect rect = boundingRect(points);
//	Mat mat = Mat(rect.height, rect.width, CV_8UC3, Scalar(0,0,0));
	Mat mat = Mat(500, 500, CV_8UC3, Scalar(0,0,0));
	writePointstoMat(points,&mat,pointsColor);
	return mat;
}