#include <stdio.h>
typedef unsigned char byte;
typedef byte state[4][4];
typedef byte word[4];

#define mul2(x) (byte)(((x) << 1) ^ (((x) & 0x80) ? 0x1b : 0x00))
#define mul3(x) (byte)(mul2(x) ^ (x))

void MixColumns(state s) {
    for (int c = 0; c < 4; c++) {
        byte s0=s[0][c]; byte s1=s[1][c]; byte s2=s[2][c]; byte s3=s[3][c];
        s[0][c] = mul2(s0) ^ mul3(s1) ^ s2 ^ s3;
        s[1][c] = s0 ^ mul2(s1) ^ mul3(s2) ^ s3;
        s[2][c] = s0 ^ s1 ^ mul2(s2) ^ mul3(s3);
        s[3][c] = mul3(s0) ^ s1 ^ s2 ^ mul2(s3);
    }
}
void AddRoundKey(state s, word round_key[4]){
    for (int r=0; r<4; r++)
        for (int c=0; c<4; c++)
            s[r][c] ^= round_key[c][r];
}
int main() {
    state in = {
        {0xd4, 0xe0, 0xb8, 0x1e},
        {0xbf, 0xb4, 0xe8, 0x27},
        {0x5d, 0x30, 0x11, 0x98},
        {0x52, 0xae, 0xf1, 0xe5}
    };
    MixColumns(in);
    printf("MixColumns:\n");
    for(int r=0;r<4;r++) printf("%02x %02x %02x %02x\n", in[r][0], in[r][1], in[r][2], in[r][3]);
    
    state in2 = {
        {0x32, 0x88, 0x31, 0xe0},
        {0x43, 0x5a, 0x31, 0x37},
        {0xf6, 0x30, 0x98, 0x07},
        {0xa8, 0x8d, 0xa2, 0x34}
    };
    word rk[4] = {
        {0x2b, 0x7e, 0x15, 0x16},
        {0x28, 0xae, 0xd2, 0xa6},
        {0xab, 0xf7, 0x15, 0x88},
        {0x09, 0xcf, 0x4f, 0x3c}
    };
    AddRoundKey(in2, rk);
    printf("AddRoundKey:\n");
    for(int r=0;r<4;r++) printf("%02x %02x %02x %02x\n", in2[r][0], in2[r][1], in2[r][2], in2[r][3]);
    return 0;
}
