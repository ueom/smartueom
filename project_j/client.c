#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

void * recv_msg(void * arg);
void error_handling(char * msg);


int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t rec_thread;
	void * thread_return;
	if(argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	while(1)
	{
		pthread_create(&rec_thread, NULL, recv_msg, (void *)&sock);
		pthread_detach(rec_thread);
	}	
	close(sock);
	return 0;
}

void * recv_msg(void * arg)
{
	int sock = *((int *)arg);
	char msg[BUF_SIZE];
	int str_len;

	while(1)
	{
		str_len = read(sock, msg, BUF_SIZE-1);
		msg[str_len] = 0;	
		fputs(msg, stdout);		
	}
	return NULL;
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
	
	return NULL;
}
