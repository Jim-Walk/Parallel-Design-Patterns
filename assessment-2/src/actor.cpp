#include "../include/actor.hpp"

Actor::Actor(int id){
    this->id = id;
}

const int Actor::get_id(){
    return id;
}
