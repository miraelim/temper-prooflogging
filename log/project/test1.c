#include "test.h"
int getcounter(){
    int cnt ;
    char temp[256];
    system("tpm2_dump_capability -c properties-fixed | grep COUNTER >> get_counter.txt");
    get_counter = fopen("get_counter.txt", "r");
    printf("get counter.txt open success\n");
    if (get_counter == NULL) {
	fprintf(stderr, "Can't open get_counter.txt\n");
	exit(1);
    }

    counter = fopen("counter.txt", "w+");
    printf("counter.txt open success\n");
    if (counter == NULL) {
	fprintf(stderr, "Can't open counter.txt\n");
	exit(1);
    }

    fscanf(get_counter,"%s %0x\n",temp, &cnt);
    printf("%d\n",cnt);
//    system("tpm2_nvwrite -x 0x1500018 -a 0x40000001 -f counter.txt");
    fprintf(counter, "%d",cnt);
    fclose(counter);
    fclose(get_counter);
    return cnt;

}

void makeRootkey(struct key_NODE* head){
    unsigned char digest[SHA256_DIGEST_LENGTH];
    int temp;
    temp = getcounter();
    sprintf(tpm_counter, "%d", temp);
    printf("counter:%s \n",tpm_counter);
    SHA256((unsigned char*)&tpm_counter, strlen(tpm_counter), (unsigned char*)&digest);   
    for(int i =0 ; i < SHA256_DIGEST_LENGTH; i++){
	sprintf(&rootkey[i*2], "%02x", (unsigned int)digest[i]);
    }

    printf("First RootKey: %s\n",rootkey);


    key= fopen("key.txt","w+");

    if(key==NULL){
	fprintf(stderr, "Can't open key.txt\n");
	exit(1);
    }
    fputs(rootkey,key);
    fclose(key);

    printf("Rootkey write success\n");
    //system("tpm2_sign -c signkey.txt -g 0x000B -m key.txt -s keyoutput.txt");


}


int main(){

    char string[256]; 


    makeRootkey(head);
    /*    get_filelock();
	  while(!feof(fd1)){
	  fgets(string, sizeof(string), fd1);
	  log_hmac(string,head);
	  }*/
    return 0;
}
