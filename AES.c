#include <stdio.h>

/*
Byte = Octet =  unsigned char (unsigned pour avoir les opérations de décalage shift )
Word = 32 bits = un tableau de 4 octets. 
State = 4*word
Nb=4 pour AES
*/
//Définition des types byte,word,state
typedef unsigned char byte;
typedef byte word[4];
typedef byte state[4][4]; // ou un tableau de 4 word : typedef word state[4]

word Rcon[99]; // The round constant word array

static const byte sbox[256] = {
    0x63,0x7c,0x77, 0x7b,0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01,  0x67,
    0x2b, 0xfe,
    0xd7,
    0xab,0x76,
    0xca,
    0x82,
    0xc9,
    0x7d,
    0xfa,
    0x59,
    0x47,
    0xf0,
    0xad,
    0xd4,
    0xa2,
    0xaf,
    0x9c,
    0xa4,
    0x72,
    0xc0,
    0xb7,
    0xfd,
    0x93,
    0x26,
    0x36,
    0x3f,
    0xf7,
    0xcc,
    0x34,
    0xa5,
    0xe5,
    0xf1,
    0x71,
    0xd8,
    0x31,
    0x15,
    0x04,
    0xc7,
    0x23,
    0xc3,
    0x18,
    0x96,
    0x05,
    0x9a,
    0x07,
    0x12,
    0x80,
    0xe2,
    0xeb,
    0x27,
    0xb2,
    0x75,
    0x09,
    0x83,
    0x2c,
    0x1a,
    0x1b,
    0x6e,
    0x5a,
    0xa0,
    0x52,
    0x3b,
    0xd6,
    0xb3,
    0x29,
    0xe3,
    0x2f,
    0x84,
    0x53,
    0xd1,
    0x00,
    0xed,
    0x20,
    0xfc,
    0xb1,
    0x5b,
    0x6a,
    0xcb,
    0xbe,
    0x39,
    0x4a,
    0x4c,
    0x58,
    0xcf,
    0xd0,
    0xef,
    0xaa,
    0xfb,
    0x43,
    0x4d,
    0x33,
    0x85,
    0x45,
    0xf9,
    0x02,
    0x7f,
    0x50,
    0x3c,
    0x9f,
    0xa8,
    0x51,
    0xa3,
    0x40,
    0x8f,
    0x92,
    0x9d,
    0x38,
    0xf5,
    0xbc,
    0xb6,
    0xda,
    0x21,
    0x10,
    0xff,
    0xf3,
    0xd2,
    0xcd,
    0x0c,
    0x13,
    0xec,
    0x5f,
    0x97,
    0x44,
    0x17,
    0xc4,
    0xa7,
    0x7e,
    0x3d,
    0x64,
    0x5d,
    0x19,
    0x73,
    0x60,
    0x81,
    0x4f,
    0xdc,
    0x22,
    0x2a,
    0x90,
    0x88,
    0x46,
    0xee,
    0xb8,
    0x14,
    0xde,
    0x5e,
    0x0b,
    0xdb,
    0xe0,
    0x32,
    0x3a,
    0x0a,
    0x49,
    0x06,
    0x24,
    0x5c,
    0xc2,
    0xd3,
    0xac,
    0x62,
    0x91,
    0x95,
    0xe4,
    0x79,
    0xe7,
    0xc8,
    0x37,
    0x6d,
    0x8d,
    0xd5,
    0x4e,
    0xa9,
    0x6c,
    0x56,
    0xf4,
    0xea,
    0x65,
    0x7a,
    0xae,
    0x08,
    0xba,
    0x78,
    0x25,
    0x2e,
    0x1c,
    0xa6,
    0xb4,
    0xc6,
    0xe8,
    0xdd,
    0x74,
    0x1f,
    0x4b,
    0xbd,
    0x8b,
    0x8a,
    0x70,
    0x3e,
    0xb5,
    0x66,
    0x48,
    0x03,
    0xf6,
    0x0e,
    0x61,
    0x35,
    0x57,
    0xb9,
    0x86,
    0xc1,
    0x1d,
    0x9e,
    0xe1,
    0xf8,
    0x98,
    0x11,
    0x69,
    0xd9,
    0x8e,
    0x94,
    0x9b,
    0x1e,
    0x87,
    0xe9,
    0xce,
    0x55,
    0x28,
    0xdf,
    0x8c,
    0xa1,
    0x89,
    0x0d,
    0xbf,
    0xe6,
    0x42,
    0x68,
    0x41,
    0x99,
    0x2d,
    0x0f,
    0xb0,
    0x54,
    0xbb,
    0x16

};
void SubWord(word w){ //substitue la valeur de l'octet à partir de sa valeur et la Sbox
    for(int i=0;i<3;i++){
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

//peut on faire mieux sans fournir Nr à KeyExpansion ?
void KeyExpansion(byte key[], word w[],int Nr, int key_length){ //la taille du tableau w change la donne -> info grace à Nr
    int Nk=key_length,Nb=4; 
    word temp;
    int i=0;
    while (i< Nk){
        for (int u=0;u<4;u++){ // modification de w[i]
            w[i][u] = key[4*i+u];
        }
        i++;
    }
    i=Nk;
    while (i<Nb*(Nr+1)){
        for (int u=0;u<4;u++){ // modification de temp par w[i-1]
            temp[u] = w[i-1][u];
        }
        if(i%Nk==0) {
            RotWord(temp);SubWord(temp);
            xor_word(temp,Rcon[i/Nk],temp);//temp^= Rcon[i/Nk];
        }
        else if(Nk>6 && i%Nk==4) SubWord(temp);
        xor_word(w[i-Nk],temp,w[i]);
        //w[i] = w[i-Nk] ^ temp;
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


#define mul2(x) (byte)(((x) << 1) ^ (((x) & 0x80) ? 0x1b : 0x00)) // ici mul2 est la mulplication l'octet (polynome) par x, donc on décale d'un cran vers la gauche (x << 1), si le bit de poids fort est à 1, on applique un XOR avec le polynome irreductible Ox11b
#define mul3(x) (byte)(mul2(x) ^ (x)) // multiplier par 0x03 revient a multiplier par X+1,
#define mul4(x) (byte)(mul2(mul2(x)))
#define mul8(x) (byte)(mul2(mul4(x)))

#define mul9(x) (byte)(mul8(x) ^ (x))
#define mul11(x) (byte)(mul8(x) ^ mul2(x) ^ (x))
#define mul13(x) (byte)(mul8(x) ^ mul4(x) ^ (x))
#define mul14(x) (byte)(mul8(x) ^ mul4(x) ^ mul2(x))

void MixColumns(state s) {
    for (int c = 0; c < 4; c++) {
        byte s0=s[0][c];
        byte s1=s[1][c];
        byte s2 = s[2][c];
        byte s3 = s[3][c];
        s[0][c] = mul2(s0) ^ mul3(s1) ^ s2 ^ s3;
        s[1][c] = s0 ^ mul2(s1) ^ mul3(s2) ^ s3;
        s[2][c] = s0 ^ s1 ^ mul2(s2) ^ mul3(s3);
        s[3][c] = mul3(s0) ^ s1 ^ s2 ^ mul2(s3);
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



void InvShiftRows(state s) {
    for (int i=1;i<4;i++) {
        for (int j=0;j<4-i;j++) {
            RotWord(s[i]);
        }
    }
}

void InvMixColumns(state s) {
    for (int c = 0; c < 4; c++) {
        byte s0=s[0][c];
        byte s1=s[1][c];
        byte s2=s[2][c];
        byte s3=s[3][c];
        s[0][c] = mul14(s0) ^ mul11(s1) ^ mul13(s2) ^ mul9(s3);
        s[1][c] = mul9(s0)  ^ mul14(s1) ^ mul11(s2) ^ mul13(s3);
        s[2][c] = mul13(s0) ^ mul9(s1)  ^ mul14(s2) ^ mul11(s3);
        s[3][c] = mul11(s0) ^ mul13(s1) ^ mul9(s2)  ^ mul14(s3);
    }
}

int val_nr(int Nk){ //donne la valeur de Nr pour Nk=key_length. Nb=4 est fixe
    if (Nk==4) return 10;
    if (Nk==6) return 12;
    else return 14;
}


//Cipher
void aes_encrypt(state dst, state src, word key[], int key_length){
    state temp; int Nr=val_nr(key_length);

    //copie de src dans dst, puis on modifie dst

    AddRoundKey(dst, key);
    for (int round=1;round<Nr;round++){
        SubBytes(dst);ShiftRows(dst);MixColumns(dst);
        intervale_4(key,temp,round*4);
        AddRoundKey(dst, temp);
    }
    SubBytes(dst);ShiftRows(dst);
    intervale_4(key,temp,Nr*4); //modifie temp
    AddRoundKey(dst, temp);

}
void aes_decrypt(state dst, state src, word key[], int key_length);
// key_length permet toujours de déduire Nk et Nr. Si key_length est incorrect, il faut appeler exit()



/*
Faire en sorte de créer des cas de tests pour le code C à partir du code Haskell


-fonction copie de word 
-Faire fonction copie de state

*/


// --- Helper pour les tests ---
int compare_state(state s1, state s2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (s1[i][j] != s2[i][j]) return 0;
        }
    }
    return 1;
}

void test_subBytes(){
    state in = {
        {0x19, 0xa0, 0x9a, 0xe9},
        {0x3d, 0xf4, 0xc6, 0xf8},
        {0xe3, 0xe2, 0x8d, 0x48},
        {0xbe, 0x2b, 0x2a, 0x08}
    };
    state expected = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0x27, 0xbf, 0xb4, 0x41},
        {0x11, 0x98, 0x5d, 0x52},
        {0xae, 0xf1, 0xe5, 0x30}
    };
    SubBytes(in);
    if (compare_state(in, expected)) {
        printf("[OK] SubBytes\n");
    } else {
        printf("[ECHEC] SubBytes\n");
    }
}

void test_shiftRows(){
    state in = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0x27, 0xbf, 0xb4, 0x41},
        {0x11, 0x98, 0x5d, 0x52},
        {0xae, 0xf1, 0xe5, 0x30}
    };
    state expected = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0xbf, 0xb4, 0x41, 0x27},
        {0x5d, 0x52, 0x11, 0x98},
        {0x30, 0xae, 0xf1, 0xe5}
    };
    ShiftRows(in);
    if (compare_state(in, expected)) {
        printf("[OK] ShiftRows\n");
    } else {
        printf("[ECHEC] ShiftRows\n");
    }
}

