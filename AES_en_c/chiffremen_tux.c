#include "bmp.h"

int main(){
  bmp_image *img, *img2;
  chunk *h,  *h2, *p, *p2;

  /* Chargement de l'image en mémoire */
  img = bmp_read("Tux.bmp");

  /* Récupération de l'en-tête et des pixels */
  h = bmp_get_header(img);
  p = bmp_get_pixels(img);

  aes_ecb_encrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, word key[], int key_length); // Chiffre les pixels avec ECB

  /* Copie des blocs en-tête et pixels*/
  h2 = chunk_copy(h);
  p2 = chunk_copy(p);
  img2 = bmp_create(h2, p2);

  /* Libération de la mémoire de l'image originale */
  bmp_free(img);

  /* Ecriture de l'image img2 sur disque */
  bmp_write("Tux2.bmp", img2);

  /* Libération de img2 */
  bmp_free(img2);

  return 0;
}
