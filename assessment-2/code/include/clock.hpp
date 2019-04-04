#ifndef CLOCK_HPP
#define CLOCK_HPP
#include "actor.hpp"

class Clock : public Actor {
    public:
        Clock(Actor const& a) : Actor(a){
            active = true;
            months = 12;
        }

        void set_months(int months);
        int get_months();
        void run();

    private:
        int months;

        void tick();
};
#endif
