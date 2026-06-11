#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
#include "CBC.h"

void test_fonction(char nom_fonction[], int booleen) { printf("[%s] %s\n", booleen ? "OK" : "ECHEC", nom_fonction); }

void tester_cbc_taille(int taille, const char* nom_test){
    byte key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; //clé de test 128 bits
    unsigned char iv[16]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; //vecteur d'initialisation de 16 octets
    unsigned char *clair= (unsigned char*)malloc(taille); //allocation de mémoire
    for (int i = 0; i < taille; i++) {
        clair[i] = (unsigned char)(i % 256);
    }
    unsigned char *chiffre=NULL; //variable qui recoit le message chiffré
    int chiffre_len=0; //taille du message chiffré (>taille clair)
    int res_enc =aes_cbc_encrypt(&chiffre,&chiffre_len,clair,taille,iv,(word*)key, 4);
    int expected_chiffre_len=taille+(16-(taille%16));
    int enc_success=(res_enc==0 && chiffre!=NULL && chiffre_len==expected_chiffre_len && chiffre_len % 16 == 0); //verification chiffrement
    unsigned char *dechiffre = NULL;
    int dechiffre_len = 0;
    int res_dec = aes_cbc_decrypt(&dechiffre, &dechiffre_len, chiffre, chiffre_len, iv, (word*)key, 4);
    int dec_success = (res_dec == 0 && dechiffre != NULL && dechiffre_len == taille); // verif dechiffremnt
    if (dec_success) {
        if (memcmp(clair, dechiffre, taille) != 0){ //verif octet par octet de clair et dechiffre
            dec_success = 0; //en cas d'echec
        }
    }
    char msg_enc[100];
    char msg_dec[100];
    sprintf(msg_enc,"CBC Encrypt(%s - %d octets)",nom_test,taille);
    sprintf(msg_dec,"CBC Decrypt (%s - %d octets)",nom_test, taille);
    
    test_fonction(msg_enc,enc_success);
    test_fonction(msg_dec,dec_success);
    free(clair);
    if (chiffre) free(chiffre);
    if (dechiffre) free(dechiffre);
}

int main() {
    printf("=== Lancement des tests CBC ===\n");
    
    tester_cbc_taille(32, "Multiple de 16");
    tester_cbc_taille(20, "Non multiple 1");
    tester_cbc_taille(43, "Non multiple 2");
    
    printf("===========================\n");
    return 0;
}
