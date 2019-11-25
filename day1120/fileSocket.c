#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
	int fd1, fd2, fd3;
	fd1 = socket(PF_INET, SOCK_STREAM, 0);
	fd2 = open("test2.dat", "O_CREAT");
	fd3 = socket(PF_INET, SOCK_DGRAM, 0);

	printf("file descroptor 1: %d\n", fd1);
	printf("file descroptor 2: %d\n", fd2);
	printf("file descroptor 3: %d\n", fd3);

	close(fd1); close(fd2); close(fd3);
	return 0;
}