void test_invSubBytes(){
    state in = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0x27, 0xbf, 0xb4, 0x41},
        {0x11, 0x98, 0x5d, 0x52},
        {0xae, 0xf1, 0xe5, 0x30}
    };
    state expected = {
        {0x19, 0xa0, 0x9a, 0xe9},
        {0x3d, 0xf4, 0xc6, 0xf8},
        {0xe3, 0xe2, 0x8d, 0x48},
        {0xbe, 0x2b, 0x2a, 0x08}
    };
    InvSubBytes(in);
    if (compare_state(in, expected)) {
        printf("[OK] InvSubBytes\n");
    } else {
        printf("[ECHEC] InvSubBytes\n");
    }
}

void test_invShiftRows(){
    state in = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0xbf, 0xb4, 0x41, 0x27},
        {0x5d, 0x52, 0x11, 0x98},
        {0x30, 0xae, 0xf1, 0xe5}
    };
    state expected = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0x27, 0xbf, 0xb4, 0x41},
        {0x11, 0x98, 0x5d, 0x52},
        {0xae, 0xf1, 0xe5, 0x30}
    };
    InvShiftRows(in);
    if (compare_state(in, expected)) {
        printf("[OK] InvShiftRows\n");
    } else {
        printf("[ECHEC] InvShiftRows\n");
    }
}

