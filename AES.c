/*
Byte = Octet =  unsigned char (unsigned pour avoir les opérations de décalage shift )
Word = 32 bits = un tableau de 4 octets. 
State = 4*word
*/
//Définition des types byte,word,state
typedef unsigned char byte;
typedef byte word[4];
typedef byte state[4][4]; // ou un tableau de 4 word : typedef word state[4]


void SubWord(word w);
void RotWord(word w);
void KeyExpansion(byte key[], word w[], int key_length);

void SubBytes(state s);
void ShiftRows(state s);
void MixColumns(state s);
void AddRoundKey(state s, word round_key[4]);

void InvSubBytes(state s);
void InvShiftRows(state s);
void InvMixColumns(state s);

void aes_encrypt(state dst, state src, word key[], int key_length);
void aes_decrypt(state dst, state src, word key[], int key_length);
// key_length permet toujours de déduire Nk et Nr. Si key_length est incorrect, il faut appeler exit()