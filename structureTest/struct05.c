#include<stdio.h>
#include<stdlib.h>


typedef struct person
{
	int id;
	int age;
	double battlePoint;
	struct person * link;

} Person;

int gIssuedId = 0;

int main()
{ 
	printf("*** Struct Test Program5 ***\n");
	
	int n=0;
	printf("Input a number: ");
	scanf("%d", &n);

	int i=0;
	Person * head = NULL;

	for(i=0; i<n; i++)
	{

		int age = 0;
		double battlePoint = 0;

		printf("Input age:");
		scanf("%d", &age);
		
		printf("Input battle point: ");
		scanf("%lf", &battlePoint);

		Person * a = (Person*)malloc(sizeof(Person));
		a->id = gIssuedId++;
		a->age = age;	
		a->battlePoint = battlePoint;
		a->link = NULL;

		if(head==NULL) head = a;
		else
		{
			Person * current = head;
			while(current->link!=NULL) current=current->link;
			current->link = a;
		}
	}

	Person * current = head;
	while(current!=NULL)
	{
		printf("%d age = %d\n", current->id, current->age);
		printf("%d battlePoint = %lf\n", current->id, current->battlePoint);
		current=current->link;
	}

	//#########Finding#########################################
	int n2=0;
	printf("Enter the id that you want to find: ");
	scanf("%d", &n2);

	current = head;
	i = 0;
	for(i=0; i<n2; i++)
	{
		current = current->link;
	}
	
	printf("The Information of %d\n",n2);	
	printf("%d age = %d battlePoint = %lf\n", current->id, current->age, current->battlePoint);

	//####################################################
	current = head;
	while(current!=NULL)
	{
		Person * a = current;
		current=current->link;
		free(a);
	}

	return 0;
}
