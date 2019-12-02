#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#define STR_HOST "localhost"
#define STR_ID "root"
#define STR_PW "1234"
#define STR_DB "factory"

int init(MYSQL * mysql);
void destroy(MYSQL * mysql);
int readDB(MYSQL * mysql, char * buf, int size, int num);

int main()
{
	MYSQL mysql;
	init(&mysql);
	char buf[256];	
	readDB(&mysql, buf, 256, 30);
	destroy(&mysql);
}


int init(MYSQL * mysql)
{
	mysql_init(mysql);
	if(mysql_real_connect(mysql, STR_HOST, STR_ID, STR_PW, STR_DB, 0, NULL, 0))
	{
		printf("Connect with FACTORY databases success\n");
		return 0;
	}
	else
	{
		printf("(!)ERROR, Connect failde\n");
		return -1;
	}
}

void destroy(MYSQL * mysql)
{
	mysql_close(mysql);
	return;
}

int readDB(MYSQL * mysql, char * buf, int size, int num)
{
	char strQuery[256] = "";
	buf[0] = 0;

	sprintf(strQuery, "SELECT p_num, p_output, p_date, p_time FROM product WHERE p_num = %d;", num);

	int result = mysql_query(mysql, strQuery);
	if(result != 0)
	{
		return -1;
	}
	else
	{
		MYSQL_RES * res_ptr = mysql_use_result(mysql);
		MYSQL_ROW sqlRow = mysql_fetch_row(res_ptr);

		unsigned int field_count = 0;
		while(field_count < mysql_field_count(mysql))
		{
			char buf_field[256] = "";
			if(sqlRow[field_count])
				sprintf(buf_field, "|%s", sqlRow[field_count]);
			else sprintf(buf_field, "|0");
			
			printf("%s", buf_field);
			field_count++;
		}
		printf("\n");
		
		if(mysql_errno(mysql))
		{
			fprintf(stderr, "(!)ERROR: %s\n", mysql_error(mysql));
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	return 0;
}


