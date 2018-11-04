#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN], sum;

//orphaned is used when vector sizes are different

float dotprod ()
{
	int i,tid;
	tid = omp_get_thread_num();
	#pragma omp for reduction(+:sum)		//reduction operation here
	for (i=0; i < VECLEN; i++)
    {
		sum = sum + (a[i]*b[i]);
		printf("  tid= %d i=%d\n",tid,i);
    }
	return 0.0;
}

int main (int argc, char *argv[]) 
{
	int i;
	for (i=0; i < VECLEN; i++)
	a[i] = b[i] = 1.0 * i;
	sum = 0.0;
	#pragma omp parallel			//parallel operation here
	dotprod();
	printf("Sum = %f\n",sum);
}

