#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024 
#define NAME_SIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc != 4)
	{
		printf("Usage : %s <IP> <PORT> <name>\n", argv[0]);
		exit(1);
	}
	
	sprintf(name, "[%s]", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");

	printf("What date of the production do you want to check?: ");
	while(1)
	{
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_detach(snd_thread);
	pthread_detach(rcv_thread);
	}
	close(sock);
	return 0;
}

void * send_msg(void * arg)
{
	int sock = *((int*)arg);
	char send_num[10];
	int num = 0;
	
//	scanf("%d", &num);
	while(1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg, "q\n")||!strcmp(msg,"Q\n"))
		{
			shutdown(sock, SHUT_WR);
			exit(0);
		}
		sprintf(send_num, "%s", msg);
		write(sock, send_num, strlen(msg));
	}
	return NULL;
}

void * recv_msg(void * arg)
{
	int sock = *((int*)arg);
	char recv_msg[BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len = read(sock, recv_msg, BUF_SIZE-1);
		if(str_len == -1)
			return (void*)-1;	
		recv_msg[str_len] = 0;
		fputs(recv_msg, stdout);
	}
	return NULL;
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