void test_mixColumns(){
    state in = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0xbf, 0xb4, 0x41, 0x27},
        {0x5d, 0x52, 0x11, 0x98},
        {0x30, 0xae, 0xf1, 0xe5}
    };
    state expected = {
        {0x04, 0xe0, 0x48, 0x28},
        {0x66, 0xcb, 0xf8, 0x06},
        {0x81, 0x19, 0xd3, 0x26},
        {0xe5, 0x9a, 0x7a, 0x4c}
    };
    MixColumns(in);
    if (compare_state(in, expected)) {
        printf("[OK] MixColumns\n");
    } else {
        printf("[ECHEC] MixColumns\n");
    }
}

void test_invMixColumns(){
    state in = {
        {0x04, 0xe0, 0x48, 0x28},
        {0x66, 0xcb, 0xf8, 0x06},
        {0x81, 0x19, 0xd3, 0x26},
        {0xe5, 0x9a, 0x7a, 0x4c}
    };
    state expected = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0xbf, 0xb4, 0x41, 0x27},
        {0x5d, 0x52, 0x11, 0x98},
        {0x30, 0xae, 0xf1, 0xe5}
    };
    InvMixColumns(in);
    if (compare_state(in, expected)) {
        printf("[OK] InvMixColumns\n");
    } else {
        printf("[ECHEC] InvMixColumns\n");
    }
}

void test_addRoundKey(){
    state in = {
        {0x32, 0x88, 0x31, 0xe0},
        {0x43, 0x5a, 0x31, 0x37},
        {0xf6, 0x30, 0x98, 0x07},
        {0xa8, 0x8d, 0xa2, 0x34}
    };
    word round_key[4] = {
        {0x2b, 0x7e, 0x15, 0x16},
        {0x28, 0xae, 0xd2, 0xa6},
        {0xab, 0xf7, 0x15, 0x88},
        {0x09, 0xcf, 0x4f, 0x3c}
    };
    state expected = {
        {0x19, 0xa0, 0x9a, 0xe9},
        {0x3d, 0xf4, 0xc6, 0xf8},
        {0xe3, 0xe2, 0x8d, 0x48},
        {0xbe, 0x2b, 0x2a, 0x08}
    };
    AddRoundKey(in, round_key);
    if (compare_state(in, expected)) {
        printf("[OK] AddRoundKey\n");
    } else {
        printf("[ECHEC] AddRoundKey\n");
    }
}

int main(){
    printf("=== Lancement des tests AES ===\n");
    test_subBytes();
    test_shiftRows();
    test_invSubBytes();
    test_invShiftRows();
    test_mixColumns();
    test_invMixColumns();
    test_addRoundKey();
    printf("===============================\n");
    return 0;
}