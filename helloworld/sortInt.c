#include<stdio.h>
void swap(int * pa, int * pb);
void line_up(int * maxp, int * midp, int * minp);

int main(void)
{
   int max, mid, min;
   printf("Enter Three of int : ");
   scanf("%d%d%d", &max, &mid, &min);
   line_up(&max, &mid, &min);
   printf("Print Sorted Number : %d, %d, %d\n", max, mid, min);
   
   return 0;
}

void swap(int * pa, int * pb)
{
   int temp;

   temp = *pa;
   *pa = *pb;
   *pb = temp;
}

void line_up(int * maxp, int * midp, int * minp)
{
  if(*maxp < *midp) 
	swap(maxp, midp);
  if(*midp < *minp)
	swap(midp, minp);
  if(*maxp < *minp)
	swap(maxp, minp);
  if(*maxp < *midp)
	swap(maxp, midp);
}

