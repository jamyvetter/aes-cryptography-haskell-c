#ifndef AES_H
#define AES_H

typedef unsigned char byte;
typedef byte word[4];
typedef byte state[4][4];

void SubWord(word w);
void RotWord(word w);
void KeyExpansion(byte key[], word w[], int Nr, int key_length);
void SubBytes(state s);
void ShiftRows(state s);
void MixColumns(state s);
void AddRoundKey(state s, word round_key[4]);
void InvSubBytes(state s);
void InvShiftRows(state s);
void InvMixColumns(state s);
void aes_encrypt(state dst, state src, word key[], int key_length);
void aes_decrypt(state dst, state src, word key[], int key_length);

#endif
