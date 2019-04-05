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
    printf("Master %d: Creating sim with %d squirrels, of which %d are infected\n", id, live_squirrels, inf_count);

    float loc_vec[2] = {2.0, 2.0}; // all initial squirells are created at this location
    // Create grid cells - we do this first so they always have rank 1-16
    for (int i=0; i<num_grid_cells; i++){
        workerRank = startWorkerProcess();
        send_msg(workerRank, actor_type::GRID);
    }
    // Create Controller with rank 17
    workerRank = startWorkerProcess();
    send_msg(workerRank, actor_type::CONTROL);
    // Create clock with rank 18
    workerRank = startWorkerProcess();
    send_msg(workerRank, actor_type::CLOCK);
    // Create healthy squirrles
    for (int i=0; i<live_squirrels-inf_count; i++){
        workerRank = startWorkerProcess();
        send_msg(workerRank, actor_type::SQ);
        //TODO Abstract this
        MPI_Bsend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
    }
    // Create infected squirrels
    for (int i=live_squirrels-inf_count; i<live_squirrels; i++){
        workerRank = startWorkerProcess();
        send_msg(workerRank, actor_type::INFSQ);
        //TODO Abstract this
        MPI_Bsend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
    }
    active = true;
}
