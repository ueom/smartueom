#include <mysql/mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
#define MAX_CLNT 256

void error_handling(char * msg);
char * readDB(MYSQL * mysql, char * buf, int size, int id);
void *handle_clnt(void * arg);
void send_msg(char * buf, int len);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;
char buf[256] = ""; 
int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;

	MYSQL mysql;

	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "1234", "plant", 0, NULL, 0);

	if(argc!=2)	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	while(1)
	{
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

	pthread_mutex_lock(&mutx);
	clnt_socks[clnt_cnt++] = clnt_sock;
	pthread_mutex_unlock(&mutx);

	printf("Connected client IP : %s\n", inet_ntoa(clnt_adr.sin_addr));


	int rowid = 1;
	char res = readDB(&mysql, buf, 256, rowid);

	mysql_close(&mysql);


	}
	close(serv_sock);
	return 0;
}

char * readDB(MYSQL * mysql, char * buf, int size, int id)
{
	char strQuery[256] = "";
	buf[0] = 0;
	int len = 0;	

	sprintf(strQuery, "select machineID from machine where machineRisk = 0");
	int res = mysql_query(mysql, strQuery);
	
	if(res!=0)
		return -1;
	else
	{
		MYSQL_RES * res_ptr = mysql_store_result(mysql);
		MYSQL_ROW sqlrow;
	
		while(sqlrow = mysql_fetch_row(res_ptr))
		{
			char buf_field[256] = "";
			sprintf(buf_field, "%s");
			strcat(buf, buf_field);
			printf("machine ID : %s\n", sqlrow[0]);
	
			int i;
			pthread_mutex_lock(&mutx);
			for(i=0; i<clnt_cnt; i++)
				write(clnt_socks[i], sqlrow[0], strlen(sqlrow[0]));
			pthread_mutex_unlock(&mutx);

		}
		
		if(mysql_errno(mysql))
		{
			fprintf(stderr, "(!) error: %s\n", mysql_error(mysql));
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	return buf;
}



void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_msg(char * msg, int len)
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
