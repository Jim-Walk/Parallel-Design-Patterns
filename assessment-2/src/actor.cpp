#include <mpi.h>
#include <cstdlib>
#include <tuple>
#include "../lib/pool.h"
#include "../include/actor.hpp"

Actor::Actor(int id){
    this->id = id;
    buf = malloc( sizeof(int)*40);
    MPI_Buffer_attach(&buf, 40);
}

void Actor::send_msg(int dest, int msg){
    MPI_Bsend(&msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

void Actor::send_data(int dest, float data){
    MPI_Bsend(&data, 1, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
}

int Actor::get_id(){
    return id;
}
Actor::actor_type Actor::get_type(){
    return act_type;
}

void Actor::start_up(){
    MPI_Status status;
    MPI_Recv(&act_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
}

void Actor::run(){
    printf("this is a stub, please implement in child class\n");
}

void Actor::check_active(){
    //std::cout << id << ": check if active" << std::endl; 
    if (shouldWorkerStop()){
        active = false;
//        std::cout << "Worker end on rank " << id << std::endl;
    }
}

/* Returns a bool indicating if message was received, 
 * which rank it was recieved from, and the message
   itself */
std::tuple<bool, int, int> Actor::msg_recv(){
    int rank, msg, msg_flag = 0;
    MPI_Status stat;
    MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&msg_flag, &stat);
    if (msg_flag){
        rank = stat.MPI_SOURCE;
        MPI_Recv(&msg, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, &stat);
    }
    return std::make_tuple(msg_flag == 1, rank, msg);
}

bool Actor::data_recv(float *data){
    MPI_Status stat;
    int pop_flag = 0;
    MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&pop_flag, &stat);
    if (pop_flag){
        MPI_Recv(&data, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
        //printf("%d: got pop from %d\n",id,cell);
    }
    return pop_flag == 1;
}

