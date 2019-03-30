#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include "actor.hpp"

class Grid_cell : Actor {
    public:
        Grid_cell(Actor const& a) : Actor(a) {
            active = true;
            avg_pop = 0;
            avg_inf = 0;

            inf_count = 0;
            pop_count = 0;
            month_update = false;
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
        void exchange_pop_and_inf();
};

#endif
