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

#define BUFF_SIZE 1024

unsigned char digest[SHA256_DIGEST_LENGTH];
const char* id = "MIRAE LIM";
char rootkey[SHA256_DIGEST_LENGTH*2+1];
int fd1, fd2;
char    temp[BUFF_SIZE];
char	hmac_log[BUFF_SIZE];
struct log{
    char* log;
    char* hmac_log;
};

struct log list;

struct key_NODE{
    struct key_NODE * next;
    char key[SHA256_DIGEST_LENGTH*2+1];

};

void makeRootkey(){
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256((unsigned char*)&id, strlen(id), (unsigned char*)&digest);   
    for(int i =0 ; i < SHA256_DIGEST_LENGTH; i++){
	sprintf(&rootkey[i*2], "%02x", (unsigned int)digest[i]);
    }

    printf("First RootKey: %s\n",rootkey);
}

void get_filelock(){
    fd1 = open("key.txt", O_RDWR);
    fd2 = open("log.txt", O_RDWR);
    if(fd1<0)
	printf("open fd1 fail\n");

    else{
	printf("open fd1 success\n");
    }

    if(fd2<0)
	printf("open fd2 fail\n");

    else{
	printf("open fd2 success\n");
	read(fd2, temp, BUFF_SIZE);
	printf("%s",temp);
    }

    if (flock(fd1, LOCK_SH) == -1) {
	printf("file lock failed\n");
	exit(1);
    }
    else
	printf("log.txt lock success\n");

    if (flock(fd2, LOCK_SH) == -1) {
	printf("file lock failed\n");
	exit(1);
    }
    else
	printf("key.txt lock success\n");
}

void hmac_sha256(
	const unsigned char *text,      /* pointer to data stream        */
	int                 text_len,   /* length of data stream         */
	const unsigned char *key,       /* pointer to authentication key */
	int                 key_len,    /* length of authentication key  */
	void                *digest)    /* caller digest to be filled in */
{
    unsigned char k_ipad[65];   /* inner padding -
				 * key XORd with ipad
				 */
    unsigned char k_opad[65];   /* outer padding -
				 * key XORd with opad
				 */
    unsigned char tk[SHA256_DIGEST_LENGTH];
    unsigned char tk2[SHA256_DIGEST_LENGTH];
    unsigned char bufferIn[1024];
    unsigned char bufferOut[1024];
    int           i;

    /* if key is longer than 64 bytes reset it to key=sha256(key) */
    if ( key_len > 64 ) {
	SHA256( key, key_len, tk );
	key     = tk;
	key_len = SHA256_DIGEST_LENGTH;
    }

    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    memset( k_ipad, 0, sizeof k_ipad );
    memset( k_opad, 0, sizeof k_opad );
    memcpy( k_ipad, key, key_len );
    memcpy( k_opad, key, key_len );

    /* XOR key with ipad and opad values */
    for ( i = 0; i < 64; i++ ) {
	k_ipad[i] ^= 0x36;
	k_opad[i] ^= 0x5c;
    }

    /*
     * perform inner SHA256
     */
    memset( bufferIn, 0x00, 1024 );
    memcpy( bufferIn, k_ipad, 64 );
    memcpy( bufferIn + 64, text, text_len );

    SHA256( bufferIn, 64 + text_len, tk2 );

    /*
     * perform outer SHA256
     */
    memset( bufferOut, 0x00, 1024 );
    memcpy( bufferOut, k_opad, 64 );
    memcpy( bufferOut + 64, tk2, SHA256_DIGEST_LENGTH );

    SHA256( bufferOut, 64 + SHA256_DIGEST_LENGTH, digest );
}



void log_hmac(){
//    printf("temp in hmac:%s\n",temp);
    hmac_sha256("MIRAE", strlen("MIRAE"), "19940427", strlen("19940427"), hmac_log );
    printf("log hmac : %s\n",hmac_log);
}



int main(){
    char* string ;
    int fd;

    fd = open("/var/log/auth.log",O_RDWR);

    if(fd<0)
	printf("open auth.log fail\n");
    else 
	printf("open auth.log success\n");

    struct key_NODE *head = malloc(sizeof(struct key_NODE));

    makeRootkey();
    strcpy(head->key, rootkey);
    printf("%s\n",head->key);
    get_filelock();
    log_hmac();
}

