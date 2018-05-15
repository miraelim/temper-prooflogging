#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/file.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/x509.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
//#include "counter.h"
//#include "counter1.h"
//#include "tpm20.h"
int main(){
    unsigned int random;
    char* tmp;
    //    TPMS_COMMAND_AUDIT_INFO tpm_cmd;
    system("tpm2_getrandom 1 >>random.txt");
    FILE *f;
    f=fopen("random.txt","rw");
    if(f==0) return 0;
    fscanf(f,"%0x", &random);
    fclose(f);
    printf("%0x\n", random);
//    random = strtoul(tmp, NULL, 16);
  //  printf("random :%s	%0x\n",tmp ,random);
    
    return 0;


//    printf("%0x\n",TPMA_PERMANENT_OWNERAUTHSET);
 //   printf("TPM_RC_EXCLUSIVE: %0X\n",TPM_RC_EXCLUSIVE);
}
