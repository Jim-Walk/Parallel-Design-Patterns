#include <mpi.h>
#include <iostream>
#include <string>
#include "./include/actor.hpp"
#include "./include/master.hpp"
#include "./include/grid_cell.hpp"
#include "./include/squirrel.hpp"
#include "./include/clock.hpp"
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
        // Start grid cell and squirell processes
        
        master.run();
        
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
    if (a.get_type() == Actor::actor_type::GRID){
       Grid_cell gc = Grid_cell(a); 
       gc.run();
    } else if (a.get_type() == Actor::actor_type::SQ || a.get_type() == Actor::actor_type::INFSQ){
        Squirrel sq = Squirrel(a);
        sq.run();
    } else if (a.get_type() == Actor::actor_type::CLOCK){
        Clock c = Clock(a);
        c.run();
    }
}
