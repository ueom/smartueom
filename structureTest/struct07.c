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
	printf("*** Struct Test Program7 ***\n");
	
	int n=0;
	printf("Input a number: ");
	scanf("%d", &n);

	int i=0;
	Person * head = NULL;

	for(i=0; i<n; i++)
	{

		int age = 0;
		double battlePoint = 0;

		printf("Input age: ");
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

	//#########Finding#########################################
	int n2=0;
	printf("----------------------------------------------\n");
	printf("Enter the id that you want to find (#0 to #%d): ", n-1);
	scanf("%d", &n2);
/*	if( n2 < 0 || n2 >=n)
	{
		printf("That Id #%d isn't exist in this structure\n", n2);
		printf("Please Enter the id that you want to find #0 to #%d : ",n-1);
		scanf("%d", &n2);
	}
*/
	Person * current = head;
	i = 0;
	for(i=0; i<n2; i++)
	{
		current = current->link;
	}
	
	printf("<Information of %d>\n",n2);	
	printf("%d\t age = %d\t battlePoint = %lf\n", current->id, current->age, current->battlePoint);

	//#########Change Node####################################
	printf("---------------------------------------------\n");
	printf("<Change #%d's Information>\n",n2);

	int c_age = 0;
	double c_battlePoint = 0;

	printf("Input age: ");
	scanf("%d", &c_age);

	printf("Input battlePoint: ");
	scanf("%lf", &c_battlePoint);

	Person * c_a = current;
	c_a->age = c_age;
	c_a->battlePoint = c_battlePoint;

	current = head;
	while(current!=NULL)
	{
		printf("%d\t age = %d\t battlePoint = %lf\n", current->id, current->age, current->battlePoint);	
	//	printf("%d age = %d\n", current->id, current->age);
	//	printf("%d battlePoint = %lf\n", current->id, current->battlePoint);
		current = current->link;
	}	

	//#########Add Node########################################	
	printf("----------------------------------------------\n");	
	printf("<Add New Node in #%d>\n,n_add);

	//#########Clear###########################################
	current = head;
	while(current!=NULL)
	{
		Person * a = current;
		current=current->link;
		free(a);
	}

	return 0;
}
