#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>
#include <complex.h>
#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
#include "ESLib.h"	
#include "OutilsLib.h"	
#include "BmpLib.h"

#include "definitions.h"
#include "IHM.h"
#include "Mat_Im_Recadrage.h"

int ** alloueMatrice(int h, int l)
{
	int ** m;
	int i;
	m = (int **)malloc(sizeof(int *)*h);
	for(i=0;i<h;i++)
	{
		m[i] = (int *)malloc(sizeof(int)*l);
	}
	return m;
}

void libereMatrice(int h, int l,int ** m)
{
	int i;
	for(i=0;i<h;i++)
	{
		free(m[i]); 
	}

	free(m);
}


void allocation4Matrices(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s)
{
	int h,l;
	h = image->hauteurImage;
	l = image->largeurImage;

	//allocation memoire des differentes matrices
	*r = alloueMatrice(h,l);
	*v = alloueMatrice(h,l);
	*b = alloueMatrice(h,l);
	*s = alloueMatrice(h,l);
}



void cree3Matrices(DonneesImageRGB * image, int *** bleu, int *** vert, int *** rouge)
{
	int ** b, ** v, ** r;
	int i,j,k = 0; //seulement k initialisé à 0
	r = alloueMatrice(image->hauteurImage,image->largeurImage);
	v = alloueMatrice(image->hauteurImage,image->largeurImage);
	b = alloueMatrice(image->hauteurImage,image->largeurImage);
	for(i=0;i<image->hauteurImage;i++)
	{
		for(j=0;j<image->largeurImage;j++)
		{
			b[i][j] = image->donneesRGB[k];
			k++;
			v[i][j] = image->donneesRGB[k];
			k++;
			r[i][j] = image->donneesRGB[k];
			k++;
		}
	}


	* rouge = r;
	* vert = v;
	* bleu = b;

}



DonneesImageRGB *creeImage(int h, int l, int **r, int **v, int **b)
{
	DonneesImageRGB *image;
	int i,j, k=0;

	image = (DonneesImageRGB *)malloc(sizeof(DonneesImageRGB));
	image->largeurImage = l;
	image->hauteurImage = h;
	image->donneesRGB = (unsigned char *)malloc(sizeof(unsigned char) *l*h*3);

	for(i=0; i<h; i++)
	{
		for(j=0; j<l; j++)
		{
			image->donneesRGB[k++] = b[i][j];
			image->donneesRGB[k++] = v[i][j];
			image->donneesRGB[k++] = r[i][j];
		}
	}

	return image;
}


void creeMatNG(DonneesImageRGB * image, int ** b, int ** v, int ** r, int ***s)
{
	int h,l;
	h = image->hauteurImage;
	l = image->largeurImage;

	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{
			(*s)[i][j] = 0.2125*r[i][j] + 0.7154*v[i][j] + 0.0721*b[i][j];
		}
	}
}



void afficheTab(int *tab, int taille)
{
	for (int i=0; i<taille; i++)
	{
		printf("tab[%d] = %d\n", i, tab[i]);
	}
}

int maxTab (int *tab, int taille)
{
	int max=0;
	
	for (int i=0; i<taille; i++)
	{
		if (tab[i] > max)
		{
			max = tab[i];
		}
	}
	
	return max;
}





void seuillage(DonneesImageRGB * image, int ***s, int seuil)
{
	int h,l;
	h = image->hauteurImage;
	l = image->largeurImage;

	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{	
			//si inferieur au seuil noir, sinon blanc
			if((*s)[i][j] < seuil)
			{
				(*s)[i][j]=0;
			}
			else{
				(*s)[i][j]=255;
			}
		}
	}

}

//cree matrices r,v,b depuis l'image, remplie matrice s des valeurs en niveau de gris puis des valeurs seuillees 
void creeMatSeuil(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s)
{
	int seuil=120;

	cree3Matrices(image,b,v,r); 
	/*printf("\nLes 3 matrices de couleur sont créées, choisissez le seuil pour l'image NB\n");
	do{

		printf("Compris dans ]0,255[ :\n");
		scanf("%d", &seuil);
	}while(seuil<=0 || seuil>=255);*/

	creeMatNG(image, *b,*v,*r,s); //creation matrice niveau de gris
	seuillage(image,s,seuil); //creation matrice noir et blanc

}


