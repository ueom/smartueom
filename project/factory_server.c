#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <mysql/mysql.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

#define STR_HOST "localhost"
#define STR_ID "root"
#define STR_PW "1234"
#define STR_DB "factory"

int readDB(MYSQL * mysql, char * buf, int size, int num);
int init(MYSQL * mysql);
void destroy(MYSQL * mysql);
void executeQuery(MYSQL * mysql, char * query);
void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char *  msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	MYSQL mysql;
	init(&mysql);
	char buf[256];
	int num = 0;
	printf("When do you want to search?: ");
	scanf("%d", &num);
	readDB(&mysql, buf, 256, num);

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	destroy(&mysql);
	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	char msg[BUF_SIZE];
	
	while((str_len = read(clnt_sock, msg, sizeof(msg)))!=0)
		send_msg(msg, str_len);

	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

int init(MYSQL * mysql)
{
	mysql_init(mysql);
	if(mysql_real_connect(mysql, STR_HOST, STR_ID, STR_PW, STR_DB, 0, NULL, 0))
	{
		printf("mysql_real_connect success\n");
		return 0; 
	}
	else
	{
		printf("mysql_real_connect failed\n");
		return -1;
	}
}

int readDB(MYSQL * mysql, char * buf, int size, int num)
{
	char stringQuery[255] = "";
	buf[0] = 0;

	sprintf(stringQuery, "SELECT p_num, p_output, p_date, p_time FROM factory WHERE p_num = %d;", num);

	int result = mysql_query(mysql, stringQuery);
	if(result !=0)
	{
		return -1;
	}
	else
	{
		MYSQL_RES * res_ptr = mysql_use_result(mysql);
		MYSQL_ROW sqlRow = mysql_fetch_row(res_ptr);
		
		unsigned int field_count = 0;
		while(field_count<mysql_field_count(mysql))
		{
			char buf_field[256]="";
			if(sqlRow[field_count])
				sprintf(buf_field, "|%s", sqlRow[field_count]);
			else sprintf(buf_field, "|0");
		
			printf("%s", buf_field);	
			field_count++;
		}
		printf("\n");
		
		if(mysql_errno(mysql))
		{
			fprintf(stderr, "Error: %s\n", mysql_error(mysql));
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	return 0;
}

void destroy(MYSQL * mysql)
{
	mysql_close(mysql);
	return;
}

void executeQuery(MYSQL * mysql, char * query)
{
	int result = mysql_query(mysql, query);
	if(result == 0) printf("Execute Query Success\n");
	else printf("Execute Query Failed:%s\n", query);
	return;
}


void send_msg(char * msg, int len)
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
