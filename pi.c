/* Example source code generated for Cpt S 411: Intro to Parallel Computing
 * Author: Ananth Kalyanaraman
 * Institution: Washington State University
 * Disclaimer: Use at your own risk!
 *
 * Alex Shirk & Clancy Andrews
 * Cpts 411 
 * Fall 2023
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

#define PI 3.141592653589793238462643383279502884197 //https://www.britannica.com/science/pi-mathematics

int p = 1;

int computeHits(int );

int main(int argc, char *argv[])
{
	int i;
	int  n;

	// loop {number of iterations} [number of threads]

	if(argc<2) {
		printf("Usage: pi n [number of threads]\n");
		exit(1);
	}
	
	n = atoll(argv[1]);
	printf("Debug: n= %d \n",n);

	if(argc==3) {
		p = atoi(argv[2]);
		assert(p>=1);
		printf("Debug: number of requested threads = %d\n",p);
	}

	omp_set_num_threads(p);

	for(n = 1024; n <=1048576;n = n*2) {

		#pragma omp parallel
		{
			assert(p==omp_get_num_threads());
			//printf("Debug: number of threads set = %d\n",omp_get_num_threads());

			int rank = omp_get_thread_num();
			printf("Rank=%d: my world has %d threads\n",rank,p);
		}  // end of my omp parallel region

		double time = omp_get_wtime();

		//dispArray(a,n);

		// 2. compute sum using reduce
		int hits = computeHits(n);
		double pi_estimate = (hits / (double) n) * 4; //hits / n * 4
		printf("%f\n", pi_estimate);

		//Difference between PI and the estimated value
		double diff = abs(PI - pi_estimate);

		time = omp_get_wtime() - time;

		//printf("Total time = %f seconds (using %d threads)\n ", time, p);
		printf("Threads: %d\nDarts: %d\nDifference: %f\nTime: %f\n\n",p,n,diff,time);
		}

	return 0;
}


/* This function computes the sum of the n elements in array a, in parallel (across p threads).
 */
int inCircle(double x, double y) {
	//(x-a)^2 + (y-b)^2 = r^2; a, b, and r are .5
	return (x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) < 0.5 * 0.5;
}

double randomDouble() { //generates a random double between 0 and 1.
	return rand() / (double) RAND_MAX;
}

int computeHits(int n) {

	omp_set_num_threads(p);

	int i;
	int hits=0;

	#pragma omp parallel for shared(n) reduction(+:hits) 
	for(i=0;i<n;i++) {
		double x = randomDouble();
		double y = randomDouble();
		if (inCircle(x, y)) {
			hits++;
		}
		//printf("i=%d, n=%d, hits=%d,a[i]=%d\n",i,n,hits,a[i]);
		//fflush(stdout);
	} // end for

	// Q) The hits variable is being written onto 

	// Q) Which thread will have the final hits to print?
	printf("hits=%d\n",hits);
	return hits;

}// end computeHits

