#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

ssize_t write(int fd, const void * buf, size_t nbytes);
void error_handling(char * str);

int main(void)
{
	int fd;
	char buf[] = "Let's go!\n";
	fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
	if(fd==-1)
		error_handling("open() error!");
	printf("file descriptor: %d \n", fd);

	if(write(fd, buf, sizeof(buf))==-1)
		error_handling("write() error!");
	close(fd);
	return 0;
}

void error_handling(char * str)
{
	fputs(str, stderr);
	fputc('\n', stderr);
	exit(1);
}
