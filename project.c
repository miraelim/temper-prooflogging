#include <stdio.h>
#include <stddef.h>

const int source = 21;
void printBinary(int input);

/*size_t stringLength(const char* source)
{
    if(source == NULL) { return 0; }
    size_t length = 0;
    while(*source != '\0') {
	length++;
source++;
    }
    return length;
}
*/
static int getHash(const int source){
//    size_t length = stringLength(source);
    int hash = NULL;
    hash = (source | (source >>1)) + source/10;        
    
    return hash;
}
//static const char *const testCases[] = {
  //  "this", "is", "a", "test", "but", "i", "should", "use", "real", "dictonary"};

#define TABLE_SIZE (16)

int main()
{
    int name = getHash(source);
    printf("%d hash: %d\n", source, name);
    printBinary(name);
    for(size_t i = 0; i < 5; i++) {
//	const char* source = "key";
	name = getHash(name);
	printf("%d, <==> ", name);
	printBinary(name);
    }
    return 0;
}

void printBinary(int input){
    int i = 31;
    for(i; i>=0;--i){
	printf("%d",(input>>i)&1);
    }
    printf("\n");
}
