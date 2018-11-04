#include<stdlib.h>	//maloc and free
#include<stdio.h>	//printf
#include<omp.h>	//opemp

#define ARRAY_SIZE 8	//size of arrays whose size will be added together
#define NUM_THREADS 4	//number of threads to use for vector addition 

int main(int argc,char *argv[])
{
	int *a;
	int *b;
	int *c;

	int n=ARRAY_SIZE;				//number of array elements
	int n_per_thread;				//elements per thread
	int total_threads=NUM_THREADS;	//number of threads to use
	int i;							//loop index
	
	//allocate space for arrays
	a=(int *)malloc(sizeof(int)*n);
	b=(int *)malloc(sizeof(int)*n);
	c=(int *)malloc(sizeof(int)*n);

	//initialize arrya a and b with consecutive iteger values as an example

	for(i=0;i<n;i++)
	{
		a[i]=i;
	}
	for(i=0;i<n;i++)
	{
		b[i]=i;
	}

	omp_set_num_threads(total_threads);

	//determine how many elements each proces wil  work on 

	n_per_thread=n/total_threads;

#pragma omp parallel for shared(a,b,c) private(i) schedule (static, n_per_thread)
	for(i=0;i<n;i++)
	{
		c[i]=a[i]+b[i];
		//which threa am i? shows who works on what for this mall example
		printf("Thread %d works on element %d\n",omp_get_thread_num(),i);
	}
	printf("i\ta[i]\t+\tb[i]\t=\tc[i]\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t%d\t\t%d\t\t%d\n",i,a[i],b[i],c[i]);
	}
	//clean up memory
	free(a); free(b); free(c);
	return 0;
}
