#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

// Threshold of polution that needs to be cleaned
#define THRESHOLD_VALUE 18.75
// The maximum number of iterations
#define MAX_ITERATIONS 100000
// How often to report the norm
#define REPORT_NORM_PERIOD 1000

void initialise(double*, double*, int, int, double, double);
void read_files(int, char**);
void write_data();
void data_analysis(int, int);
void perform_calculation(int, int, double, int);
void average_sample_values();

double active_time;

int main(int argc, char * argv[]) {
	MPI_Init(&argc, &argv);
	active_time=0.0;
	double start_time=MPI_Wtime();

	if (argc < 5) {
  		fprintf(stderr, "You must provide four command line arguments, the global size in X, the global size in Y, convergence accuracy and max number iterations\n");
    		return -1;
  	}
	int nx=atoi(argv[1]);
  	int ny=atoi(argv[2]);
  	double convergence_accuracy=atof(argv[3]);
  	int max_its=atoi(argv[4]);

	int myrank, size;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Need to consider which MPI process will run which stage in pipeline and put it in here
	if (myrank == 0) {
		// First stage
	} else if (myrank == 1) {
		// Second stage
	}
	// etc for other stages of the pipeline

	// Calculate and display the load imbalance factor here (recall it is the maximum utilisation / average utilisation) - I suggest a couple of reductions for this, 
	// the first one summing up all the active_time and the second one finding the max of the average time. 
	// Then on the root you can divide the summed average time by the
	// number of processes to find the overall global average

	if (myrank == 0) printf("Overall runtime %f seconds\n", MPI_Wtime() - start_time);
	MPI_Finalize();
	return 0;
}

void read_files(int argc, char * argv[]) {
	FILE * fileHandle;
	char buffer[3000], *nextSep, *buffer_point;
	double extractedValues[60];
	int i, j,n;
	for (i=5;i<argc;i++) {
		double startTime=MPI_Wtime();
		fileHandle=fopen(argv[i], "r");
		if (fileHandle != NULL) {
			n=0;
			while (fgets(buffer, 3000, fileHandle) != NULL) {
				buffer_point=buffer;
				while (buffer_point != NULL) {
					nextSep=strchr(buffer_point, ',');
					if (nextSep != NULL) *nextSep='\0';
					extractedValues[n++]=atof(buffer_point);
					if (nextSep != NULL) {
						buffer_point=nextSep+1;
					} else {
						buffer_point=NULL;
					}
				}
			}
			fclose(fileHandle);
			active_time+=MPI_Wtime()-startTime;
			// Need to send the 60 values to the next stage in the pipeline
		} else {
			printf("Warning: File %s does not exist\n", argv[i]);
		}
	}
	// Need to send a "poisoned pill" termination message to the next stage in the pipeline
}

void average_sample_values() {
	double raw_sample_values[60], average_vals[2];
	int i;

	while (1==1) {
		// Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
		// The data from the previous stage is expected in the raw_sample_values array
		double startTime=MPI_Wtime();
		// We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
		// then check this number of elements, if it is zero you can break out of the loop	
		average_vals[0]=0.0;
		average_vals[1]=0.0;
		for (i=0;i<30;i++) {
			average_vals[0]+=raw_sample_values[i];
			average_vals[1]+=raw_sample_values[i+30];
		}
		average_vals[0]/=30;
		average_vals[1]/=30;
		active_time+=MPI_Wtime()-startTime;
		// Need to send the values to the next stage in the pipeline
	}
	// Need to send a "poisoned pill" termination message to the next stage in the pipeline
}

void write_data() {
	int analysed_data[2];	
	int file_number=0;
	char file_name[30];
	FILE * file_handle;

	while (1==1) {
		// Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
		// The data from the previous stage is expected in the analysed_data array
		double startTime=MPI_Wtime();
		// We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
		// then check this number of elements, if it is zero you can break out of the loop	
		sprintf(file_name, "results/results_%d", file_number);
		file_handle=fopen(file_name, "w");
		fprintf(file_handle, "Sample number %d\nPollution starts at grid point %d\nNumber of grid points over threshold that need cleaning %d\n", file_number, analysed_data[0],
				analysed_data[1]);
		fclose(file_handle);
		printf("Result file is: %s\n", file_name);
		file_number++;
		active_time+=MPI_Wtime()-startTime;
	}
}

