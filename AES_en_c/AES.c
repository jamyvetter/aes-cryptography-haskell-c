#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
// The round constant word array Rcon[i] contains the values given by [x^(i-1), {00}, {00}, {00}]
// Les indices vont de 1 à 14 (maximum pour AES-256). L'indice 0 n'est pas utilisé.
const word Rcon[15] = {
    {0x00, 0x00, 0x00, 0x00}, // Rcon[0] (inutilisé)
    {0x01, 0x00, 0x00, 0x00}, // Rcon[1]
    {0x02, 0x00, 0x00, 0x00}, // Rcon[2]
    {0x04, 0x00, 0x00, 0x00}, // Rcon[3]
    {0x08, 0x00, 0x00, 0x00}, // Rcon[4]
    {0x10, 0x00, 0x00, 0x00}, // Rcon[5]
    {0x20, 0x00, 0x00, 0x00}, // Rcon[6]
    {0x40, 0x00, 0x00, 0x00}, // Rcon[7]
    {0x80, 0x00, 0x00, 0x00}, // Rcon[8]
    {0x1b, 0x00, 0x00, 0x00}, // Rcon[9]
    {0x36, 0x00, 0x00, 0x00}, // Rcon[10] (AES-128 s'arrête ici)
    {0x6c, 0x00, 0x00, 0x00}, // Rcon[11]
    {0xd8, 0x00, 0x00, 0x00}, // Rcon[12]
    {0xab, 0x00, 0x00, 0x00}, // Rcon[13]
    {0x4d, 0x00, 0x00, 0x00}  // Rcon[14] (AES-256 s'arrête ici)
};


static const byte sbox[256] = {
    0x63,0x7c,0x77, 0x7b,0xf2, 0x6b, 0x6f, 0xc5,0x30, 0x01,  0x67,0x2b, 0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,
    0xc0,
    0xb7,
    0xfd,
    0x93,
    0x26,0x36,0x3f,0xf7,0xcc,0x34,
    0xa5,0xe5,
    0xf1,0x71,
    0xd8,0x31,
    0x15,0x04,
    0xc7,0x23,
    0xc3,0x18,
    0x96,0x05,
    0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,
    0xd1,0x00,
    0xed,0x20,
    0xfc,0xb1,
    0x5b,0x6a,
    0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,0xcd,0x0c,0x13,
    0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,
    0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,
    0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,0xba,0x78,0x25,0x2e,0x1c,
    0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

//Auxiliaire
int val_nr(int Nk){ //donne la valeur de Nr pour Nk=key_length. Nb=4 est fixe
    if (Nk==4) return 10;
    if (Nk==6) return 12;
    if (Nk==8) return 14;
    return 99;//code d'erreur pour appeler exit()
}

void SubWord(word w){ //substitue la valeur de l'octet à partir de sa valeur et la Sbox
    for(int i=0;i<4;i++){
        w[i]=sbox[w[i]];
    }
}

void RotWord(word w){ //Prend un word [a,b,c,d]-> fait [b,c,d,a]
    byte temp;
    temp=w[0];
    w[0]=w[1];w[1]=w[2];w[2]=w[3];w[3]=temp;
}

//--------Fonction auxiliaire
void xor_word(byte a[4], byte b[4],byte dst[4]){ //modifie dst, fait le xor octet par octet pour des word
    for (int i=0;i<4;i++){
        dst[i]= a[i] ^ b[i];
    }
}

void KeyExpansion(byte key[], word w[],int key_length){
    int Nk=key_length;
    word temp;
    int i=0;
    while (i< Nk){
        for (int u=0;u<4;u++){ // modification de w[i]
            w[i][u] = key[4*i+u];
        }
        i++;
    }
    i=Nk;
    while (i<NB*(val_nr(Nk)+1)){
        for (int u=0;u<4;u++){ // modification de temp par w[i-1]
            temp[u] = w[i-1][u];
        }
        if(i%Nk==0) {
            RotWord(temp);SubWord(temp);
            xor_word(temp,Rcon[i/Nk],temp);//temp^= Rcon[i/Nk];
        }
        else if(Nk>6 && i%Nk==4) SubWord(temp);
        xor_word(w[i-Nk],temp,w[i]); //w[i] = w[i-Nk] ^ temp;
        i++;
    }
}

void ShiftRows(state s){
    for (int i=1;i<4;i++){
        for(int j=0;j<i;j++) RotWord(s[i]);
    }
}

void AddRoundKey(state s, word round_key[4]){
    for (int i = 0; i < 4; i++) {
        for (int c = 0; c < 4; c++) {
            s[c][i] ^= round_key[i][c];
        }
    }
}

void SubBytes(state s) {
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            s[i][j]= sbox[s[i][j]];
        }
    }
} 

