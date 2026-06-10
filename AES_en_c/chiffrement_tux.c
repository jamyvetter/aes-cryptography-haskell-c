#include "bmp.h"
#include "ECB.h"
#include <stddef.h>

int main(){
  bmp_image *img, *img2,*img3;
  chunk *h,  *h2,*h3, *p, *p2,*p3;

  /* Chargement de l'image en mémoire */
  img = bmp_read("Tux.bmp");

  /* Récupération de l'en-tête et des pixels */
  h = bmp_get_header(img);
  p = bmp_get_pixels(img);

//Clef (on peut la changer ici): //--------------------

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
/*word key[4] = {
    {0xfe, 0x01, 0x02, 0x03},
    {0x04, 0xaa, 0x06, 0x08},
    {0x08, 0xec, 0x1a, 0x0b} ,
    {0x0c, 0x0d, 0x0e, 0x0b }
  };
  int key_size=4;
  */
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

  //Destination:
  int dst_data_length=0;
  unsigned char* dst= NULL; //Sera alloué avec un malloc dans ECB

  int taille_pixels_src = chunk_get_size(p);
  unsigned char* pointeur_chunk_src= (unsigned char*)chunk_get_data(p); //(transypage pour avoir le 'unsigned' requis par aes_encrypt)

  aes_ecb_encrypt(&dst, &dst_data_length, pointeur_chunk_src, taille_pixels_src, key, key_size); // Chiffre les pixels avec ECB

  p2= as_chunk((char *) dst, dst_data_length);// Création d'un bloc à partir du pointeur contenant le chiffrement des pixels de l'image

  h2 = chunk_copy(h);// Copie de l'entête (non modifiée)
  img2 = bmp_create(h2, p2); //fusion entête-pixels chiffrés

  /* Libération de la mémoire de l'image originale */
  bmp_free(img);

  /* Ecriture de l'image chiffree sur disque */
  bmp_write("Tux_chiffre_256.bmp", img2);

  /* Libération de l'image chiffree */
  //bmp_free(img2);

//--------------------------------------Dechiffrement -------

  //Destination:
  int dst_data_length_u=0;
  unsigned char* dst_u= NULL; //Sera alloué avec un malloc dans ECB

  int taille_pixels_src_u = chunk_get_size(p2);
  unsigned char* pointeur_chunk_src_u= (unsigned char*)chunk_get_data(p2); //(transypage pour avoir le 'unsigned' requis par aes_encrypt)

  //Fonction à utiliser pour chiffre Tux
  aes_ecb_decrypt(&dst_u, &dst_data_length_u, pointeur_chunk_src_u, taille_pixels_src_u, key, key_size); // Chiffre les pixels avec ECB

  p3= as_chunk((char *) dst_u, dst_data_length_u);// Création d'un bloc à partir du pointeur contenant le chiffrement des pixels de l'image

  h3 = chunk_copy(h2);// Copie de l'entête (non modifiée)
  img3 = bmp_create(h3, p3); //fusion entête-pixels déchiffrés

  bmp_free(img2);
  bmp_write("Tux_dechiffre_256.bmp", img3);
  bmp_free(img3);


  // L'image déchiffrée a l'air identique à l'image chiffrée (logique).
  return 0;
}
