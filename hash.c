#include <stdio.h>
#include <stddef.h>

const char* source = "MiRae";
void printBinary(int input);

size_t stringLength(const char* source)
{
    if(source == NULL) { return 0; }
    size_t length = 0;
    while(*source != '\0') {
	length++;
source++;
    }
    return length;
}

static size_t getHash(const char* source){
    size_t length = stringLength(source);
    size_t hash = 0;
    for(size_t i = 0; i < length; i++) {
	char c = source[i];
	int a = c - '0';
	hash = (hash * 10) + a;
    }
    return hash;
}
static const char *const testCases[] = {
    "this", "is", "a", "test", "but", "i", "should", "use", "real", "dictonary"};

#define TABLE_SIZE (16)

int main()
{
    size_t name = getHash(source);
    printf("%zu\n", name);
    printBinary(name);
    for(size_t i = 0; i < 5; i++) {
//	const char* source = "key";
	name = getHash(name);
	printf("%s <==> ", source);
	printBinary(name);
    }
    return 0;
}

void printBinary(int input){
    int i, check=1;
    char bit;
    for(i=31; i>=3;i--){
	bit=(input&(1<<i))?1:0;
	if(check){
	    if(bit==1){
		check=0;
		printf("%d",bit);
	    }
	}
	else printf("%d", bit);
    }
    printf("\n");
}
