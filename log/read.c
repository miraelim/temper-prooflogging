#include <stdio.h>

#include <stdlib.h>



int main(int argc, char* argv[])

{

    FILE *fp;
    char data[256];
    int total=0;

    fp = fopen("log.txt", "r");
    printf("open success\n");
    if (fp == NULL) {
	fprintf(stderr, "Can't open file\n");
	exit(1);

    }

    int ret;

//    while(!feof(fp))
  //  {
	fgets(data, sizeof(data),fp);
	total++;
	printf("#%d log: %s\n",total, data);

//    }

    fclose(fp);

    return 0;

}