//redimmensionne image aux contours d'une forme noire et enregistre dans matrice et zone
void creePetiteMat(DonneesImageRGB * image,DonneesImageRGB **petite_image, int **s, int ***petite_s, zone *zPetiteI, int *petite_h, int *petite_l)
{
	//cette fonction parcourt la matrice dans tous les sens pour déterminer les points extérieurs
	//du chiffre, et ainsi réduire la matrice à son minimum

	int h,l;
	int i=0,j=0;
	int /*x_dessus=0,*/y_dessus=0;
	int /*x_dessous=0,*/ y_dessous=0;
	int x_gauche=0/*, y_gauche=0*/;
	int x_droite=0/*, y_droite=0*/;

	h = image->hauteurImage;
	l = image->largeurImage;

	bool point_dessus_trouve = false, point_dessous_trouve = false;
	bool point_gauche_trouve = false, point_droite_trouve = false;

	for(i=0 ; i<h ; i++) //De gauche à droite, puis passe à la ligne du dessus
	{
		for(j=0; j<l; j++)
		{
			if(s[i][j] == 0) //si pixel noir, point trouvé
			{
				point_dessous_trouve = true;
				break;
			}
		}
		if(point_dessous_trouve == true) break;

	}

	//on enregistre les coordonnees du point
	//x_dessous = j;
	y_dessous = i;
	//printf("x_dessous : %d\n", x_dessous); printf("y_dessous : %d\n", y_dessous);


	for(i=h-1 ; i>=0 ; i--) //De gauche à droite, puis passe à ligne du dessous
	{
		for(j=0; j<l; j++)
		{
			if(s[i][j] == 0)
			{
				point_dessus_trouve = true;
				break;
			}
		}
		if(point_dessus_trouve == true) break;
	}

	//x_dessus = j;
	y_dessus = i;
	//printf("\nx_dessus : %d\n", x_dessus); printf("y_dessus : %d\n", y_dessus);
	

	for(j=0 ; j<l ; j++) //De bas en haut, puis passe à la colonne de droite
        {
                for(i=0; i<h; i++)
                {
                        if(s[i][j] == 0)
                        {
                                point_gauche_trouve = true;
                                break;
                        }
                }
                if(point_gauche_trouve == true) break;
        }

        x_gauche = j;
        //y_gauche = i;
        //printf("\nx_gauche : %d\n", x_gauche); printf("y_gauche : %d\n", y_gauche);

	
        for(j=l-1 ; j>=0 ; j--) //De bas en haut, puis passe à la colonne de gauche (part de droite)
        {
                for(i=0; i<h; i++)
                {
                        if(s[i][j] == 0)
                        {
                                point_droite_trouve = true;
                                break;
                        }
                }
                if(point_droite_trouve == true) break;
        }

        x_droite = j;
        //y_droite = i;
        //printf("\nx_droite : %d\n", x_droite); printf("y_droite : %d\n", y_droite);

//on a maintenant les différents points permettant de réduire la matrice
//nous n'avons plus qu'à allouer la memoire necessaire...
	*petite_h = (y_dessus-y_dessous)+1;
	*petite_l = (x_droite-x_gauche)+1;
	
	printf("Taille petite image:\npetite_h = %d et petite_l = %d\n", *petite_h, *petite_l);

	*petite_s = alloueMatrice(*petite_h, *petite_l);
//et y stocker la matrice réduite...	
	for(i=y_dessous ; i<=y_dessus; i++)
	{
		for(j=x_gauche; j<=x_droite; j++)
		{
			(*petite_s)[i-y_dessous][j-x_gauche] = s[i][j]; //remplissage petite matrice
		}
	}
	
//il faut maintenant créer une image à partir de la nouvelle matrice...
	*petite_image = creeImage(*petite_h, *petite_l, *petite_s, *petite_s, *petite_s);

//on enregistre dans la zone sa taille
	zPetiteI->xmax = zPetiteI->xmin + *petite_l;
	zPetiteI->ymin = zPetiteI->ymax - *petite_h;
	
	//printf("Zxmin:%d\nZxmax:%d\nZymin:%d\nZymax:%d\n", zPetiteI->xmin, zPetiteI->xmax, zPetiteI->ymin, zPetiteI->ymax);

	
}


