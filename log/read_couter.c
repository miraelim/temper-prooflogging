#include <stdio.h>
#include <stdlib.h>
int main(){
    int cnt ;
    char temp[256],count[256];
    FILE *counter;
    system("tpm2_dump_capability -c properties-fixed | grep COUNTER >> counter.txt");

    counter = fopen("counter.txt", "r");
    printf("open success\n");
    if (counter == NULL) {
	fprintf(stderr, "Can't open counter.txt\n");
	exit(1);
    }
    fscanf(counter,"%s %0x\n",temp, &cnt);
    sprintf(count, "%d",cnt);
    printf("공백으로 분리 : %s\n", count);
    return 0;
}
