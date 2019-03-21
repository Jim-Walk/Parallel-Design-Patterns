#include "ran2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "omp.h"

static void sort(double*, int, int);
static void merge(double*, int, int);
static int qsort_compare_function(const void*, const void*);
static double* generateUnsortedData(int);
static void displayData(double *, int);

int main(int argc, char * argv[]) {
	if (argc != 4) {
		fprintf(stderr, "You must provide three command line arguments, the number of elements to be sorted, the serial threshold and whether to display raw and sorted data\n");
		return -1;
	}
   omp_set_nested(1);
	int data_length=atoi(argv[1]);
	int serial_threshold=atoi(argv[2]);
	int should_displayData=atoi(argv[3]);	

	double * data = generateUnsortedData(data_length);
	if (should_displayData) {
		printf("Unsorted\n");
		displayData(data, data_length);
	}
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);
#pragma omp parallel
   {
#pragma omp single
      {
      sort(data, data_length, serial_threshold);
      }
   }
   gettimeofday(&end_time, NULL);
	if (should_displayData) {
		printf("\nSorted\n");
		displayData(data, data_length);
	}
	long int elapsedtime = (end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec);
	printf("Runtime is %ld ns\n", elapsedtime);
	return 0;
}

static void sort(double * data, int length, int serial_threshold) {
	if (length < serial_threshold) {
		qsort(data, length, sizeof(double), qsort_compare_function);
	} else {
         int pivot=length/2;

         sort(data, pivot, serial_threshold);
         
         sort(&data[pivot], length-pivot, serial_threshold);
         merge(data, pivot, length);
    }
}

static int qsort_compare_function(const void * a, const void * b) {
	double d1=*((double*) a);
	double d2=*((double*) b);
	if (d1 < d2) return -1;
	if (d1 > d2) return 1;
	return 0;
}

static void merge(double * data, int pivot, int length) {
	double new_data[length];
	int i, pre_index=0, post_index=pivot;
	for (i=0;i<length;i++) {
		if (pre_index >= pivot) {
			new_data[i]=data[post_index++];
		} else if (post_index >= length) {
			new_data[i]=data[pre_index++];
		} else if (data[pre_index] < data[post_index]) {
			new_data[i]=data[pre_index++];
		} else {
			new_data[i]=data[post_index++];
		}
	}
	memcpy(data, new_data, sizeof(double) * length);
}

static double* generateUnsortedData(int data_length) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long seed = -1-tv.tv_usec;
	ran2(&seed);
	int i;
	double * data = (double*) malloc(sizeof(double) * data_length);
	for (i=0;i<data_length;i++) {
		data[i]=(double) ran2(&seed);
	}
	return data;
}

static void displayData(double * data, int length) {
	int i;
	for (i=0;i<length;i++) {
		printf("%.3f ", data[i]);
	}
	printf("\n");
}
