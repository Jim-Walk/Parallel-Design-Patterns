#include <mpi.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include "../include/master.hpp"
#include "../lib/pool.h"

void Master::run(){
    set_up();
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    int masterStatus = 1;
    while (masterStatus){
        masterStatus = masterPoll();
        manage_squirrels();
        // status should be masterPoll();
    }
   
    // End the simulation by active changing to false
    active = false;
    printf("final num squirrels %d \n", live_squirrels);
    
}

void Master::manage_squirrels(){
    bool recvd;
    int rank, msg = -1;
    
    // if a squirrel has died, add its id to the dead
    // squirrels pool
    
    std::tie(recvd, rank, msg) = msg_recv();
    if (msg == MSG::STOP){
        printf("wtf squirrel %d died \n", rank);
        std::remove_if( squirrel_ids.begin(), squirrel_ids.end(),
                       [rank](int elem){return elem == rank;} );
       dead_squirrel_ids.push_back(rank); 
       live_squirrels--;
    } 
    else if (msg == MSG::START){
        int new_sq = startWorkerProcess();
        printf("squirrel %d born \n", new_sq);
        actor_type cmd = actor_type::SQ; 
        MPI_Ssend(&cmd, 1, MPI_INT, new_sq, 0, MPI_COMM_WORLD);
        std::remove_if( dead_squirrel_ids.begin(), dead_squirrel_ids.end(),
                       [rank](int elem){return elem == rank;} );
        squirrel_ids.push_back(new_sq);
        live_squirrels++;
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
        inf_squirrel_ids.push_back(workerRank);
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

