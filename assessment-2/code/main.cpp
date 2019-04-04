#include <mpi.h>
#include <iostream>
#include <string>
#include "./include/actor.hpp"
#include "./include/master.hpp"
#include "./include/grid_cell.hpp"
#include "./include/squirrel.hpp"
#include "./include/clock.hpp"
#include "./include/controller.hpp"
#include "./lib/pool.h"

static void worker_code(int tot_sq, int live_sq, int inf_sq, int months);

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int statusCode = processPoolInit();
    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int tot_sq, live_sq, inf_sq, months;
    // Set default settings, or get them from the command line
    if (argc != 5){
        tot_sq = 200;
        live_sq = 15;
        inf_sq = 5;
        months = 24;
        if (myRank == 0)
            printf("Set default params with mpirun -nN ./bin/main $total_squirrels $live_squirrels, $infected_squirrels, $number_of_months\n");
    } else {
        tot_sq = atoi(argv[1]);
        live_sq = atoi(argv[2]);
        inf_sq = atoi(argv[3]);
        months = atoi(argv[4]);
    }

    if (statusCode == 1){
        worker_code(tot_sq, live_sq, inf_sq, months);
    } else if (statusCode == 2){
        Master master = Master(myRank);
        master.set_initial_vals(tot_sq, live_sq, inf_sq); 
        master.run();
        printf("master done \n");
    }

    processPoolFinalise();
    MPI_Finalize();
    return 0;
}


static void worker_code(int tot_sq, int live_sq, int inf_sq, int months){
    int workerStatus = 1;
    while (workerStatus) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        Actor a = Actor(rank);
        // Recv my type from master
        a.start_up();
        if (a.get_type() == Actor::actor_type::GRID){
           Grid_cell gc = Grid_cell(a); 
           gc.run();
        } else if (a.get_type() == Actor::actor_type::SQ || a.get_type() == Actor::actor_type::INFSQ){
            Squirrel sq = Squirrel(a);
            sq.run();
        } else if (a.get_type() == Actor::actor_type::CLOCK){
            Clock c = Clock(a);
            c.set_months(months);
            c.run();
        } else if(a.get_type() == Actor::actor_type::CONTROL){
            Controller control = Controller(a);
            control.set_initial_vals(tot_sq, live_sq, inf_sq);
            control.run(); 
        }
        // If I'm a dead squirrel, I'll sleep until I'm needed
        workerStatus = workerSleep();
    }
}