// Definition de la notation xtime (multiplication par le polynome X => décalage a gauche)
byte xtime(byte x) {
    return (x << 1) ^ (((x & 0x80) != 0) ? 0x1b : 0x00); //si le bit de poids fort était à 1 on xor avec 0x1b sinon on decale tout simplement
}

void MixColumns(state s) {
    for (int c = 0; c < 4; c++) {
        byte s0 = s[0][c], s1 = s[1][c], s2 = s[2][c], s3 = s[3][c];
        s[0][c] = xtime(s0) ^ (xtime(s1) ^ s1) ^ s2 ^ s3;
        s[1][c] = s0 ^ xtime(s1) ^ (xtime(s2) ^ s2) ^ s3;
        s[2][c] = s0 ^ s1 ^ xtime(s2) ^ (xtime(s3) ^ s3);
        s[3][c] = (xtime(s0) ^ s0) ^ s1 ^ s2 ^ xtime(s3);
    }
}


void InvSubBytes(state s) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            byte current = s[i][j];
            for (int k = 0; k < 256; k++) {
                if (sbox[k] == current) {s[i][j] = (byte)k; break; }
            }
        }
    }
}

//Super long, il faudrait utiliser reverse-Sbox


void InvShiftRows(state s){
    for (int i=1;i<NB;i++){
        for (int j=0;j<NB-i;j++){
            RotWord(s[i]);
}
}
}

void InvMixColumns(state s) {
    for (int c=0;c<NB; c++) {
        byte s0=s[0][c], s1=s[1][c], s2=s[2][c], s3=s[3][c];
        
        // 14=0x0E (x^3+x^2+x), 11=0x0B (x^3+x+1), 13=0x0D (x^3+x^2+1), 9=0x09 (x^3+1)
        s[0][c] = (xtime(xtime(xtime(s0))) ^ xtime(xtime(s0)) ^ xtime(s0)) ^ //0x0E * s0 
                  (xtime(xtime(xtime(s1))) ^ xtime(s1) ^ s1) ^ //0x0B * s1 
                  (xtime(xtime(xtime(s2))) ^ xtime(xtime(s2)) ^ s2) ^ //0x0D * s2 
                  (xtime(xtime(xtime(s3))) ^ s3); //0x09 * s3
                  
        s[1][c] = (xtime(xtime(xtime(s0))) ^ s0) ^ //0x09 * s0
                  (xtime(xtime(xtime(s1))) ^ xtime(xtime(s1)) ^ xtime(s1)) ^ //0x0E * s1 
                  (xtime(xtime(xtime(s2))) ^ xtime(s2) ^ s2) ^ //0x0B * s2 
                  (xtime(xtime(xtime(s3))) ^ xtime(xtime(s3)) ^ s3); //0x0D * s3
                  
        s[2][c] = (xtime(xtime(xtime(s0))) ^ xtime(xtime(s0)) ^ s0) ^ //0x0D * s0
                  (xtime(xtime(xtime(s1))) ^ s1) ^ //0x09 * s1
                  (xtime(xtime(xtime(s2))) ^ xtime(xtime(s2)) ^ xtime(s2)) ^ //0x0E * s2
                  (xtime(xtime(xtime(s3))) ^ xtime(s3) ^ s3); //0x0B * s3
                  
        s[3][c] = (xtime(xtime(xtime(s0))) ^ xtime(s0) ^ s0) ^ //0x0B * s0
                  (xtime(xtime(xtime(s1))) ^ xtime(xtime(s1)) ^ s1) ^ //0x0D * s1
                  (xtime(xtime(xtime(s2))) ^ s2) ^ //0x09 * s2
                  (xtime(xtime(xtime(s3))) ^ xtime(xtime(s3)) ^ xtime(s3)); //0x0E * s3
    }
}


