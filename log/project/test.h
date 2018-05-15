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
#include <time.h>

#define BUFF_SIZE 1024

unsigned char digest[SHA256_DIGEST_LENGTH];
char tpm_counter[256];
char rootkey[SHA256_DIGEST_LENGTH*2+1];
FILE  *fd2, *fd3, *t_log, *key, *logs, *logs_hmac, *get_counter, *counter, *counter1;
char    temp[BUFF_SIZE];
char    hmac_log[BUFF_SIZE];
struct log{
    char* log;
    char* hmac_log;
};
struct log list;

struct key_NODE{
    struct key_NODE * next;
    char key[SHA256_DIGEST_LENGTH*2+1];

};
struct key_NODE *head;
