#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef byte word[4];
typedef byte state[4][4];

void copy_state(state src, state dst) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

// Include the user's AES.c but we'll mock the main function so it doesn't run.
#define main old_main
#include "AES.c"
#undef main

void print_matrix_init(state s) {
    printf("    state expected = {\n");
    for (int i = 0; i < 4; i++) {
        printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", 
            s[i][0], s[i][1], s[i][2], s[i][3], (i == 3) ? "" : ",");
    }
    printf("    };\n");
}

int main() {
    printf("--- COPY FROM HERE ---\n");
    
    state s1 = {
        {0x19, 0xa0, 0x9a, 0xe9},
        {0x3d, 0xf4, 0xc6, 0xf8},
        {0xe3, 0xe2, 0x8d, 0x48},
        {0xbe, 0x2b, 0x2a, 0x08}
    };
    state s2; copy_state(s1, s2);
    SubBytes(s2);
    printf("void test_subBytes(){\n");
    printf("    state in = {\n        {0x19, 0xa0, 0x9a, 0xe9},\n        {0x3d, 0xf4, 0xc6, 0xf8},\n        {0xe3, 0xe2, 0x8d, 0x48},\n        {0xbe, 0x2b, 0x2a, 0x08}\n    };\n");
    print_matrix_init(s2);
    printf("    SubBytes(in);\n    if (compare_state(in, expected)) printf(\"[OK] SubBytes\\n\"); else printf(\"[ECHEC] SubBytes\\n\");\n}\n\n");

    state s3; copy_state(s2, s3);
    ShiftRows(s3);
    printf("void test_shiftRows(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", s2[i][0], s2[i][1], s2[i][2], s2[i][3], (i==3)?"":",");
    printf("    };\n");
    print_matrix_init(s3);
    printf("    ShiftRows(in);\n    if (compare_state(in, expected)) printf(\"[OK] ShiftRows\\n\"); else printf(\"[ECHEC] ShiftRows\\n\");\n}\n\n");

    printf("void test_invSubBytes(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", s2[i][0], s2[i][1], s2[i][2], s2[i][3], (i==3)?"":",");
    printf("    };\n");
    print_matrix_init(s1);
    printf("    InvSubBytes(in);\n    if (compare_state(in, expected)) printf(\"[OK] InvSubBytes\\n\"); else printf(\"[ECHEC] InvSubBytes\\n\");\n}\n\n");

    printf("void test_invShiftRows(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", s3[i][0], s3[i][1], s3[i][2], s3[i][3], (i==3)?"":",");
    printf("    };\n");
    print_matrix_init(s2);
    printf("    InvShiftRows(in);\n    if (compare_state(in, expected)) printf(\"[OK] InvShiftRows\\n\"); else printf(\"[ECHEC] InvShiftRows\\n\");\n}\n\n");

    state s4; copy_state(s3, s4);
    MixColumns(s4);
    printf("void test_mixColumns(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", s3[i][0], s3[i][1], s3[i][2], s3[i][3], (i==3)?"":",");
    printf("    };\n");
    print_matrix_init(s4);
    printf("    MixColumns(in);\n    if (compare_state(in, expected)) printf(\"[OK] MixColumns\\n\"); else printf(\"[ECHEC] MixColumns\\n\");\n}\n\n");

    printf("void test_invMixColumns(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", s4[i][0], s4[i][1], s4[i][2], s4[i][3], (i==3)?"":",");
    printf("    };\n");
    print_matrix_init(s3);
    printf("    InvMixColumns(in);\n    if (compare_state(in, expected)) printf(\"[OK] InvMixColumns\\n\"); else printf(\"[ECHEC] InvMixColumns\\n\");\n}\n\n");

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
    state out2; copy_state(in2, out2);
    AddRoundKey(out2, rk);
    printf("void test_addRoundKey(){\n");
    printf("    state in = {\n");
    for (int i=0;i<4;i++) printf("        {0x%02x, 0x%02x, 0x%02x, 0x%02x}%s\n", in2[i][0], in2[i][1], in2[i][2], in2[i][3], (i==3)?"":",");
    printf("    };\n");
    printf("    word round_key[4] = {\n        {0x2b, 0x7e, 0x15, 0x16},\n        {0x28, 0xae, 0xd2, 0xa6},\n        {0xab, 0xf7, 0x15, 0x88},\n        {0x09, 0xcf, 0x4f, 0x3c}\n    };\n");
    print_matrix_init(out2);
    printf("    AddRoundKey(in, round_key);\n    if (compare_state(in, expected)) printf(\"[OK] AddRoundKey\\n\"); else printf(\"[ECHEC] AddRoundKey\\n\");\n}\n\n");

    return 0;
}