//Auxiliaire, fait la copie d'un word dans un autre.
// Faire memcpy plus tard plus rapide //
void copie_word(byte src[4],byte dst[4]){
    for(int i=0;i<4;i++){   
        dst[i]=src[i];
    }
}

//Auxiliaire, copie un state dans un autre
void copie_state(state src, state dst){
    memcpy(dst,src,16);
}

//Cipher, chiffre le message à partir de la clef étendue
void Cipher(state dst, state src, word key[], int key_length){
    int Nr=val_nr(key_length);
    copie_state(src,dst);//copie de src dans dst, puis on modifie dst
    AddRoundKey(dst, key);
    for (int round=1;round<Nr;round++){
        SubBytes(dst);ShiftRows(dst);MixColumns(dst);
        AddRoundKey(dst, &key[round*4]);
    }
    SubBytes(dst);ShiftRows(dst);
    AddRoundKey(dst, &key[Nr*NB]);
}


void aes_encrypt(state dst, state src, word key[], int key_length){
    int Nr=val_nr(key_length);
    if (Nr==99) exit(EXIT_FAILURE);

    word expanded_key[NB*(Nr+1)];
    KeyExpansion(key,expanded_key,key_length); //Chiffrement de la clef (KeyExpansion)
    Cipher(dst,src,expanded_key,key_length); //Encodage avec la clef chiffrée
}

void InvCipher(state dst, state src, word w[], int Nk){
// key_length permet toujours de déduire Nk et Nr. Si key_length est incorrect, il faut appeler exit()
    int Nr=val_nr(Nk);
    copie_state(src,dst);//copie de src dans dst, puis on modifie dst
    AddRoundKey(dst,&w[Nr*NB]);
    for(int round=Nr-1;round>0;round--){ 
        InvShiftRows(dst);
        InvSubBytes(dst);
        AddRoundKey(dst,&w[round*NB]);        
        InvMixColumns(dst);
    }
    InvShiftRows(dst);
    InvSubBytes(dst);
    AddRoundKey(dst,&w[0]);    
}

