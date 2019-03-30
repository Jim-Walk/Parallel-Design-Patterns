#include <mpi.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include "../include/master.hpp"
#include "../lib/pool.h"

void Master::run(){
    set_up_sim();
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    int masterStatus = 1;
    while (masterStatus){
        masterStatus = masterPoll();
        manage_squirrels();
        // status should be masterPoll();
        if (live_squirrels < 1 || live_squirrels >= total_squirrels){
            printf("\n\t!!! ERROR !!!\n");
            printf("Illegal number of Squirrels: %d\n", live_squirrels);
            printf("\t!!! ERROR !!!\n");
                break;
        }
    }
   
    // End the simulation by active changing to false
    active = false;
    printf("Master %d: final num squirrels %d \n", id, live_squirrels);
    
}

void Master::manage_squirrels(){
    bool recvd;
    int rank, msg = -1;
    float loc_vec[2];
    // if a squirrel has died, add its id to the dead
    // squirrels pool
    
    std::tie(recvd, rank, msg) = msg_recv();
    if (msg == MSG::STOP){
        //std::cout << "🐿️   💀  ID: " << rank <<std::endl;
        std::remove_if( squirrel_ids.begin(), squirrel_ids.end(),
                       [rank](int elem){return elem == rank;} );
       dead_squirrel_ids.push_back(rank); 
       live_squirrels--;
    } 
    else if (msg == MSG::START){
        MPI_Status stat;
        MPI_Recv(loc_vec, 2, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, &stat);
        int new_sq = startWorkerProcess();
        //std::cout <<  "🐿️  born ID: " << new_sq << std::endl;
        actor_type cmd = actor_type::SQ; 
        MPI_Ssend(&cmd, 1, MPI_INT, new_sq, 0, MPI_COMM_WORLD);
        MPI_Ssend(loc_vec, 2, MPI_FLOAT, new_sq, 0, MPI_COMM_WORLD);
        std::remove_if( dead_squirrel_ids.begin(), dead_squirrel_ids.end(),
                       [rank](int elem){return elem == rank;} );
        squirrel_ids.push_back(new_sq);
        live_squirrels++;
    } 
    else if (msg == MSG::TICK){
        printf("Master %d: live_squirrels: %d \n", id, live_squirrels);
    }
    
}

void Master::set_up_sim(){
    int workerRank;
    actor_type cmd;
    printf("Master %d: Creating sim with %d squirrels, of which %d are infected\n",
            id, live_squirrels, initial_inf_sq);

    float loc_vec[2] = {2.0, 2.0}; // all initial squirells are created at this location
    // Create grid cells - we do this first so they always have rank 1-16
    for (int i=0; i<num_grid_cells; i++){
        cmd = actor_type::GRID; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        grid_ids.push_back(workerRank);
    }
    // Create clock - we do this second so it will alway have rank 17
    cmd = actor_type::CLOCK;
    workerRank = startWorkerProcess();
    MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
    active = true;
    // Create healthy squirrles
    for (int i=0; i<live_squirrels-initial_inf_sq; i++){
        cmd = actor_type::SQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
        squirrel_ids.push_back(workerRank);
    }
    // Create infected squirrels
    for (int i=live_squirrels-initial_inf_sq; i<live_squirrels; i++){
        cmd = actor_type::INFSQ; 
        workerRank = startWorkerProcess();
        MPI_Ssend(&cmd, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(loc_vec, 2, MPI_FLOAT, workerRank, 0, MPI_COMM_WORLD);
        inf_squirrel_ids.push_back(workerRank);
    }
}

bool Master::is_active(){
    return active;
}

