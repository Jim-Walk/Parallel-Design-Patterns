#ifndef CONTROLLER_HPP 
#define CONTROLLER_HPP
#include "master.hpp"

class Controller : public Master {
    public:
        Controller(Actor const& a) : Master(a){
            active = true;
            month = 0;
        }

        void run();

    private:
        int month;
        void manage_squirrels();
};
#endif
