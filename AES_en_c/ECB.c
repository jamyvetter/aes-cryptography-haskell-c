#include <stdio.h>
#include "AES.h"

int aes_ecb_encrypt(unsigned char** dst, int *dst_data_length, unsigned char* src,int src_data_length, word key[], int key_length);
int aes_ecb_decrypt(unsigned char** dst, int *dst_data_length, unsigned char* src,int src_data_length, word key[], int key_length);