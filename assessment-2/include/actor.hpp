#ifndef ACTOR_HPP
#define ACTOR_HPP
#include <tuple>

class Actor{
    protected:
        int id;
        bool active;
        enum MSG {STOP=0, START=1, STEP=3};
        void *buf;
    private:
        int worker_type;
    public:
        Actor(int id);
        void start_up();
        virtual void run();
        int get_id();
        int get_type();
        void check_active();
        void send_msg(int dest, int msg);
        void send_data(int dest, float data);
        std::tuple<bool, int> msg_recv();
        bool data_recv(float *data);
        
};

#endif
