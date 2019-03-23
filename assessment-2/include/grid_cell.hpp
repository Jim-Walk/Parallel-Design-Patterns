#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include "actor.hpp"

class Grid_cell : Actor {
    public:
        Grid_cell(Actor const& a) : Actor(a) {
            active = true;
        }
        void run();
    private:
        // values
        float avg_inf;
        float inf_count;

        float avg_pop;
        float pop_count;
        
        bool month_update;

        // functions
        void check_for_month();
        void exchange_pop();
        void exchange_inf();
};

#endif
