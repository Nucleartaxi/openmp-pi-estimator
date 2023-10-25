/* Example source code generated for Cpt S 411: Intro to Parallel Computing
 * Author: Ananth Kalyanaraman
 * Institution: Washington State University
 * Disclaimer: Use at your own risk!
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

int p=1;

int *generateArray(int);
void dispArray(int *,int);
void computeSum(int *,int );

int main(int argc, char *argv[])
{
	int i;
	int  n;

	// loop {number of iterations} [number of threads]

	if(argc<2) {
		printf("Usage: sumcomp {array size} [number of threads]\n");
		exit(1);
	}
	
	n = atoll(argv[1]);
	printf("Debug: array size = %d \n",n);

	if(argc==3) {
		p = atoi(argv[2]);
		assert(p>=1);
		printf("Debug: number of requested threads = %d\n",p);
	}

	omp_set_num_threads(p);

	#pragma omp parallel
	{
		assert(p==omp_get_num_threads());
		//printf("Debug: number of threads set = %d\n",omp_get_num_threads());

		int rank = omp_get_thread_num();
		printf("Rank=%d: my world has %d threads\n",rank,p);
	}  // end of my omp parallel region

	double time = omp_get_wtime();

	// 1. generate the array
	//
	//
	int *a=NULL;
	a = generateArray(n);

	//dispArray(a,n);

	// 2. compute sum using reduce
	computeSum(a,n);


	
	time = omp_get_wtime() - time;
	printf("Total time = %f seconds (using %d threads)\n ", time, p);

	return 0;
}

/* This function generates an array of size n in parallel (across p threads).
 * */
int *generateArray(int n) {

	int bound = 20;

	int *a = (int *)malloc(sizeof(int)*n);
	assert(a!=NULL);

	printf("n=%d, p=%d\n",n,p);

	omp_set_num_threads(p);

	//#pragma omp parallel default(none) shared(a,n) private (i)
	#pragma omp parallel
	{

		int i=0;
		#pragma omp for 
		for(i = 0; i < n; i++)  //Can you think of why we need this to be #pragma omp for? (as opposed to just a for loop)?
		{

			int rank = omp_get_thread_num();
			int seed = rank+1;
			seed = seed*i;		// I am trying to make "seed" a function of the thread rank and the iteration variable 
			
			//printf("rank=%d: seed = %d, i=%d,n=%d\n",rank,seed,i,n);
			a[i] = rand_r(&seed) % bound; //rand_r makes it the reentrant version

		}

	}

	return a;
} // end generateArray


/* This function computes the sum of the n elements in array a, in parallel (across p threads).
 */
void computeSum(int *a,int n) {

	omp_set_num_threads(p);

	int i;
	int sum=0;

	#pragma omp parallel for shared(a,n) reduction(+:sum) 
	for(i=0;i<n;i++) {
		//printf("i=%d, n=%d, sum=%d,a[i]=%d\n",i,n,sum,a[i]);
		//fflush(stdout);
		sum+=a[i];	// Q) will this be a thread-safe write?
	} // end for

	// Q) The sum variable is being written onto 

	// Q) Which thread will have the final sum to print?
	printf("sum=%d\n",sum);

}// end computeSum

/* This is a simple display function for the array for your testing purposes.
 */

void dispArray(int *a,int n) {
	int i;
	printf("********************\n");
	printf("Debug: Array a is: \n");
	for(i=0;i<n;i++) {
		printf("%d\n",a[i]);
	}
	printf("********************\n");
}

