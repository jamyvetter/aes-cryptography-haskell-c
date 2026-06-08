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

static const byte sbox[256];

void SubWord(word w){
    for(int i=0;i<3;i++){
        w[i]=sbox[w[i]];
    }
}

void RotWord(word w){ //Prend un word [a,b,c,d]-> fait [b,c,d,a]
    byte temp;
    temp=w[0];
    w[0]=w[1];w[1]=w[2];w[2]=w[3];w[3]=w[0];
}

void KeyExpansion(byte key[], word w[], int key_length){ // Nr en paramètre de ça ?
    int Nk=key_length,Nb=4; 
    int Nr=0;// achanger
    word temp;
    int i=0;
    while (i< Nk){
        w[i]= {key[4*i],key[4*i+1],key[4*i+2],key[4*i+3]};
        i++;
    }
    i=Nk;
    while (i<Nb*(Nr+1)){
        temp= w[i-1];
        if(i%Nk==0) {
            RotWord(temp);SubWord(temp);temp^= Rcon[i/Nk];
        }
        else if(Nk>6 && i%Nk==4) SubWord(temp);
        w[i] = w[i-Nk] ^ temp;
        i++;
    }
}






void ShiftRows(state s){
    for (int i=1;i<4;i++){
        for(int j=0;j<i+1;j++) RotWord(s[i]);
    }
}



void SubBytes(state s);
//J -> fait
void MixColumns(state s);
//J

void AddRoundKey(state s, word round_key[4]){ // on passe un tableau de word à AddRoundKey, donc le paramètre round est pris en compte
    for (int i=0;i<4;i++){
        for(int c=0;c<4;c++){
            s[i][c] ^= round_key[c]; //ici
        }
    }
}


void InvSubBytes(state s);
void InvShiftRows(state s);
void InvMixColumns(state s);

void aes_encrypt(state dst, state src, word key[], int key_length);
void aes_decrypt(state dst, state src, word key[], int key_length);
// key_length permet toujours de déduire Nk et Nr. Si key_length est incorrect, il faut appeler exit()



/*
Faire en sorte de créer des cas de tests pour le code C à partir du code Haskell

*/

int main(){

    return 0;
}