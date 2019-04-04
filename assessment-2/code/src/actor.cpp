#include <mpi.h>
#include <cstdlib>
#include <tuple>
#include "../lib/pool.h"
#include "../include/actor.hpp"

Actor::Actor(int id){
    this->id = id;
    buf = malloc( (sizeof(int)+MPI_BSEND_OVERHEAD)*200);
    MPI_Buffer_attach(&buf, (sizeof(int)+MPI_BSEND_OVERHEAD)*200);
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
    parent = getCommandData();
    MPI_Recv(&act_type, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, &status);
}

void Actor::run(){
    printf("this is a stub, please implement in child class\n");
}

void Actor::check_active(){
    if (shouldWorkerStop()){
        if (id == 19)
            printf("sq stopped\n");
        active = false;
    }
}

/* Returns a bool indicating if message was received, 
 * which rank it was recieved from, and the message
   itself.  */
std::tuple<bool, int, int> Actor::msg_recv(){
    int msg_flag = 0;
    int rank, msg = -1; // make sure rank and message refer to things that don't exist
    MPI_Status stat;
    MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&msg_flag, &stat);
    rank = stat.MPI_SOURCE;
    if (msg_flag){
        MPI_Recv(&msg, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, &stat);
    }
    return std::make_tuple(msg_flag == 1, rank, msg);
}

bool Actor::data_recv(int rank, float *data){
    MPI_Status stat;
    int msg_flag = 0;
    while (msg_flag != 1){
        MPI_Iprobe(rank, 0, MPI_COMM_WORLD,&msg_flag, &stat);
        if (shouldWorkerStop()){
            active = false;
            return false;
        }
    }
    if (msg_flag){ 
       MPI_Recv(data, 1, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, &stat);
    }
    return msg_flag == 1;
}

