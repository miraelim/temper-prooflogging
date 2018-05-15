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
FILE  *fd1, *fd2, *fd3, *fd4, *t_log;
char    temp[BUFF_SIZE];
char	hmac_log[BUFF_SIZE];
double getcounter_t, makerootkey_t, filelock_t, hmac256_t, loghmac_t,verifykey_t, main_t, signkey_t;

struct log{
    char* log;
    char* hmac_log;
};

struct log list;

struct key_NODE{
    struct key_NODE * next;
    char key[SHA256_DIGEST_LENGTH*2+1];

};

int get_counter(){
    clock_t t;
    t = clock();
    int cnt ;
    char temp[256];
    FILE *counter;
    system("tpm2_dump_capability -c properties-fixed | grep COUNTER >> counter.txt");
    counter = fopen("counter.txt", "r");
    printf("open success\n");
    if (counter == NULL) {
	fprintf(stderr, "Can't open counter.txt\n");
	exit(1);
    }
    fscanf(counter,"%s %0x\n",temp, &cnt);
    getcounter_t = (double)(clock() - t);
    fprintf(t_log, "getcounter: %.6fms\n",(getcounter_t/CLOCKS_PER_SEC)*1000);
    return cnt;

}

void makeRootkey(struct key_NODE* head){
    clock_t t;
    t = clock();
    unsigned char digest[SHA256_DIGEST_LENGTH];
    int temp;
    temp = get_counter();
    sprintf(tpm_counter, "%d", temp);
    printf("counter:%s \n",tpm_counter);
    SHA256((unsigned char*)&tpm_counter, strlen(tpm_counter), (unsigned char*)&digest);   
    for(int i =0 ; i < SHA256_DIGEST_LENGTH; i++){
	sprintf(&rootkey[i*2], "%02x", (unsigned int)digest[i]);
    }
    strcpy(head->key, rootkey);

    printf("First RootKey: %s\n",head->key);


    fd4= fopen("key.txt","w+");

    fputs(head->key,fd4);
    fclose(fd4);
    printf("Rootkey write success\n");
    makerootkey_t= (double)(clock() - t);
    fprintf(t_log, "makerootkey: %.6fms\n",(makerootkey_t/CLOCKS_PER_SEC)*1000);

}


void get_filelock(){
    clock_t t;
    t = clock();

    fd1 =  fopen("log.txt", "r+");
    printf("log.txt success\n");
    if (fd1 == NULL) {
	fprintf(stderr, "Can't open log.txt\n");
	exit(1);
    }


    /*    fd3 = open("log_hmac.txt", O_RDWR);
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
	  }*/
    if (flock(fileno(fd1), LOCK_SH) == -1) {
	printf("file lock failed\n");
	exit(1);
    }
    else
	printf("log.txt lock success\n");
    /*
       if (flock(fd2, LOCK_SH) == -1) {
       printf("file lock failed\n");
       exit(1);
       }
       else
       printf("key.txt lock success\n");*/
    filelock_t= (double)(clock() - t);
    fprintf(t_log, "filelock: %.6fms\n",(filelock_t/CLOCKS_PER_SEC)*1000);

}

void hmac_sha256(
	const unsigned char *text,      /* pointer to data stream        */
	int                 text_len,   /* length of data stream         */
	const unsigned char *key,       /* pointer to authentication key */
	int                 key_len,    /* length of authentication key  */
	void                *digest)    /* caller digest to be filled in */
{

    clock_t t;
    t = clock();
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
    hmac256_t= (double)(clock() - t);
    fprintf(t_log, "hmac256 : %.6fms\n",(hmac256_t/CLOCKS_PER_SEC)*1000);
}



void log_hmac(char *log,struct key_NODE* head ){
    clock_t t;
    t = clock();
    //    printf("temp in hmac:%s\n",temp);
    fd3= fopen("log_hmac.txt","a+");

    hmac_sha256(log, strlen(log), head->key, strlen(head->key), hmac_log );
    printf("log: %s\t log hmac : %s\n",log, hmac_log);
    fputs(hmac_log,fd3);
    fclose(fd3);
    printf("file log_hmac success\n");
    loghmac_t= (double)(clock() - t);
    fprintf(t_log, "loghmac: %.6fms\n",(loghmac_t/CLOCKS_PER_SEC)*1000);
}



int main(){

    clock_t t1,t2,t3;
    char string[256]; 
    t_log = fopen("time.txt","w+");

    t1 = clock();

    t2 = clock();
    system("./test_tpm2_sign.sh");

    verifykey_t= (double)(clock() - t1);
    fprintf(t_log, "verify %.6fms\n",(verify_t /CLOCKS_PER_SEC)*1000);


    //   int fd;

    /*    fd = open("/var/log/auth.log",O_RDWR);

	  if(fd<0)
	  printf("open auth.log fail\n");
	  else 
	  printf("open auth.log success\n");
     */
    struct key_NODE *head = malloc(sizeof(struct key_NODE));

    makeRootkey(head);
    get_filelock();
    while(!feof(fd1)){
	fgets(string, sizeof(string), fd1);
	log_hmac(string,head);
    }
    main_t= (double)(clock() - t1);
    fprintf(t_log, "main %.6fms\n",(main_t /CLOCKS_PER_SEC)*1000);

}
