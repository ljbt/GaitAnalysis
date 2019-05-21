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

Mat createMatfromPointVector(vector<Point> points, Scalar pointsColor)
{
//	Rect rect = boundingRect(points);
//	Mat mat = Mat(rect.height, rect.width, CV_8UC3, Scalar(0,0,0));
	Mat mat = Mat(500, 500, CV_8UC3, Scalar(0,0,0)); // cree une matrice de 500 par 500
	writePointstoMat(points,&mat,pointsColor);
	return mat;
}

// si le point P est entre ymin et ymax, alors enregistre P dans articulation
// puis trace la ligne entre articulationPrecedente et articulation, dans mat avec la couleur color
// enfin l'articulation precedente devient articulation
void ifPointinRange_drawLine(Point p, int ymin, int ymax, Point *articulation, Point *articulationPrecedente, Mat *mat, Scalar color)
{
    if(p.y > ymin && p.y < ymax)
    {
        *articulation = p;

        if( articulationPrecedente->x == -1 && articulationPrecedente->y == -1  )
            *articulationPrecedente = *articulation;
        line( *mat, *articulationPrecedente, *articulation,  color);	
        *articulationPrecedente = *articulation;
    }
}

// A partir d'un tableau de points, identifie quel point correspond à une articulation et l'enregistre
// un point peut correspondre à un pied, genou, main et coude pour les pastilles bleues
void identifyArticulationsfromBluePoints(vector<Point> bluePoints, int yminFoot,int ymaxFoot,Point *foot,Point *previousfoot, int yminKnee,int ymaxKnee,Point *knee,Point *previousknee, int yminHand,int ymaxHand,Point *hand,Point *previoushand, int yminElbow,int ymaxElbow,Point *elbow,Point *previouselbow, Mat *mat, Scalar color)
{
    for (size_t i = 0; i < bluePoints.size(); i++)
    {
        ifPointinRange_drawLine(bluePoints[i],yminFoot,ymaxFoot,foot,previousfoot,mat,color);
        ifPointinRange_drawLine(bluePoints[i],yminKnee,ymaxKnee,knee,previousknee,mat,color);
        ifPointinRange_drawLine(bluePoints[i],yminHand,ymaxHand,hand,previoushand,mat,color);
        ifPointinRange_drawLine(bluePoints[i],yminElbow,ymaxElbow,elbow,previouselbow,mat,color);
    }
}

// A partir d'un tableau de points, identifie quel point correspond à une articulation et l'enregistre
// un point peut correspondre à un pied, genou, main, coude et epaule pour les pastilles rouges
void identifyArticulationsfromRedPoints(vector<Point> redPoints, int yminFoot,int ymaxFoot,Point *foot,Point *previousfoot, int yminKnee,int ymaxKnee,Point *knee,Point *previousknee, int yminHand,int ymaxHand,Point *hand,Point *previoushand, int yminElbow,int ymaxElbow,Point *elbow,Point *previouselbow, int yminShoulder,int ymaxShoulder,Point *shoulder,Point *previousshoulder, Mat *mat, Scalar color)
{
    for (size_t i = 0; i < redPoints.size(); i++)
    {
        ifPointinRange_drawLine(redPoints[i],yminFoot,ymaxFoot,foot,previousfoot,mat,color);
        ifPointinRange_drawLine(redPoints[i],yminKnee,ymaxKnee,knee,previousknee,mat,color);
        ifPointinRange_drawLine(redPoints[i],yminHand,ymaxHand,hand,previoushand,mat,color);
        ifPointinRange_drawLine(redPoints[i],yminElbow,ymaxElbow,elbow,previouselbow,mat,color);
        ifPointinRange_drawLine(redPoints[i],yminShoulder,ymaxShoulder,shoulder,previousshoulder,mat,color);
    }
}


