#include<stdio.h>

int fibonacci(int n); 

int main(void)
{
  int num;
  int i = 0;

  printf("Enter one Integer : ");
  scanf("%d", &num);

  for( i = 0; i < num; i++)
  { 
	printf("%d  ", fibonacci(i));
  }
  printf("\n");
}

int fibonacci(int n)
{
  if(n==0)
  {
   return 1;
  }
  else if(n ==1)
  {
   return 1;
  }
  else
  {
   return fibonacci(n-1) + fibonacci(n-2);
  }
}
