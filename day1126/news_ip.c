#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);

int main(int argc, char * argv[])
{
	int recv_sock;
	struct sockaddr_in adr, send_adr;
	int str_len;
	socklen_t adr_sz;
	char buf[BUF_SIZE];
	
	if(argc !=2)
	{
		printf("Usage : %s <PORT>\n", argv[0]);
		exit(1);
	}

	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&adr, 0, sizeof(adr));
	adr.sin_family=AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(atoi(argv[1]));

	if(bind(recv_sock, (struct sockaddr*)&adr, sizeof(adr))==-1)
		error_handling("bind() error");

	memset(&send_adr, 0, sizeof(send_adr));

	while(1)
	{
		adr_sz = sizeof(send_adr);
		str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0,
			(struct sockaddr*)&send_adr, &adr_sz);
		if(str_len<0)
			break;
		buf[str_len]=0;
		printf("Sender IP : %s\t",inet_ntoa(send_adr.sin_addr));
		fputs(buf, stdout);
	}
	close(recv_sock);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
