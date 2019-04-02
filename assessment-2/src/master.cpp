#include <mpi.h>
#include <algorithm>
#include "../include/master.hpp"
#include "../lib/pool.h"

void Master::run(){
    set_up_sim();
    int masterStatus = 1;
    while (masterStatus){
        masterStatus = masterPoll();
    }
   
    // End the simulation by active changing to false
    active = false;
    
}


void Master::set_up_sim(){
    int workerRank;
    actor_type cmd;
    printf("Master %d: Creating sim with %d squirrels, of which %d are infected\n", id, live_squirrels, initial_inf_sq);

    float loc_vec[2] = {2.0, 2.0}; // all initial squirells are created at this location
    // Create grid cells - we do this first so they always have rank 1-16
    for (int i=0; i<num_grid_cells; i++){
        cmd = actor_type::GRID; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
    }
    // Create Controller with rank 17
    cmd = actor_type::CONTROL;
    workerRank = startWorkerProcess();
    MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
    // Create clock with rank 18
    cmd = actor_type::CLOCK;
    workerRank = startWorkerProcess();
    MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
    // Create healthy squirrles
    for (int i=0; i<live_squirrels-initial_inf_sq; i++){
        cmd = actor_type::SQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
    }
    // Create infected squirrels
    for (int i=live_squirrels-initial_inf_sq; i<live_squirrels; i++){
        cmd = actor_type::INFSQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
    }
    active = true;
}
