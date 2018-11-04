#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define SIZE 10

int main (int argc, char *argv[]) 
{
	float u[SIZE], v[SIZE], dp,dpp;
	int i, j, tid;
	dp=0.0;
	
	for(i=0;i<SIZE;i++)
	{
		u[i]=1.0*(i+1);
		v[i]=1.0*(i+2);
	}
	printf("\n values of u and v:\n");
	for (i=0;i<SIZE;i++)
	{
	printf(" u[%d]= %.1f\t v[%d]= %.1f\n",i,u[i],i,v[i]);
	}
	#pragma omp parallel shared(u,v,dp,dpp) private (tid,i)
	{
		tid=omp_get_thread_num();
		#pragma omp for private (i)
		for(i=0;i<SIZE;i++)
		{
			dpp+=u[i]*v[i];
			printf("thread: %d\n", tid);
		}
		#pragma omp critical
		{
			dp=dpp;
			printf("thread %d\n",tid);
		}
	}
	printf("\n dot product is %f\n",dp);
}

