#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char * message);

int main(int argc, char * argv[])
{
	int send_sock;
	struct sockaddr_in mul_adr, clnt_adr; 
	int time_live = TTL, str_len;
	char buf[BUF_SIZE];
	socklen_t clnt_sz;
	
	if(argc != 3)
	{
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);	
	}

	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&mul_adr, 0, sizeof(mul_adr));
	mul_adr.sin_family = AF_INET;
	mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
	mul_adr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, 
		   (void*)&time_live, sizeof(mul_adr));

	while(1)
	{
		clnt_sz = sizeof(clnt_adr);
		str_len = recvfrom(send_sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_sz); 
		if(str_len<0)
			break;
		sendto(send_sock, buf, strlen(buf), 0,
			 (struct sockaddr*)&mul_adr, sizeof(mul_adr));
	}
	close(send_sock);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
