#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
    bool status;
    system("tpm2_sign -k 0x81010005 -g 0x000B -m signinput.txt -s signoutput.txt");
    printf("sign success\n");
//    system("tpm2_verifysignature -k 0x81010005 -g 0x000B -m key.txt -s output1.txt -t ticket.txt");
    return 0;
}
