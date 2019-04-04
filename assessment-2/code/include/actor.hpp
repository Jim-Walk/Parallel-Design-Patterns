#ifndef ACTOR_HPP
#define ACTOR_HPP
#include <tuple>

class Actor{
    public:
        enum actor_type { 
            GRID=0, 
            SQ=1, 
            INFSQ=2,
            DEADSQ=3,
            CLOCK=4,
            CONTROL=5
        };
        Actor(int id);
        void start_up();
        virtual void run();
        int get_id();
        actor_type get_type();
        void check_active();
        void send_msg(int dest, int msg);
        void send_data(int dest, float data);
        std::tuple<bool, int, int> msg_recv();
        bool data_recv(int src, float *data);
        
    protected:
        int id;
        bool active;
        enum MSG {STOP=0, START=1, STEP=3, INFSTEP=4, TICK=5};
        void *buf;
        actor_type act_type;
        int parent;
};

#endif
