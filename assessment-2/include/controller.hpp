#ifndef CONTROLLER_HPP 
#define CONTROLLER_HPP
#include "master.hpp"

class Controller : public Master {
    public:
        Controller(Actor const& a) : Master(a){
            active = true;
        }

        void run();

    private:
        void manage_squirrels();
        void print_info();
};
#endif
