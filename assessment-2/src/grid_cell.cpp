#include "../include/grid_cell.hpp"

void Grid_cell::run(){
    while (active){
        if (month_update){
            avg_pop = pop_count;
            pop_count = 0;
            avg_inf = inf_count/avg_pop;
            inf_count = 0;
            month_update = false;
        }
        check_for_month();
        exchange_pop();
        exchange_inf();
    }
}
