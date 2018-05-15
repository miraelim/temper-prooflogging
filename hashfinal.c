#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/file.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 1024
int main() {
    // char    temp[BUFF_SIZE];
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned char digest1[SHA256_DIGEST_LENGTH];
    char* string ;
    int fd;

    fd = open("log.txt",O_RDWR);
    if(fd<0)
	printf("open auth.log fail\n");

    else {
	printf("open log.txt success\n");
	read(fd, string, BUFF_SIZE);
	printf("%s\n",string);
    }

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, string, strlen(string));
    SHA256_Final(digest, &ctx);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("SHA256 digest: %s\n", mdString);
    printf("SHA256_DIGEST_LENGTH : %d\n", SHA256_DIGEST_LENGTH);

    printf("second key\n");
    SHA256_CTX ctx1;
    SHA256_Init(&ctx1);
    SHA256_Update(&ctx1, digest, strlen(digest));
    SHA256_Final(digest1, &ctx1);
    char mdstring1[SHA256_DIGEST_LENGTH*2+1];
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++)
	sprintf(&mdstring1[i*2], "%02x",(unsigned int)digest1[i]);

    printf("SHA256 digest: %s\n",mdstring1);


    return 0;
}
