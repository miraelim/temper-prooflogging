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

void main(){
    system("./test_tpm2_rsaencrypt.sh");
    printf("encrypt success\n");
    system("./test_tpm2_rsadecrypt.sh ");
    printf("decrypt success\n");
}
