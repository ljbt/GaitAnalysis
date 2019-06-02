#include "imageProcessing.h"
#include "opencv2/imgproc.hpp"
#include "statistics.h"
#include "definitions.h"

#include <iostream>
#include <fstream>


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
bool detectFootDown(vector<Point> posFoot, size_t numElmts, double variation)
{
    if(posFoot.size() < numElmts)
        return false;
    
    vector<double> posX;
    for (size_t i = 0; i < posFoot.size(); i++)
        posX.push_back(posFoot[i].x);
    
    double coef = coef_variation(posX);
    cout << "coef = "<<coef<<endl;
    if(coef < variation)
        return true;
    else return false;
}

bool detectGaitCycle(int *numFootRightDown, vector<Point> posFootRight, int *numFootLeftDown, vector<Point> posFootLeft)
{
    // d'abord on cherche la pose du pied rouge, et qd il s'est posé on cherche la pose du pied bleu
    // puis on recommence
    if(*numFootRightDown == 0)
    {
        cout <<"cherche pose pied rouge"<<endl;
        if( detectFootDown(posFootRight, NB_IMAGES_DETECT_FOOT_DOWN, 0.04) )
        {
            cout<<"foot red down"<<endl;
            (*numFootRightDown) ++;
        }
    }
    if(*numFootRightDown == 1 )
    {
        cout <<"cherche pose pied bleu"<<endl;
        if( detectFootDown(posFootLeft, NB_IMAGES_DETECT_FOOT_DOWN, 0.04) )
        {
            cout<<"foot blue down"<<endl;
            (*numFootLeftDown) ++;
            *numFootRightDown = *numFootLeftDown = 0;
            return true;
        }
    } 
    return false;
}

