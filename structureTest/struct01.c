#include<stdio.h>


typedef struct
{
	int age;
	double battlePoint;
} Person;

int main()
{ 
	printf("*** Struct test Program ***\n");

	Person a;

	a.age = 300;
	a.battlePoint = 1030.77;

	printf("a.age = %d\n",a.age);
	printf("a.battlepoint = %lf\n", a.battlePoint);

	return 0;
}
