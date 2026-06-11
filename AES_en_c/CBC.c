#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"


//unsigned char** dst => adresse du pointeur vers le tableau qui recevra le message chiffre
//int *dst_data_length => adresse du pointeur vers la taille du message chiffré
//unsigned char* src => tableau du message clair complet
//int src_data_length => taille du message clair
//unsigned char* iv => vecteur d'initialisation
//word key[] => clé non etendue
//int key_length => taille de la clé (Nk: 4, 6, ou 8)



int aes_cbc_encrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, unsigned char* iv, word key[], int key_length) {
    if (dst==NULL || dst_data_length==NULL || src==NULL || key==NULL || iv==NULL) return -1;
    int padding_len=16-(src_data_length%16);
    *dst_data_length=src_data_length + padding_len;
    *dst=(unsigned char*)malloc(*dst_data_length);
    if (*dst==NULL) return -1; // echec allocation
    for (int i=0;i<src_data_length;i++){
        (*dst)[i]=src[i];
    }
    for (int i=src_data_length;i<*dst_data_length;i++){
        (*dst)[i]=(unsigned char)padding_len; }
    int Nr=val_nr(key_length);//calcul du nombre de tour selon la taille de la clé
    if (Nr==99) exit(EXIT_FAILURE); 
    word expanded_key[NB*(Nr+1)]; //creation d'un tableau pour stocker toutes les clés
    KeyExpansion((byte*)key,expanded_key,key_length); 
    unsigned char previous_block[16]; // initialisation du bloc précédent avec l'IV
    for(int i=0;i<16;i++){
        previous_block[i]=iv[i];
}
    for (int saut=0;saut<*dst_data_length;saut+=16){
        state in_state,out_state;
        for (int i=0;i<16;i++){
            int ligne=i%4;
            int col=i/4;
            in_state[ligne][col]=((*dst)[saut+i])^(previous_block[i]); //XOR entre le texte clair et le bloc chiffré précédent (IV lors du premier tour)
        }
        Cipher(out_state, in_state, expanded_key, key_length); //utilisation du tableau précalculé lors du chiffrement=>gain de temps
        for (int i=0;i<16;i++){
            int ligne=i%4;
            int col=i/4;
            (*dst)[saut+i]=out_state[ligne][col];
            previous_block[i]=out_state[ligne][col]; 
        }
}
    return 0;
}


int aes_cbc_decrypt(unsigned char** dst,int *dst_data_length,unsigned char* src,int src_data_length,unsigned char* iv,word key[],int key_length) {
    if (dst==NULL || dst_data_length==NULL || src==NULL || key==NULL || iv==NULL) return -1;
    if (src_data_length==0 || src_data_length%16!=0) return -1;
    *dst=(unsigned char*)malloc(src_data_length);
    if (*dst==NULL) return -1;
    int Nr=val_nr(key_length); //calcul du nombre de tour selon la taille de la clé
    if (Nr==99) exit(EXIT_FAILURE); 
    word dw[NB*(Nr+1)]; 
    word expanded_key[NB*(Nr+1)];
    EqKeyExpansion((byte*)key,expanded_key,dw,key_length); //création des clés pour dechiffrement dans dw (descryption word)
    unsigned char previous_block[16]; // initialisation du bloc précédent avec l'IV
    for(int i=0;i<16;i++){
        previous_block[i]=iv[i];
    }
    for (int saut=0;saut<src_data_length;saut+=16){
        state in_state, out_state;
        unsigned char current_cipher_block[16]; //sauvegarde du bloc chiffré actuel avant déchiffrement
        for (int i=0;i<16;i++){
            int ligne=i%4; 
            int col=i/4;
            in_state[ligne][col]=src[saut+i];
            current_cipher_block[i]=src[saut+i]; 
        }
        //on lit le bloc chiffré suivant depuis src, et on le conserve. Pendant ce temps previous bloc contient toujours le bloc chiffré precedent (ou l'IV)
        EqInvCipher(out_state,in_state,dw,key_length);
        for (int i=0;i<16;i++) {
            int r=i%4;
            int c=i/4;
            (*dst)[saut+i]=out_state[r][c]^previous_block[i]; //XOR entre le résultat déchiffré brut et le bloc précédent
            previous_block[i]=current_cipher_block[i]; // Le bloc chiffré actuel devient le bloc précédent 
        }
    }
    int padding_len=(*dst)[src_data_length-1];
    if (padding_len == 0 || padding_len > 16){ //si padding est faux
        free(*dst);
        *dst = NULL;
        return -1;
    }
    for (int i=1;i<=padding_len;i++) {
        if ((*dst)[src_data_length-i]!=padding_len){
            free(*dst);
            *dst=NULL;
            return -1;
        }
    }
    *dst_data_length=src_data_length-padding_len;
    return 0;
}