void data_analysis(int nx, int ny) {
	int mem_size_x=nx+2;
	int mem_size_y=ny+2;

	double * u_k = malloc(sizeof(double) * mem_size_x * mem_size_y);
	int i, j, analysed_data[2];
	short past_threshold=0;	

	while (1==1) {
		// Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
		// The data from the previous stage is expected in the u_k array
		double startTime=MPI_Wtime();
		// We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
		// then check this number of elements, if it is zero you can break out of the loop	

		past_threshold=0;
		analysed_data[0]=analysed_data[1]=0;
		for (i=0;i<mem_size_x;i++) {
			for (j=0;j<mem_size_y;j++) {
				double value=u_k[j+(i*mem_size_y)];			
				if (value >= THRESHOLD_VALUE) {
					if (!past_threshold) {
						past_threshold=1;
						analysed_data[0]=i;
					}
					analysed_data[1]++;
				}
			}
		}
		active_time+=MPI_Wtime()-startTime;
		// Need to send the values to the next stage in the pipeline
	}
	// Need to send a "poisoned pill" termination message to the next stage in the pipeline
	free(u_k);
}

void perform_calculation(int nx, int ny, double convergence_accuracy, int max_its) {
	int mem_size_x=nx+2;
	int mem_size_y=ny+2;

	double * u_k = malloc(sizeof(double) * mem_size_x * mem_size_y);
	double * u_kp1 = malloc(sizeof(double) * mem_size_x * mem_size_y);
	double * temp, boundaryValues[2];	

	while (1==1) {
		// Need to receive (blocking call) a message from the previous rank, also handle the poisoned pill termination case
		// The data from the previous stage is expected in the boundaryValues array, element 0 for the left item and 1 for the right item
		double startTime=MPI_Wtime();
		// We suggest using MPI_Get_count on the status to figure out the number of elements (if it is zero then this is the poisoned pill)
		// then check this number of elements, if it is zero you can break out of the loop	

		initialise(u_k, u_kp1, nx, ny, boundaryValues[0], boundaryValues[1]);

		double rnorm=0.0, bnorm=0.0, norm;

		int i, j, k;
		for (j=1;j<=nx;j++) {
			for (i=1;i<=ny;i++) {
				bnorm=bnorm+pow(u_k[i+(j*mem_size_y)]*4-u_k[(i-1)+(j*mem_size_y)]-
					u_k[(i+1)+(j*mem_size_y)]-u_k[i+((j-1)*mem_size_y)]-u_k[i+((j+1)*mem_size_y)], 2);
			}
		}
		bnorm=sqrt(bnorm);

		for (k=0;k<MAX_ITERATIONS;k++) {
			rnorm=0.0;
    			// Calculates the current residual norm
    			for (j=1;j<=nx;j++) {
      				for (i=1;i<=ny;i++) {
        				rnorm=rnorm+pow(u_k[i+(j*mem_size_y)]*4-u_k[(i-1)+(j*mem_size_y)]-
          	  				u_k[(i+1)+(j*mem_size_y)]-u_k[i+((j-1)*mem_size_y)]-u_k[i+((j+1)*mem_size_y)], 2);
      				}
    			}
    			norm=sqrt(rnorm)/bnorm;
		
			if (norm < convergence_accuracy) break;
	    		if (max_its > 0 && k >= max_its) break;

			for (j=1;j<=nx;j++) {
      				for (i=1;i<=ny;i++) {
        				u_kp1[i+(j*mem_size_y)]=0.25 * (u_k[(i-1)+(j*mem_size_y)]+u_k[(i+1)+(j*mem_size_y)]+
          	  				u_k[i+((j-1)*mem_size_y)]+u_k[i+((j+1)*mem_size_y)]);
      				}
    			}

			temp=u_kp1; u_kp1=u_k; u_k=temp;
			rnorm=0.0;
		}
		active_time+=MPI_Wtime()-startTime;
		// Need to send all the pollution values to the next stage in the pipeline
	}
	// Need to send a "poisoned pill" termination message to the next stage in the pipeline
	free(u_k);
	free(u_kp1);
}

/**
 * Initialises the arrays, such that u_k contains the boundary conditions at the start and end points and all other
 * points are zero. u_kp1 is set to equal u_k
 */
void initialise(double * u_k, double * u_kp1, int nx, int ny, double leftValue, double rightValue) {
	int i,j;
  	for (i=0;i<nx+1;i++) {
    		u_k[i*(ny+2)]=leftValue;
    		u_k[(ny+1)+(i*(ny+2))]=rightValue;
  	}
  	for (j=0;j<=nx+1;j++) {
    		for (i=1;i<=ny;i++) {
      			u_k[i+(j*(ny+2))]=0.0;
    		}
  	}
  	for (j=0;j<=nx+1;j++) {
    		for (i=0;i<=ny+1;i++) {
      			u_kp1[i+(j*(ny+2))]=u_k[i+(j*(ny+2))];
    		}
  	}
}

