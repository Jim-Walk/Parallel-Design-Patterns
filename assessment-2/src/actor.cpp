#include <mpi.h>
#include <cstdlib>
#include "../include/actor.hpp"

Actor::Actor(int id){
    this->id = id;
    MPI_Buffer_attach(malloc( sizeof(int)*40), 40);
}

void Actor::send(int dest, int msg){
    MPI_Bsend(&msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

int Actor::get_id(){
    return id;
}
int Actor::get_type(){
    return worker_type;
}

void Actor::start_up(){
    MPI_Status status;
    MPI_Recv(&worker_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
}

void Actor::run(){
    printf("this is a stub, please implement in child class\n");
}
