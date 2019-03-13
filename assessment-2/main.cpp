#include <iostream>
#include <string>
#include "./include/actor.hpp"

int main(){
    Actor a = Actor(1);
    std::cout << "Created actor with ID " << a.get_id() << std::endl;
    return 0;
}