double longeurJambe (Point pied, Point genou, Point hanche)
{
    if( pied.x != -1 && pied.y != -1 && genou.x != -1 && genou.y != -1 && hanche.x != -1 && hanche.y != -1 )
    {
        return norm(genou - pied) + norm(hanche - genou);
    }
    else return -1;
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

// fonction qui prend en param un vecteur de vecteurs de double,
// et retourne un vecteur contenant les moyennes
vector<double> meanVector(vector<vector<double>> v)
{
    vector<double> meanVector;

    for (size_t i = 0; i < v.size(); i++)
    {
        if(!v[i].empty())
        {
            if( meanVector.size() < i+1 )
            {
                meanVector.resize(i+1);
            }
            meanVector[i] = round(mean(v[i])) ;
        }
    }
    return meanVector;
}

// fonction qui enregistre dans un fichier de nom filename, le vecteur contenant les ordonnees du cycle de marche moyen
void savemeanVector(string filename,vector<double> meanVector)
{
    cout << "Saving mean vector into file "<<filename<<endl;
    ofstream myfile (filename); // se place a la fin du ficher pour ecrire les nouveaux points
    if (myfile.is_open())
    {
        for (size_t i = 0; i < meanVector.size(); i++)
        {
            myfile << i << " " << meanVector[i] <<"\n";
        }
        myfile.close();
    }
    else cout << "Unable to open file "<<filename<< " to write";
}

// fonction qui va chercher dans le fichier de nom filename les donnees permettant de recreer le vecteur decrivant un cycle normal moyen
vector<double> getNormalmeanVector(string filename)
{
    vector<double> meanVector;
    ifstream myfiletoread (filename);
    if (myfiletoread.is_open())
    {
        string chaine;
        int i,y;
        do{
            getline(myfiletoread,chaine);
            istringstream iss(chaine);
            iss >> i >> y;
            meanVector.resize(i+1);
            meanVector[i] = y ;
        }while ( !myfiletoread.eof() );

        myfiletoread.close();
    }
    else cout << "Unable to open file "<<filename<< " to read";  
    return meanVector;
}


// fonction qui prend un param un tableau de double et retourne un tableau de points (le x de chaque point correspond a l'abscisse du double, et le y au double)
vector<Point> doubleToPoints(vector<double> tabDouble)
{
    vector<Point> tabPoints;
    for (size_t i = 0; i < tabDouble.size(); i++)
    {
        tabPoints.resize(i+1);
        tabPoints[i].x = i;
        tabPoints[i].y = tabDouble[i];
    }
    
    return tabPoints;
}

// fonction qui prend en param un tableau de vecteurs représentant une courbe, mais dont certains points manquent (valent 0 en ordonnee)
// Le but est de tracer une ligne entre ces points et d'entrer les points de la ligne entre les deux points connus
// et de retourner le vecteur contenant la courbe complete (une valeur de y pour chaque abscisse)
vector<Point> fillVectorPoints(vector<Point> vecNotFull, Mat supportMat)
{
    vector<Point> vectorFull;

    Point p1, p2;
    long unsigned int i1=0, i2;
    int ecart;
    while(i1 < vecNotFull.size())
    {
        if(i1 == 0 && vecNotFull[i1].y == 0) // premier point du vecteur = 0 donc on doit aller chercher le prochain premier point
        {
            do{
                if(vectorFull.size() < i1+1)
                    vectorFull.resize(i1+1);
                vectorFull[i1] = vecNotFull[i1];
                i1++;
            }while(vecNotFull[i1].y == 0);
        }
        p1 = vecNotFull[i1];
        i2 = i1+1;
        ecart =0;
        if(vecNotFull[i2].y == 0 && i2 < vecNotFull.size() ) 
        {
            while (vecNotFull[i2].y == 0 && i2 < vecNotFull.size())
            {
                ecart++;
                i2++;
            }
            if(i2 >= vecNotFull.size())
            {
                cout << "No other point after " << p1 << endl;
                return vectorFull;
            }
            if(ecart > 0)
            {
                // on doit entrer les deux points connus et les points manquants dans vectorFull
                p2 = vecNotFull[i2];

                if(vectorFull.size() < i2+1)
                    vectorFull.resize(i2+1);
                vectorFull[i1] = p1;
                vectorFull[i2] = p2;
                LineIterator it(supportMat, p1, p2, 8);
                for(int i = 0; i < it.count; i++, ++it)
                {
                    vectorFull[it.pos().x] = it.pos() ; 
                }
            }
        }
        else
        {
            if(vectorFull.size() < i2+1 && i2+1 <= vecNotFull.size())
                vectorFull.resize(i2+1);
            vectorFull[i2] = vecNotFull[i2];
        }
        i1=i2;
    }
    

    return vectorFull;
}

vector<Point> adaptSizeVector(vector<Point> v, size_t new_size)
{
    vector<Point> newVec;
    size_t newAbs;
    for (size_t i = 0; i < v.size(); i++)
    {
        newAbs = (new_size*i)/v.size();
        if(newAbs>0)
        {
            if(newVec.size() < newAbs+1)
                newVec.resize(newAbs+1);
            newVec[newAbs].x = newAbs;
            newVec[newAbs].y = v[i].y;
        }
    }
    return newVec;
}

// fonction qui retourne l'erreur quadratique entre le vecteur du cycle normal et celui du cycle de la video actuelle
double mean_quadratic_error(vector<Point> normalVector, vector<Point> meanVector)
{
    double error = 0, nb_elmts = 0;
    if( normalVector.size() != meanVector.size())
    {
        cout<<"Pas possible de comparer deux vecteurs de taille differente !"<<endl;
        return -1;
    }
    else
    {
        for (size_t i = 0; i < normalVector.size(); i++)
        {
            if(normalVector[i].y > 0 && meanVector[i].y > 0)
            {
                error += pow( abs(normalVector[i].y - meanVector[i].y) , 2);
                nb_elmts++;
            }
        }
    }

    return error/nb_elmts;
}

void afficheTabPoints(vector<Point> tab)
{
    for (size_t i = 0; i < tab.size(); i++)
        cout << tab[i]<<endl;

}

double vitesseX(vector<Point> posFoot, size_t numElmts, int seuilMini)
{
	int d = 0;
	double v = 0.0;
	int c = 0;
	for (size_t i = 0; i < posFoot.size() - 1; i++)
    {
        d = posFoot[i+1].x -  posFoot[i].x;
		if (d>seuilMini)
		{
			c++;
			v += (double)d;
		}
		else
		{
			//cout << "Seuil non atteint : " << d << endl;
		}
    }
    
    if (c < 3) // Au moins 3 vitesses valides
    {
		cout << "C : " << c << endl;
		return 0.0;
	}
	else
		return v / (double)c;
}
