#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#include "NNdefinitions.h"
#include "fonctions.h"

int main(void)
{
    // initialisations
	int nb_entrees = 2; // La donnée d'entrée est un point, donc 2 entrées pour x et y
    
    int choix;
    do{
        cout <<"-----MENU-----"<<endl;
        cout <<"-> 1 pour apprentissage pastilles rouges"<<endl;
        cout <<"-> 2 pour apprentissage pastilles bleues"<<endl;
        cin >> choix;
    }while(choix <1 || choix>2);

    int nb_sorties;
    if(choix == 1)
	    nb_sorties = 5; // Ce réseau détermine si le centre de la pastille rouge est celui du pied, du genou, de la main, du coude ou de l'epaule, donc 5 sorties
	else nb_sorties = 4; // pas epaule pour pastilles bleues
    MODELE modele = init_modele(nb_entrees,nb_sorties);

    // creation du réseau de neurone (architecture definie dans NNdefinitions.h)
	RESEAU reseau = init_reseau(modele);
    srand(time(NULL));
    init_poids_alea_Reseau(&reseau);
    init_biais_Reseau(&reseau);

 
    // creation du tableau contenant tous les modeles
    string nomFichier = "footRed.txt";
    ifstream myfiletoread (nomFichier);
    if (myfiletoread.is_open())
    {
    string chaine;
    int x,y;
    do{
        getline(myfiletoread,chaine);
        istringstream iss(chaine);
        iss >> x >> y;
        cout << "x = "<<x<<" y = "<<y<<endl;

    }while ( !myfiletoread.eof() );

    myfiletoread.close();
    }
    else cout << "Unable to open file "<<nomFichier<< " to read"; 




    // boucle d'aprentissage
    // -> remplit les entrees du reseau 
    // -> propagation avant et calcul erreur
    // -> retropropagation si erreur trop importante (definie dans NNdefinitions.h)
/*     do{

    }while();

    printf("TOUS LES MODELES SONT SATISFAITS AVEC CES POIDS!\n");
    enregistrement_biais_et_poids_reseau(reseau);
*/
    printf("Liberation de la memoire\n");
    free(modele.entrees);
    free(modele.sorties_attendues);
    
    for(int i = 0 ; i< reseau.nb_couches ; i++)
    {
        for(int j = 0; j< reseau.couches[i].nb_neurones ; j++)
        {
            free(reseau.couches[i].neurones[j].entrees);
            free(reseau.couches[i].neurones[j].entrees_suivantes_liees);
        }
        free(reseau.couches[i].neurones);
    }
    free(reseau.couches); 

    printf("Fin du programme\n");
}