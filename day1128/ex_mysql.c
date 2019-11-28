#include <stdio.h>
#include <mysql/mysql.h>

int main()
{
	MYSQL mysql;
	MYSQL_RES * myresult;
	MYSQL_ROW row;
	unsigned int num_fields;
	unsigned int num_rows;
	char * string_query;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "1234", "mes", 0, NULL, 0);
	string_query = "select * from product\n";
	mysql_query(&mysql, string_query);
	myresult = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(myresult))
		printf("%s\t %s\n", row[0], row[1]);
	mysql_free_result(myresult);
	mysql_close(&mysql);
	return 0;
}
