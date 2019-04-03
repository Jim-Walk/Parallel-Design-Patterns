#ifndef MASTER_HPP
#define MASTER_HPP
#include "../include/actor.hpp"
#include <vector>

class Master : public Actor {
    public:
        // Contructors
        Master(int id) : Actor(id){
        }
        Master(Actor const& a) : Actor(a){
        }

        void set_initial_vals(int tot_sq, int live_sq, int inf_sq){
            total_squirrels = tot_sq;
            live_squirrels = live_sq;
            inf_count = inf_sq;
        }
        void run();

    protected:
        int live_squirrels;
        int total_squirrels;
        int inf_count;

    private: 
        // Variables
        int num_grid_cells = 16; 

        // Functions
        void set_up_sim();
};

#endif
