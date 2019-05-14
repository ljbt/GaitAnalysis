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

#if !defined(NN_DEF) 
    #define NN_DEF 
    #include "NNdefinitions.h"
#endif
#include "fonctions.h"
#include "gestionModeles.h"
#include "imageProcessing.h"

#include "opencv2/opencv.hpp"
using namespace cv;

int main(void)
{
    // initialisations
	int nb_entrees = 1; // une entree qui correspond à y

    int choix_global;
    do{
        cout <<"-----MENU-----"<<endl;
        cout <<"- 1 pour apprentissage"<<endl;
        cout <<"- 2 pour test"<<endl;
        cout <<"- 3 pour représenter les donnees d'apprentissage"<<endl;
        cin >> choix_global;
    }while(choix_global <1 || choix_global>3);  

    int choix;
    do{
        cout <<"\t-> 1 pour reseau pastilles rouges"<<endl;
        cout <<"\t-> 2 pour reseau pastilles bleues"<<endl;
        cin >> choix;
    }while(choix <1 || choix>2);

    int nb_sorties;
    if(choix == 1)
	    nb_sorties = 5; // Ce réseau détermine si le centre de la pastille rouge est celui du pied, du genou, de la main, du coude ou de l'epaule, donc 5 sorties
	else nb_sorties = 4; // pas epaule pour pastilles bleues
    MODELE modele = init_modele(nb_entrees,nb_sorties);

    // creation du réseau de neurone (architecture definie dans NNdefinitions.h)
	RESEAU reseau = init_reseau(modele);


    if(choix_global == 1|| choix_global == 3)
    {
        srand(time(NULL));
        init_poids_alea_Reseau(&reseau);
        init_biais_Reseau(&reseau);

    
        // creation du tableau contenant tous les modeles
        vector<MODELE> tabModeles;
        if( choix == 1 )
        {
            string nomFichier = "../textFiles/footRed.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/kneeRed.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/handRed.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/elbowRed.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/shoulderRed.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);        
        }
        else if(choix == 2)
        {
            string nomFichier = "../textFiles/footBlue.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/kneeBlue.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/handBlue.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
            nomFichier = "../textFiles/elbowBlue.txt";
            litPointsFichier_remplitModeles(nomFichier,nb_entrees,nb_sorties,&tabModeles);
        }

        melangeTabModeles(&tabModeles); // un peu d'aleatoire pour melanger les donnees a apprendre

        cout << tabModeles.size() << " modeles"<<endl;
       
        //return 0;
        if(choix_global == 1)
        {
            // boucle d'aprentissage
            // -> remplit les entrees du reseau 
            // -> propagation avant et calcul erreur
            // -> retropropagation si erreur trop importante (definie dans NNdefinitions.h)
            int nb_erreurs, nb_boucles = 0;
            cout << "LEARNING"<<endl;
            do{
                nb_boucles++;
                nb_erreurs = 0;
                for (size_t i = 0; i < tabModeles.size(); i++)
                {
                    recopie_EntreesModele_dansEntreesReseau(tabModeles[i], &reseau);
                    propagation_avant_selon_modele(&reseau, tabModeles[i].sorties_attendues);

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

                if(nb_boucles % 10000 == 0)
                {
                    cout << "BOUCLE N° "<< nb_boucles<<" : " << nb_erreurs<< " erreurs"<<endl;
                }
                
            }while(nb_erreurs != 0 && nb_erreurs > 12);

            if(nb_erreurs != 0)
                cout <<"Tous les modèles ne sont satisfaits au bout de "<<nb_boucles<< " boucles mais bon"<<endl;
            else cout<<"Tous les modèles sont satisfaits au bout de "<<nb_boucles<< " boucles !"<<endl;
            
            if(choix == 1)
                enregistrement_biais_et_poids_reseau(reseau, "../textFiles/paramsNNRed.txt");
            else if(choix == 2)
                enregistrement_biais_et_poids_reseau(reseau, "../textFiles/paramsNNBlue.txt");
            else cout << "error so not writing parameters into file"<<endl;
       }

       else if (choix_global == 3) // representation des donnees
       {
           cout << "ok"<<endl;
           vector<Point> tabPoints;
           for (size_t i = 0; i < tabModeles.size(); i++)
               tabPoints.push_back(Point(tabModeles[i].entrees[0].x, tabModeles[i].entrees[1].x));
           
           
           Mat img = createMatfromPointVector(tabPoints, Scalar(0,0,255));
           imshow("Donnees", img);
           waitKey(0);
       }

        printf("Liberation de la memoire\n");
        for (size_t i = 0; i < tabModeles.size(); i++)
        {
            free(tabModeles[i].entrees);
            free(tabModeles[i].sorties_attendues);
        }
    }

    else if(choix_global == 2)
    {
        if(choix == 1) // reseau rouge
            recupere_biais_et_poids_enregistres(&reseau, "../textFiles/paramsNNRed.txt");
        else if (choix == 2) // reseau bleu
            recupere_biais_et_poids_enregistres(&reseau, "../textFiles/paramsNNBlue.txt");
        
        int y;
        cout << "Veuillez saisir les coordonnees d'un point à tester: y = ";cin >> y; 
        for(int i = 0; i < reseau.couches[0].nb_neurones; i++)
            reseau.couches[0].neurones[i].entrees[0].x = y;

        affiche_reseau(reseau);
        propagation_avant(&reseau);
        cree_et_affiche_classement_ressemblance(reseau);
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