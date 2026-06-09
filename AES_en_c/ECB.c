#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"

// unsigned char* src => tableau du message en clair complet
// int src_data_length => taille du message en clair
// word key[] => la clé etendue (w)
// int key_length => taille de la clé (Nk: 4, 6, ou 8)
// unsigned char** dst => adresse d'un pointeur
// int *dst_data_length => pointeur vers un entier qui représente la taille du resultat (multiple de 16)

int aes_ecb_encrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, word key[], int key_length) {
    if (dst==NULL || dst_data_length==NULL || src==NULL || key==NULL) return -1; //si un pointeur fourni est vide arret de la fonction
    int padding_len =16-(src_data_length % 16); // calcul de la taille avec le Padding PKCS#7
    *dst_data_length=src_data_length+padding_len;
    *dst = (unsigned char*)malloc(*dst_data_length); //allocation de memoire de la taille necessaire calculée avec le padding
    if (*dst == NULL) return -1;
    for (int i=0;i<src_data_length;i++) { //copie du message clair dans le tableau *dst
        (*dst)[i]=src[i];
    }
    for (int i=src_data_length;i<*dst_data_length;i++){ //remplissage du tableau (padding)
        (*dst)[i]=(unsigned char)padding_len;
    }
    for (int saut=0;saut<*dst_data_length;saut+=16){ //traitement des blocs de 16 octets
        state in_state, out_state;
        //convertion tableau 1D en tableau 2D colonne par colonne (car sinon stocké ligne par ligne dans le langage C)
        for (int i=0;i<16;i++){
            int r=i%4; //restedonne la ligne
            int c=i/4; //quotient donne la colonne 
            in_state[r][c]=(*dst)[saut+i];
        }
        aes_encrypt(out_state, in_state, key, key_length);
        for (int i= 0;i<16;i++) {
            int r=i%4;
            int c=i/4; 
            (*dst)[saut + i] = out_state[r][c];
        }
    }
    return 0;
}

int aes_ecb_decrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, word key[], int key_length) {
    if (dst==NULL || dst_data_length==NULL || src==NULL || key==NULL) return -1;
    if (src_data_length == 0 || src_data_length % 16!=0) return -1; //il faut que la taille du message chiffré soit un multiple de 16 car il a été paddé
    *dst = (unsigned char*)malloc(src_data_length); //allocation de la taille du message chiffré
    if (*dst == NULL) return -1;
    for (int saut=0; saut<src_data_length; saut+=16){ //dechiffrement par bloc de 16
        state in_state, out_state;
        for (int i=0;i<16;i++){
            int r=i%4; 
            int c=i/4;
            in_state[r][c]=src[saut+i]; }
        aes_decrypt(out_state,in_state,key,key_length);
        //convertion tableau 2D en chaine de caractere
        for (int i=0;i<16;i++){
            int r=i%4;
            int c=i/4;
            (*dst)[saut+i]=out_state[r][c];
        }
}
    int padding_len = (*dst)[src_data_length - 1]; //le dernier octet indique le nombre d'octets qui ont etes ajouté lors du chiffrement => a supprimer
    
    // Vérification de sécurité du padding
    if (padding_len == 0 || padding_len > 16){ //si padding est faux
        free(*dst);
        *dst=NULL;
        return -1;
    }
    for (int i=1;i<=padding_len;i++) { //si deux valeurs consecutives sont differentes a la fin du message => erreur de padding
        if ((*dst)[src_data_length - i] != padding_len) {
            free(*dst);
            *dst = NULL;
            return -1;
    }
    }
    *dst_data_length = src_data_length - padding_len; //vrai taillesans le padding
    return 0;
}
