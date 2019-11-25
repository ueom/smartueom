#include<stdio.h>
#include<stdlib.h>


typedef struct person
{
	int age;
	double battlePoint;
	struct Person * link;

} Person;

int main()
{ 
	printf("*** Struct test Program3 ***\n");

	int age = 0;
	double battlePoint = 0;

	printf("Input age:");
	scanf("%d", &age);
	
	printf("Input BattlePoint: ");
	scanf("%lf", &battlePoint);

	Person * a = (Person*)malloc(sizeof(Person));
	a->age = age;	
	a->battlePoint = battlePoint;

	printf("a->age = %d\n",a->age);
	printf("a->battlepoint = %lf\n", a->battlePoint);

	free(a);

	return 0;
}
