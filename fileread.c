#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main( )
{

    int fd,num;

    char buf [4048];

    fd = open( "key.txt", O_RDONLY );
    if( fd == -1 )
    {
	printf("file open error\n");
    }
    else
    {
	num =read(fd, buf, sizeof(buf));
	printf("key: %s\n", buf);
	printf("num: %d\n",num);
	close(fd);
    }

    return 0;
}
