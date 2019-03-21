#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include "actor.hpp"

class Grid_cell : Actor {
    public:
        Grid_cell(Actor const& a) : Actor(a) {
        }
        void run();
    private:
        // values
        float avg_inf;
        int inf_count;

        float avg_pop;
        int pop_count;
        
        bool month_update;

        // functions
        void check_for_month();
        void exchange_pop();
        void exchange_inf();
        void check_active();
};

#endif
