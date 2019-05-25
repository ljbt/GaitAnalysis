#include "imageProcessing.h"
#include "opencv2/imgproc.hpp"
#include "statistics.h"
#include "definitions.h"

#include <iostream>

using namespace std;

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

// draw lines into mat between articulation points
void drawSqueletton(Mat *mat, Point foot1, Point foot2, Point knee1, Point knee2, Point hip, Point hand1, Point hand2, Point elbow1, Point elbow2, Point shoulder, Point head)
{
    Scalar red(0,0,255);
    Scalar blue(255,0,0);
    Scalar green(0,255,0);
    Scalar yellow(0,255,255);
    Scalar white(255,255,255);


    // bas du corps

    if( (foot1.x != -1 && foot1.y != -1) || (knee1.x != -1 && knee1.y != -1) )
    {
        if( (foot1.x != -1 && foot1.y != -1) && (knee1.x != -1 && knee1.y != -1) )
            line(*mat, foot1, knee1, white);

        if(foot1.x != -1 && foot1.y != -1)
            circle(*mat, foot1 ,2, red, -1);
        if(knee1.x != -1 && knee1.y != -1)
            circle(*mat, knee1 ,2, red, -1);
    }

    if( (knee1.x != -1 && knee1.y != -1) || (hip.x != -1 && hip.y != -1) )
    {
        if( (knee1.x != -1 && knee1.y != -1) && (hip.x != -1 && hip.y != -1) )
            line(*mat, knee1, hip, white);

        if(knee1.x != -1 && knee1.y != -1)
            circle(*mat,knee1,2, red, -1);
        if(hip.x != -1 && hip.y != -1) 
            circle(*mat, hip ,2, green, -1);
    }

    if( (foot2.x != -1 && foot2.y != -1) || (knee2.x != -1 && knee2.y != -1) )
    {
        if( (foot2.x != -1 && foot2.y != -1) && (knee2.x != -1 && knee2.y != -1) )
            line(*mat, foot2, knee2, white);
        
        if(foot2.x != -1 && foot2.y != -1)
            circle(*mat, foot2 ,2, blue, -1);
        if(knee2.x != -1 && knee2.y != -1) 
            circle(*mat, knee2 ,2, blue, -1);
    }

    if( (knee2.x != -1 && knee2.y != -1) || (hip.x != -1 && hip.y != -1) )
    {
        if( (knee2.x != -1 && knee2.y != -1) && (hip.x != -1 && hip.y != -1) )
            line(*mat, knee2, hip, white);

        if(knee2.x != -1 && knee2.y != -1)
            circle(*mat,knee2,2, blue, -1);
        if(hip.x != -1 && hip.y != -1)
            circle(*mat, hip ,2, green, -1);
    }

    // haut du corps

    if( (hip.x != -1 && hip.y != -1) || (shoulder.x != -1 && shoulder.y != -1) )
    {
        if( (hip.x != -1 && hip.y != -1) && (shoulder.x != -1 && shoulder.y != -1) )
            line(*mat, hip, shoulder, white);

        if(hip.x != -1 && hip.y != -1)
            circle(*mat,hip,2, green, -1);
        if(shoulder.x != -1 && shoulder.y != -1)
            circle(*mat, shoulder ,2, red, -1);
    } 

    if( (elbow1.x != -1 && elbow1.y != -1) || (shoulder.x != -1 && shoulder.y != -1) )
    {
        if( (elbow1.x != -1 && elbow1.y != -1) && (shoulder.x != -1 && shoulder.y != -1) )
            line(*mat, elbow1, shoulder, white);

        if(elbow1.x != -1 && elbow1.y != -1)
            circle(*mat,elbow1,2, red, -1);
        if(shoulder.x != -1 && shoulder.y != -1)
            circle(*mat, shoulder ,2, red, -1);
    } 

    if( (elbow1.x != -1 && elbow1.y != -1) || (hand1.x != -1 && hand1.y != -1) )
    {
        if( (elbow1.x != -1 && elbow1.y != -1) && (hand1.x != -1 && hand1.y != -1) )
            line(*mat, elbow1, hand1, white);

        if(elbow1.x != -1 && elbow1.y != -1)
            circle(*mat,elbow1,2, red, -1);
        if(hand1.x != -1 && hand1.y != -1)
            circle(*mat, hand1 ,2, red, -1);
    } 

    if( (elbow2.x != -1 && elbow2.y != -1) || (shoulder.x != -1 && shoulder.y != -1) )
    {
        if( (elbow2.x != -1 && elbow2.y != -1) && (shoulder.x != -1 && shoulder.y != -1) )
            line(*mat, elbow2, shoulder, white);

        if(elbow2.x != -1 && elbow2.y != -1)
            circle(*mat,elbow2,2, red, -1);
        if(shoulder.x != -1 && shoulder.y != -1)
            circle(*mat, shoulder ,2, red, -1);
    } 

    if( (elbow2.x != -1 && elbow2.y != -1) || (hand2.x != -1 && hand2.y != -1) )
    {
        if( (elbow2.x != -1 && elbow2.y != -1) && (hand2.x != -1 && hand2.y != -1) )
            line(*mat, elbow2, hand2, white);

        if(elbow2.x != -1 && elbow2.y != -1)
            circle(*mat,elbow2,2, blue, -1);
        if(hand2.x != -1 && hand2.y != -1)
            circle(*mat, hand2 ,2, blue, -1);
    } 

    if( (head.x != -1 && head.y != -1) || (shoulder.x != -1 && shoulder.y != -1) )
    {
        if( (head.x != -1 && head.y != -1) && (shoulder.x != -1 && shoulder.y != -1) )
            line(*mat, head, shoulder, white);

        if(head.x != -1 && head.y != -1)
            circle(*mat,head,2, yellow, -1);
        if(shoulder.x != -1 && shoulder.y != -1)
            circle(*mat, shoulder ,2, red, -1);
    } 
}

