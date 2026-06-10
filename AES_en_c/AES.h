#ifndef AES_H
#define AES_H
#define NB 4

/* //Définition des types byte,word,state
Byte = Octet =  unsigned char (unsigned pour avoir les opérations de décalage shift )
Word = 32 bits = un tableau de 4 octets. 
State = un tableau de 4 word
Nb=4 pour AES
*/
int val_nr(int Nk); //donne la valeur de Nr pour Nk=key_length. Nb=4 est fixe (fonction auxiliaire)

typedef unsigned char byte;
typedef byte word[NB];
typedef byte state[NB][NB];

void SubWord(word w);
void RotWord(word w);

void SubBytes(state s);
void ShiftRows(state s);
void MixColumns(state s);
void AddRoundKey(state s, word round_key[NB]);

void InvSubBytes(state s);
void InvShiftRows(state s);
void InvMixColumns(state s);

void KeyExpansion(byte key[], word w[], int key_length);
void EqKeyExpansion(byte key[], word w[],word dw[],int key_length);

void Cipher(state dst, state src, word key[], int key_length);
void InvCipher(state dst, state src, word w[], int Nk);
void EqInvCipher(state dst, state src, word dw[], int key_length);

void aes_encrypt(state dst, state src, word key[], int key_length);
void aes_decrypt(state dst, state src, word key[], int key_length);
void aes_decrypt2(state dst, state src, word key[], int key_length);

#endif
