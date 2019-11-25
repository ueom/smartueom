#include<stdio.h>
#include<stdlib.h>

typedef struct
{
	int age;
	double battlePoint;
} Person;

int main()
{ 
	printf("*** Struct test Program ***\n");

	Person * a = (Person*)malloc(sizeof(Person));

	printf("Sizeof(Person)= %d\n",(int)sizeof(Person));
	printf("address size Person = %d\n",(int)sizeof(Person));
	printf("address size char* = %d\n", (int)sizeof(char*));
	printf("Sizeof Int = %d\n", (int)sizeof(int));

	a->age = 300;
	a->battlePoint = 1030.77;

	printf("a->age = %d\n",a->age);
	printf("a->battlepoint = %lf\n", a->battlePoint);

	free(a);

	return 0;
}
