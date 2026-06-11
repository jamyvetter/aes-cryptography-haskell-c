#include "bmp.h"
#include "CBC.h"
#include <stddef.h>
#include <sys/time.h>
#include <stdio.h>

//Mesure du temps d'éxecution (déjà utilisés pour le projet othello)
double obtenir_temps(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}


//double debut= obtenir_temps();
//double fin= obtenir_temps();
//printf("Temps execution : %.3f secondes, pour un débit de  %f o/s\n", fin -debut, taille_image/(fin-debut));

int main(){
  bmp_image *img, *img2,*img3;
  chunk *h,  *h2,*h3, *p, *p2,*p3;

  /* Chargement de l'image en mémoire */
  img = bmp_read("pictures/Tux.bmp");

  /* Récupération de l'en-tête et des pixels */
  h = bmp_get_header(img); //en tête (taille,largeur,...)
  p = bmp_get_pixels(img); //pixels (data)

//Clef (on peut changer sa taille entre 32 ou 16 octets ici): //--------------------

/*  
word key[4] = {
    {0x00, 0x01, 0x02, 0x03},
    {0x04, 0x05, 0x06, 0x07},
    {0x08, 0x09, 0x0a, 0x0b} ,
    {0x0c, 0x0d, 0x0e, 0x0f }
  };
  int key_size=4;
  */
  //Avec cette clef ci-dessous aléatoire, seulement les couleurs changent

  word key[4] = {
    {0xfe, 0x01, 0x02, 0x03},
    {0x04, 0xaa, 0x06, 0x08},
    {0x08, 0xec, 0x1a, 0x0b} ,
    {0x0c, 0x0d, 0x0e, 0x0b }
  };
  int key_size=4;

  /*
  word key[8] = {
    {0xaa, 0xbb, 0xcc, 0xdd}, 
    {0xee, 0xff, 0x00, 0x11},
    { 0x22, 0x33, 0x44, 0x55},
    { 0x66, 0x77, 0x88, 0x99},
    { 0xaa, 0xbb, 0xcc, 0xdd},
    { 0xee, 0xff, 0x00, 0x11},
    { 0x22, 0x33, 0x44, 0x55},
    { 0x66, 0x77, 0x88, 0x99}
  };
  int key_size = 8; //Clef plus grande, ça fonctionne aussi
  */
  unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  //Destination:
  int dst_data_length=0;
  unsigned char* dst= NULL; //Sera alloué avec un malloc dans CBC

  int taille_pixels_src = chunk_get_size(p);
  unsigned char* pointeur_chunk_src= (unsigned char*)chunk_get_data(p); //(transypage pour avoir le 'unsigned' requis par aes_encrypt)
  
  double debut= obtenir_temps();
  aes_cbc_encrypt(&dst, &dst_data_length, pointeur_chunk_src, taille_pixels_src, iv, key, key_size); // Chiffre les pixels avec CBC
  double fin_chiffrement= obtenir_temps();

  p2= as_chunk((char *) dst, dst_data_length);// Création d'un bloc à partir du pointeur contenant le chiffrement des pixels de l'image

  h2 = chunk_copy(h);// Copie de l'entête (non modifiée)
  img2 = bmp_create(h2, p2); //fusion entête-pixels chiffrés

  /* Libération de la mémoire de l'image originale */
  bmp_free(img);

  /* Ecriture de l'image chiffree sur disque */
  bmp_write("pictures/Tux_chiffre_cbc_128.bmp", img2);

//--------------------------------------Dechiffrement -------

  //Destination:
  int dst_data_length_u=0;
  unsigned char* dst_u= NULL; //Sera alloué avec un malloc dans CBC

  int taille_pixels_src_u = chunk_get_size(p2);
  unsigned char* pointeur_chunk_src_u= (unsigned char*)chunk_get_data(p2); //(transypage pour avoir le 'unsigned' requis par aes_encrypt)
  
  double debut_dechiffrement=obtenir_temps();
  //Fonction à utiliser pour chiffre Tux
  aes_cbc_decrypt(&dst_u, &dst_data_length_u, pointeur_chunk_src_u, taille_pixels_src_u, iv, key, key_size); // Déchiffre les pixels avec CBC
  //NOTE: ici on rechiffre la clef, même si on l'a déjà étendue pour le chiffrement, ce choix est fait pour montrer le temps pris par 
  // un déchiffrement classique à partir d'une clef non étendue
  double fin_dechiffrement =obtenir_temps();
  p3= as_chunk((char *) dst_u, dst_data_length_u);// Création d'un bloc à partir du pointeur contenant le chiffrement des pixels de l'image

  h3 = chunk_copy(h2);// Copie de l'entête (non modifiée)
  img3 = bmp_create(h3, p3); //fusion entête-pixels déchiffrés

  bmp_free(img2);
  bmp_write("pictures/Tux_dechiffre_cbc_128.bmp", img3);   // L'image déchiffrée a l'air identique à l'image originale (logique).
  bmp_free(img3);

  int taille_image= 912054;//octets
  printf("Temps execution chiffrement : %.3f secondes, pour un débit de  %.0f o/s\n",fin_chiffrement-debut, taille_image/(fin_chiffrement-debut));
  printf("Temps execution dechiffrement : %.3f secondes, pour un débit de  %.0f o/s\n",fin_dechiffrement-debut_dechiffrement, taille_image/(fin_dechiffrement-debut_dechiffrement));
  return 0;
}
