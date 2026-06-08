#ifndef AES_H
#define AES_H
#define Nb 4

/*
Byte = Octet =  unsigned char (unsigned pour avoir les opérations de décalage shift )
Word = 32 bits = un tableau de 4 octets. 
State = un tableau de 4 word
Nb=4 pour AES
*/
//Définition des types byte,word,state

typedef unsigned char byte;
typedef byte word[NB];
typedef byte state[Nb][Nb];

void SubWord(word w);
void RotWord(word w);
void KeyExpansion(byte key[], word w[], int key_length);
void SubBytes(state s);
void ShiftRows(state s);
void MixColumns(state s);
void AddRoundKey(state s, word round_key[Nb]);
void InvSubBytes(state s);
void InvShiftRows(state s);
void InvMixColumns(state s);
void aes_encrypt(state dst, state src, word key[], int key_length);
void aes_decrypt(state dst, state src, word key[], int key_length);

#endif
