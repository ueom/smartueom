#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	MYSQL mysql;
	mysql_init(&mysql);
	if(mysql_real_connect(&mysql, "localhost", "root", "1234", "plant", 0, NULL, 0))
		printf("Connection success\n");
	char query[255] = "";
	int res;
	for(int i = 0; i < 5; i++)
	{
		int temperature = rand()%61 + 30;

		if(temperature >= 70)
		{
			sprintf(query, "insert into machine(machineID, machineTemperature, machineRisk) values (NULL, %d, %d)\n", temperature, 0);
		}
		else
		{
			sprintf(query, "insert into machine(machineID, machineTemperature, machineRisk) values (NULL, %d, %d)\n", temperature, 1);
		}	
	res = mysql_query(&mysql, query);

	if(!res)
		printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(&mysql));
	else
		fprintf(stderr, "Insert error %d : %s\n", mysql_errno(&mysql), mysql_error(&mysql));	
	}

	mysql_close(&mysql);

	return 0;
}
