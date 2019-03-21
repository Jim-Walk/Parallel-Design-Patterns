#include <iostream>
#include <string>
#include "./include/actor.hpp"
#include "./include/master.hpp"
#include "./include/grid_cell.hpp"
#include "./include/squirrel.hpp"
#include <mpi.h>
#include "./lib/pool.h"

static void worker_code();

int main(){
    MPI_Init(NULL, NULL);
    int statusCode = processPoolInit();
    int world_size;
    MPI_Comm world = MPI_COMM_WORLD;
    MPI_Comm_size(world, &world_size);


    if (statusCode == 1){
        worker_code();
    } else if (statusCode == 2){
        
        int myRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        int total_squirrels = 4;
        printf("master rank %d \n", myRank); 
        Master master = Master(myRank);
        master.set_total_squirrels(total_squirrels);
        master.set_up();
        // Start grid cell and squirell processes

        while (master.is_running()){
            master.run_simulation();
        }
        std::cout << "master complete" << std::endl;
    }


    processPoolFinalise();
    MPI_Finalize();
    return 0;
}


static void worker_code(){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Actor a = Actor(rank);
    a.start_up();
    if (a.get_type() == 2){
       Grid_cell gc = Grid_cell(a); 
       gc.run();
    } else if (a.get_type() == 1){
        Squirrel sq = Squirrel(a);
        sq.run();
    }
    
    // recv squirell or grid cell command
    // if squirrel
    //      while not dead squirrel
    //              do squirrel stuff
    //              maybe die
    //
    //      while wait
    //              check if i'm wake or simulation ends

    std::cout << "Worker end on rank " << rank << std::endl;
}
