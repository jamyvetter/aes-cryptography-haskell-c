#ifndef CBC_H
#define CBC_H

#include "AES.h"

int aes_cbc_encrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, unsigned char* iv, word key[], int key_length);
int aes_cbc_decrypt(unsigned char** dst, int *dst_data_length, unsigned char* src, int src_data_length, unsigned char* iv, word key[], int key_length);

#endif
