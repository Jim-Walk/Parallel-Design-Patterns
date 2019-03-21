#include <iostream>
#include <string>
#include "./include/actor.hpp"
#include "./include/master.hpp"
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


        int master_status = masterPoll();
        while (master_status){
            master_status = masterPoll();
        }

    }


    processPoolFinalise();
    MPI_Finalize();
    return 0;
}


static void worker_code(){
    int worker_status = 1;
    while (worker_status){
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        Actor a = Actor(rank);
        std::cout << "Worker start on rank " << rank << std::endl;
        a.start_up();
        std::cout << "Worker type " << a.get_type() << std::endl;
        // recv squirell or grid cell command
        // if squirrel
        //      while not dead squirrel
        //              do squirrel stuff
        //              maybe die
        //
        //      while wait
        //              check if i'm wake or simulation ends

        worker_status = workerSleep();
    }
}
