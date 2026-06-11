// Fichier pour le calcul de l'entropie d'une image au format bmp.
//Utilisation : ./entropie <file_name>
#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc !=2){
        printf("Utilisation : %s <file_name>\n",argv[0]);
        return 1;
    }

  bmp_image *img ;
  chunk *p;

  /* Chargement de l'image en mémoire */
  img = bmp_read(argv[1]);

  /* Récupération des pixels */
  p = bmp_get_pixels(img);

  int taille_pixels_image= chunk_get_size(p);
  unsigned char* pixels_data= (unsigned char*) chunk_get_data(p);

  int occurences[256]={0}; //chaque case va contenir le nombre de fois que ce pixel est vu dans l'image

  for (int i=0; i< taille_pixels_image; i++){
    occurences[pixels_data[i]]++;
  }

  double entropie=0.0;

  for (int i=0; i< 256; i++){
    if (occurences[i]>0){//Au cas où un pixel n'est pas représenté dans l'image (divison par 0
        double P = (double)occurences[i]/taille_pixels_image; //proba de voir ce pixel dans l'image
        entropie -= P * log2(P);
    }
  }

  printf("Fichier :%s , Entropie : %f \n",argv[1],entropie);
 
  bmp_free(img); /* Libération de la mémoire de l'image originale */


  return 0;
}
