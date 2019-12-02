#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main(int argc, char * argv[])
{
	MYSQL * mysql;
	MYSQL_RES * result;
	MYSQL_ROW row;

	char query_buffer[2048];
	mysql = mysql_init(NULL);

	if(!mysql_real_connect(mysql, "localhost", "root", "1234", "factory", 0, NULL, 0))
	{
		printf("Connect ERROR(!)");
		exit(1);
	}
	else
	{
		printf("Factory Database is connected\n");
	}

	sprintf(query_buffer, "%s", "show tables;");
	if(mysql_query(mysql, query_buffer))
	{
		printf("query faild: %s\n", query_buffer);
		exit(1);
	}

	result = mysql_use_result(mysql);
	while((row = mysql_fetch_row(result)) != NULL)
		printf("%s \n", row[0]);

	mysql_free_result(result);
	mysql_close(mysql);
}
