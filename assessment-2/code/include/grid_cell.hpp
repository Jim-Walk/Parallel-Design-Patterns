#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include "actor.hpp"

class Grid_cell : Actor {
    public:
        Grid_cell(Actor const& a) : Actor(a) {
            active = true;
            population_influx = 0;
            infection_level = 0;

            inf_count = 0;
            pop_count = 0;
            month_update = false;
        }
        void run();
    private:
        // values
        float infection_level;
        float inf_count;

        float population_influx;
        float pop_count;
        
        bool month_update;

        // functions
        void communicate();
};

#endif
