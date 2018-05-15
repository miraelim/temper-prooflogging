#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    FILE *f = fopen("time.txt", "r");
    if((flock(fileno(f), LOCK_EX))==0)
	printf("filelock success\n");
    else 
	printf("filelock fail\n");

    return 0;
}
