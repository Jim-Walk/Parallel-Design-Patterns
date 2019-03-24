#include <mpi.h>
#include <thread>
#include <chrono>
#include "../include/master.hpp"
#include "../lib/pool.h"

void Master::run(){
    set_up();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    kill_squirrels();
    birth_squirrels();
   
    if (live_squirrels == 0){
    // End the simulation by active changing to false
        active = false;
    }
}

void Master::set_up(){
    int workerRank;
    actor_type cmd;
    // Create grid cells
    for (int i=0; i<num_grid_cells; i++){
        cmd = actor_type::GRID; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        grid_ids.push_back(workerRank);
    }
    // Create healthy squirrles
    for (int i=0; i<total_squirrels-initial_inf_sq; i++){
        cmd = actor_type::SQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        squirrel_ids.push_back(workerRank);
    }
    // Create infected squirrels
    for (int i=initial_inf_sq; i<total_squirrels; i++){
        cmd = actor_type::INFSQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        squirrel_ids.push_back(workerRank);
    }
    // Create clock
    cmd = actor_type::CLOCK;
    workerRank = startWorkerProcess();
    MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
    active = true;
}

bool Master::is_active(){
    return active;
}

void Master::kill_squirrels(){
    bool recvd;
    int rank, msg = -1;
    std::tie(recvd, rank, msg) = msg_recv();
}
