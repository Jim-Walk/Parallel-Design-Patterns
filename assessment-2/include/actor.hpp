#ifndef ACTOR_HPP
#define ACTOR_HPP

class Actor{
    protected:
        int id;
        bool active;
    private:
        int worker_type;
    public:
        Actor(int id);
        void start_up();
        virtual void run();
        int get_id();
        int get_type();
        void send(int dest, int msg);
        int recv(int src);
        
};

#endif