// fonction qui entre newPoint dans le vector v, en decalant les autres pour pas depasser maxSize
void managePointVector(Point newPoint, vector<Point> *v, size_t maxSize)
{
    if(newPoint.x != -1 && newPoint.y != -1)
    {
        if(v->size() < maxSize)
            v->push_back(newPoint);
        else
        {
            // decale les points avant d'inserer le nouveau
            rotate(v->begin(),v->begin()+(v->size()+1 - maxSize) ,v->end());
            v->resize(maxSize-1);
            v->push_back(newPoint);
        }
    }
}

// fonction qui renvoie true si la position en x d'un tableau de numElmts Points varie assez
// si le tableau ne contient pas numElmts elements retourne aussi faux
bool detectFootDown(vector<Point> posFoot, size_t numElmts)
{
    if(posFoot.size() < numElmts)
        return false;
    
    vector<double> posX;
    for (size_t i = 0; i < posFoot.size(); i++)
        posX.push_back(posFoot[i].x);
    
    double coef = coef_variation(posX);
    if(coef > 0.05)
        return false;
    else return true;
}

bool detectGaitCycle(int *numFootRightDown, vector<Point> posFootRight, int *numFootLeftDown, vector<Point> posFootLeft)
{
    // si le pied rouge ne s'est pas posé ou que le pied bleu s'est posé on doit detecter la pose du pied rouge
    if(*numFootRightDown == 0 || *numFootLeftDown == 1)
    {
        if( detectFootDown(posFootRight, NB_IMAGES_DETECT_FOOT_DOWN) )
        {
            cout<<"foot red down"<<endl;
            (*numFootRightDown) ++;
        }
    }
    // si le pied rouge s'est posé, on doit attendre que le pied bleu se pose
    if(*numFootRightDown == 1 && *numFootLeftDown < 1)
    {
        if( detectFootDown(posFootLeft, NB_IMAGES_DETECT_FOOT_DOWN) )
        {
            cout<<"foot blue down"<<endl;
            (*numFootLeftDown) ++;
        }
    }
    // Une fois que le pied bleu s'est posé on doit attendre que le pied rouge se repose

    if( *numFootRightDown == 2)
    {
        *numFootRightDown = *numFootLeftDown = 0;
        return true;
    }
    else return false;
}

// fonction qui prend en parametre un vecteur de vecteurs de points, et retourne un vecteur de vecteurs de double
// le x de chaque point correspond aux indices du vecteur, et pour chaque indice on stocke tous les y correspondant
vector<vector<double>> pointsToDouble( vector<vector<Point>> tabPoints)
{
    vector < vector <double> > tabDouble;

    for (size_t i = 0; i < tabPoints.size(); i++)
    {
        for (size_t j = 0; j < tabPoints[i].size(); j++)
        {
            // pour chaque point de chaque cycle on doit enregistrer, à l'indice x, l'ordonnee y 
            if((long unsigned int)tabPoints[i][j].x+1 > tabDouble.size() && tabPoints[i][j].y >= 0 )
            {
                tabDouble.resize((size_t)(long unsigned int)tabPoints[i][j].x+1);
            }
            if(tabPoints[i][j].y >= 0)
            {
                tabDouble[ (size_t)(long unsigned int)tabPoints[i][j].x ].push_back( tabPoints[i][j].y );
            }  
        }
    }
    return tabDouble;
}


double longeurJambe (Point pied, Point genou, Point hanche)
{
    if( pied.x != -1 && pied.y != -1 && genou.x != -1 && genou.y != -1 && hanche.x != -1 && hanche.y != -1 )
    {
        return norm(genou - pied) + norm(hanche - genou);
    }
    else return -1;
}