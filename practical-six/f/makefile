SRC = ran2.F90 qsort.F90 mergesort.F90
FFLAGS=-fopenmp
LFLAGS=-lm

FTN=gfortran

all: 
	$(FTN) -o mergesort $(FFLAGS) $(SRC) $(LFLAGS)

clean:
	rm mergesort *.mod
