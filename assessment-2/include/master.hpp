#ifndef MASTER_HPP
#define MASTER_HPP
#include "../include/actor.hpp"
#include <vector>

class Master : public Actor {
    public:
        Master(int id) : Actor(id){
        }

        bool is_active();
        void set_up();
        void set_total_squirrels(int squirrels){
            total_squirrels = squirrels;
        }
        int create_sq(float pos_x,float pos_y);
        void run();


    private: 
        // Variables
        const int num_grid_cells = 16; 
        int live_squirrels = 4;
        int total_squirrels;
        int initial_inf_sq = 2;
        std::vector<int> squirrel_ids;
        std::vector<int> inf_squirrel_ids;
        std::vector<int> dead_squirrel_ids;
        std::vector<int> grid_ids;

        // Functions
        void manage_squirrels();
};

#endif
