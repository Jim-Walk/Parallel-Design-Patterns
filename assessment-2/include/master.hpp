#ifndef MASTER_HPP
#define MASTER_HPP
#include "../include/actor.hpp"
#include <vector>

class Master : public Actor {
    public:
        Master(int id) : Actor(id){
        }

        bool is_active();
        void set_up_sim();
        int create_sq(float pos_x,float pos_y);
        void run();


    private: 
        // Variables
        const int num_grid_cells = 16; 
        int live_squirrels = 15;
        const int total_squirrels = 200;
        int initial_inf_sq = 9;

        // Functions
        void manage_squirrels();
};

#endif
