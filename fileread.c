#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main( )
{

    int fd;

    char buf [100];

    fd = open( "key.txt", O_RDONLY );
    if( fd == -1 )
    {
	printf("file open error\n");
    }
    else
    {
	read(fd, buf, sizeof(buf));
	printf("%s\n", buf);
	close(fd);
    }

    return 0;
}
