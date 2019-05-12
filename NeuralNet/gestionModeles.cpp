#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#include "NNdefinitions.h"
#include "fonctions.h"

// Entre successivement les points ecrits dans le fichier dans le tableau des modeles 
void litPointsFichier_remplitModeles(string nomFichier, const int nb_entrees, const int nb_sorties, vector<MODELE> *tabModeles)
{
    int idx;
    if(nomFichier.find("foot") != string::npos)
        idx = 0;
    else if(nomFichier.find("knee") != string::npos)
        idx = 1;
    else if(nomFichier.find("hand") != string::npos)
        idx = 2;
    else if(nomFichier.find("elbow") != string::npos)
        idx = 3;
    else if(nomFichier.find("shoulder") != string::npos)
        idx = 4;
    else idx = -1;

    if (idx == -1)
        cout<<"error file name in litPointsFichier_remplitModeles()"<<endl;
    else
    {
        ifstream myfiletoread (nomFichier);
        if (myfiletoread.is_open())
        {
            string chaine;
            int x,y;
            do{
                getline(myfiletoread,chaine);
                istringstream iss(chaine);
                iss >> x >> y;
                MODELE modele = init_modele(nb_entrees,nb_sorties);
                modele.entrees[0].x = x;
                modele.entrees[1].x = y;
                determine_sortieModeleAttendue(idx,&modele); // pour le pied c'est 4
                tabModeles->push_back(modele);
            }while ( !myfiletoread.eof() );

            myfiletoread.close();
        }
        else cout << "Unable to open file "<<nomFichier<< " to read";       
    }
}

void melangeTabModeles(vector<MODELE> *tabModele)
{
    auto rng = default_random_engine {};
    shuffle( begin(*tabModele), end(*tabModele), rng );
}