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
#include <vector>
using namespace std;

#include "NNdefinitions.h"
#include "fonctions.h"
#include "gestionModeles.h"

int main(void)
{
    // initialisations
	int nb_entrees = 2; // La donnée d'entrée est un point, donc 2 entrées pour x et y

    int choix;
    do{
        cout <<"-----MENU-----"<<endl;
        cout <<"\t-> 1 pour apprentissage pastilles rouges"<<endl;
        cout <<"\t-> 2 pour apprentissage pastilles bleues"<<endl;
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
    vector<MODELE> tabModeles;
    if(choix == 1)
    {
        string nomFichier = "footRed.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "kneeRed.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "handRed.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "elbowRed.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "shoulderRed.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);        
    }
    else if(choix == 2)
    {
        string nomFichier = "footBlue.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "kneeBlue.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "handBlue.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        nomFichier = "elbowBlue.txt";
        litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
    }

    melangeTabModeles(&tabModeles); // un peu d'aleatoire pour melanger les donnees a apprendre


    // boucle d'aprentissage
    // -> remplit les entrees du reseau 
    // -> propagation avant et calcul erreur
    // -> retropropagation si erreur trop importante (definie dans NNdefinitions.h)
    int nb_erreurs, nb_boucles = 0;
     do{
         nb_boucles++;
         nb_erreurs = 0;
         for (size_t i = 0; i < tabModeles.size(); i++)
         {
            recopie_EntreesModele_dansEntreesReseau(tabModeles[i], &reseau);
            propagation_avant_selon_modele(&reseau, modele.sorties_attendues);

			if( erreurs_reseau_insignifiantes(reseau) ){
				if(AFFICHAGE)
					cout << "erreurs des sorties du reseau negligeables pour le modele "<< i<<endl;
			}
			else 
			{
				if(AFFICHAGE)
                {
					printf("au moins une sortie du reseau a une erreur non negligeable\n");
					cout << "erreurs des sorties du reseau non negligeables pour le modele "<< i<<endl;
				}			
				nb_erreurs++;
				retropropagation(&reseau);
			} 
         }
         
    }while(nb_erreurs != 0);

    cout<<"Tous les modèles sont satisfaits au bout de "<<nb_boucles<< " boucles !"<<endl;
    
    if(choix == 1)
        enregistrement_biais_et_poids_reseau(reseau, "./paramsNNRed.txt");
    else if(choix == 2)
        enregistrement_biais_et_poids_reseau(reseau, "./paramsNNBlue.txt");
    else cout << "error so not writing parameters into file"<<endl;
    

    printf("Liberation de la memoire\n");
    for (size_t i = 0; i < tabModeles.size(); i++)
    {
        free(tabModeles[i].entrees);
        free(tabModeles[i].sorties_attendues);
    }
    
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