#ifndef CONTROLLER_HPP 
#define CONTROLLER_HPP
#include "actor.hpp"

class Controller : public Actor {
    public:
        Controller(Actor const& a) : Actor(a){
        }

        void run();

    private:
        void manage_squirrels();
        void print_info();
        const int total_squirrels = 200;
        int live_squirrels = 15;
        int initial_inf_sq = 9;
};
#endif
