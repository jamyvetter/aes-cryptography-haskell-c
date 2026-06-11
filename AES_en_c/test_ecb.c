#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
#include "ECB.h"

void test_fonction(char nom_fonction[],int booleen){ // affiche le message d'erreur ou de réussite selon la condition et le nom de la fonction
        printf("[%s] %s\n",booleen ? "OK" : "ECHEC",nom_fonction);
}

void tester_ecb_taille(int taille,const char* nom_test) {
    byte key[16]={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // clé non étendue
    unsigned char *clair=(unsigned char*)malloc(taille); //allocation de memoire pour le message dorigine
    for (int i=0;i<taille;i++){
        clair[i]=(unsigned char)(i%256); //remplit le tableau de nombres qui se suivent (0,1,2,...255,0,1,...)
    }
    unsigned char *chiffre=NULL; //pointeur pour le message chiffré
    int chiffre_len=0; //taille du message chiffré (>taille clair)
    int res_enc=aes_ecb_encrypt(&chiffre,&chiffre_len,clair,taille,(word*)key,4);
    int expected_chiffre_len=taille+(16-(taille%16)); //taille attendue avec padding
    int enc_success=(res_enc==0 && chiffre!=NULL && chiffre_len==expected_chiffre_len && chiffre_len%16==0); //verifie que le chiffrement a reussi
    unsigned char *dechiffre=NULL; //dechiffrement ecb
    int dechiffre_len=0;
    int res_dec=aes_ecb_decrypt(&dechiffre,&dechiffre_len,chiffre,chiffre_len,(word*)key, 4);
    int dec_success=(res_dec==0 && dechiffre!=NULL && dechiffre_len==taille); //verifie que le dechiffrement a reussi
    if (dec_success){
        if (memcmp(clair,dechiffre,taille)!=0) dec_success=0; //comparaison message déchiffré/message clair
    }
    // Affichage des résultats
    char msg_enc[100];
    char msg_dec[100];
    sprintf(msg_enc, "ECB Encrypt (%s - %d octets)",nom_test,taille);
    sprintf(msg_dec, "ECB Decrypt (%s - %d octets)",nom_test,taille);
    test_fonction(msg_enc, enc_success);
    test_fonction(msg_dec, dec_success);
    free(clair);
    if (chiffre) free(chiffre);
    if (dechiffre) free(dechiffre);
}
int main() {
    printf("=== Lancement des tests ECB ===\n");
    
    // Test 1: Multiple de 16
    tester_ecb_taille(32, "Multiple de 16");
    
    // Test 2: Non multiple (20 octets)
    tester_ecb_taille(20, "Non multiple 1");
    
    // Test 3: Non multiple (43 octets)
    tester_ecb_taille(43, "Non multiple 2");
    
    printf("===============================\n");
    return 0;
}