void aes_decrypt(state dst, state src, word key[], int key_length){
    int Nr=val_nr(key_length);
    if (Nr==99) exit(EXIT_FAILURE);

    word expanded_key[NB*(Nr+1)];
    KeyExpansion(key,expanded_key,key_length); //Chiffrement de la clef (KeyExpansion)
    InvCipher(dst,src,expanded_key,key_length); // Dechiffrement en utilisant la clef étendue

}
void InvMixColumns_eq(word s[NB]) {
    //Comme l'entrée est 'inversée' , le rôle de mixColumns est inversée aussi, on échange juste lignes et colonnes
    for (int c = 0; c < NB; c++) {
        byte s0 = s[c][0], s1 = s[c][1], s2 = s[c][2], s3 = s[c][3];
        
        s[c][0] = (xtime(xtime(xtime(s0))) ^ xtime(xtime(s0)) ^ xtime(s0)) ^ 
                  (xtime(xtime(xtime(s1))) ^ xtime(s1) ^ s1) ^ 
                  (xtime(xtime(xtime(s2))) ^ xtime(xtime(s2)) ^ s2) ^ 
                  (xtime(xtime(xtime(s3))) ^ s3); 
                  
        s[c][1] = (xtime(xtime(xtime(s0))) ^ s0) ^ 
                  (xtime(xtime(xtime(s1))) ^ xtime(xtime(s1)) ^ xtime(s1)) ^ 
                  (xtime(xtime(xtime(s2))) ^ xtime(s2) ^ s2) ^ 
                  (xtime(xtime(xtime(s3))) ^ xtime(xtime(s3)) ^ s3); 
                  
        s[c][2] = (xtime(xtime(xtime(s0))) ^ xtime(xtime(s0)) ^ s0) ^ 
                  (xtime(xtime(xtime(s1))) ^ s1) ^ 
                  (xtime(xtime(xtime(s2))) ^ xtime(xtime(s2)) ^ xtime(s2)) ^ 
                  (xtime(xtime(xtime(s3))) ^ xtime(s3) ^ s3); 
                  
        s[c][3] = (xtime(xtime(xtime(s0))) ^ xtime(s0) ^ s0) ^ 
                  (xtime(xtime(xtime(s1))) ^ xtime(xtime(s1)) ^ s1) ^ 
                  (xtime(xtime(xtime(s2))) ^ s2) ^ 
                  (xtime(xtime(xtime(s3))) ^ xtime(xtime(s3)) ^ xtime(s3)); 
    }
}
//Cette version génère la clef de base et celle utilisée pour l'eqCipherInv(dw)
void EqKeyExpansion(byte key[], word w[],word dw[],int key_length){
    int Nk=key_length,Nr=val_nr(Nk);
    word temp;
    int i=0;
    while (i< Nk){
        for (int u=0;u<4;u++){ // modification de w[i]
            w[i][u] = key[4*i+u];
        }
        i++;
    }
    i=Nk;
    while (i<NB*(val_nr(Nk)+1)){
        for (int u=0;u<4;u++){ // modification de temp par w[i-1]
            temp[u] = w[i-1][u];
        }
        if(i%Nk==0) {
            RotWord(temp);SubWord(temp);
            xor_word(temp,Rcon[i/Nk],temp);//temp^= Rcon[i/Nk];
        }
        else if(Nk>6 && i%Nk==4) SubWord(temp);
        xor_word(w[i-Nk],temp,w[i]); //w[i] = w[i-Nk] ^ temp;
        i++;
    }
    for(int i=0;i<(Nr+1)*NB;i++){
        copie_word(w[i],dw[i]);//dw[i]=w[i];
    }
    for(int round=1;round<Nr;round++){
        InvMixColumns_eq(&dw[round*NB]);//transtypage ici -> Erreur ici, il faut gérer le fait que les lignes et colonnes sont inversées
        // Car InvMixcolumns s'applique sur un State et pas un tableau de 16 octets
    }
}
void EqInvCipher(state dst, state src, word dw[], int key_length){
    int Nr=val_nr(key_length);
    copie_state(src,dst);//copie de src dans dst, puis on modifie dst
    AddRoundKey(dst,&dw[Nr*NB]);
    for(int round=Nr-1;round>0;round--){ 
        InvSubBytes(dst);
        InvShiftRows(dst);       
        InvMixColumns(dst);
        AddRoundKey(dst,&dw[round*NB]); 
    }
    InvSubBytes(dst);InvShiftRows(dst);
    AddRoundKey(dst,&dw[0]);
}

//Cette version utilise l'alogirthme équivalent Cipher.
void aes_decrypt2(state dst, state src, word key[], int key_length){
    int Nr=val_nr(key_length);
    if (Nr==99) exit(EXIT_FAILURE);

    word dw[NB*(Nr+1)]; word expanded_key[NB*(Nr+1)];
    EqKeyExpansion(key,expanded_key,dw,key_length); //Chiffrement de la clef (KeyExpansion)
    EqInvCipher(dst,src,dw,key_length); // Dechiffrement en utilisant la clef étendue  
}


//Fonction auxiliaire
void afficher_state(state s){ // Affiche sous format matrice, avec chaque octet sous forme de nombre hexadécimal
    for (int i = 0; i < NB; i++) {
        for (int j = 0; j < NB; j++) {
            printf(" %02x",s[i][j]); //%02 permet d'afficher 2 caratères (enlève le problème d'affichage sur 5 par exemple)   
        }
        printf("\n");
    }
}


