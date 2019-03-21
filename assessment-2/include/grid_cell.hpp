#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include "actor.hpp"

class Grid_cell : Actor {
    public:
        void run();
    private:
        float avg_inf;
        int inf_count;

        float avg_pop;
        int pop_count;

        bool month_update;
};

#endif
