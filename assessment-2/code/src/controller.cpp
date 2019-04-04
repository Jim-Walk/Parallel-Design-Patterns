#include "../include/controller.hpp"
#include "../lib/pool.h"
#include <mpi.h>

void Controller::run(){

    while (active){
        manage_squirrels();
        // status should be masterPoll();
        if (live_squirrels < 1 || live_squirrels >= total_squirrels){
            printf("\n\t!!! ERROR !!!\n");
            printf("Illegal number of Squirrels: %d\n", live_squirrels);
            printf("\t!!! ERROR !!!\n\n");
            shutdownPool();
                break;
        }
        check_active();
    }
    printf("C%d: Final squirrels: %d Infected Squirrels: %d\n", id, live_squirrels, inf_count);
}

void Controller::manage_squirrels(){
    bool recvd;
    int rank, msg = -1;
    float loc_vec[2];
    
    std::tie(recvd, rank, msg) = msg_recv();
    if (recvd){
        if (msg == MSG::STOP){
            //std::cout << id << ": 🐿️   💀  ID: " << rank <<std::endl;
            live_squirrels--;
            inf_count--;
        } 
        else if (msg == MSG::START){
            MPI_Status stat;
            MPI_Recv(loc_vec, 2, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, &stat);
            int new_sq = startWorkerProcess();
            //std::cout << id << ": 🐿️  born ID: " << new_sq << std::endl;
            actor_type cmd = actor_type::SQ; 
            MPI_Ssend(&cmd, 1, MPI_INT, new_sq, 0, MPI_COMM_WORLD);
            MPI_Ssend(loc_vec, 2, MPI_FLOAT, new_sq, 0, MPI_COMM_WORLD);
            live_squirrels++;
        } 
        else if (msg == MSG::INFSTEP){
            inf_count++;    
            //std::cout << id << ": 🐿️   🤢  ID: " << rank <<std::endl;
        }
        else if (msg == MSG::TICK){
            month++;
            printf("C%d: month: %d live_squirrels: %d infected: %d \n", id, month, live_squirrels, inf_count);
        }
    }
}
