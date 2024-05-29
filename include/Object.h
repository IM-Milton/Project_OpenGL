class Object
{
    public:
        Object();
        int object_id = 0;
        virtual void update(double delta_t)=0;
        virtual void render();


};


class Brique: Object
{
    int object_id = 1;
    void update(double delta_t);
    void render();


};


