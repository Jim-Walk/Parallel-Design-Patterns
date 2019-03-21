#ifndef MASTER_HPP
#define MASTER_HPP
#include "../include/actor.hpp"
#include <vector>

class Master : public Actor {
    public:
        Master(int id) : Actor(id){
        }

        int create_actor(int worker_type);
        bool is_running();
        void set_up();
        void set_total_squirrels(int squirrels){
            total_squirrels = squirrels;
        }
        void create_sq(int id);
        void run_simulation();


    private: 
        const int num_grid_cells = 4; 
        int live_squirrels = 0;
        int total_squirrels;
        std::vector<int> squirrel_ids;
        std::vector<int> grid_ids;
        bool running;
};

#endif
