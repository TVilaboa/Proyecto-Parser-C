#include <time.h>

typedef struct
{
    time_t start;
    time_t completion;
}car;

/**
Initialises a car and it pushes into a queue that represents the assembly line
*/
void create_car(){
    car car;
    time(&car.start);

    push(car);
}

/**
Set the completion time for a car when it is poped from the assembly line
*/
void set_completion_time( car *car ){
    car->completion = time(&car->completion);
}
