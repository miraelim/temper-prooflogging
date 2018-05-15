#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 1024
void main(){
	char    temp[BUFF_SIZE];
	char    *temp2 = "add this line";
	int	    fd1, fd2;
	fd1 = open("log.txt", O_RDWR);
	fd2 = open("log.txt", O_RDWR);

	if(fd1<0)
		printf("open fd1 fail\n");

	else{
		printf("open fd1 success\n");
		read(fd1, temp, BUFF_SIZE);
		puts(temp);
	}

	if(fd2<0)
		printf("open fd2 fail\n");
	else{
		printf("open fd2 success\n");
		read(fd1, temp, BUFF_SIZE);
		puts(temp);
	}

	if (flock(fd1, LOCK_SH) == -1) {
		printf("file lock failed\n");
		exit(1);
	}
	else 
		printf("lock success\n");
	write(fd2, temp2, strlen(temp2));
	read(fd2, temp, BUFF_SIZE);
	puts(temp);



	if (flock(fd1, LOCK_UN) == -1) {
		printf("lock released failed\n");
		exit(1);
	}
	close(fd1);
	close(fd2);
}
