#include <time.h>

typedef struct
{
    time_t request;
    char company;
    int number;
    time_t landed;
}plane;

char randomize_company();
int set_number();

char randomize_company(){
    int random = (rand() % 3) + 1;
    //printf("\n random number %02d\n", random);
    char company;
    if (random == 1){
        company = 'A';
    } if(random == 2 ){
        company = 'B';
    } if (random == 3){
        company = 'C';
    };

    return company;
}

int set_number(){
    int random = (rand() % 998) + 1;
    return random;
}


/**
Initialises a plane and it pushes into an overflight circuit of 5
*/
void create_plane(){
    plane plane;
    time(&plane.request);
    plane.company = randomize_company();
    plane.number = set_number();

    push(plane);
}

/**
Set the landing time for a plane*/
void set_landing_time( plane *plane ){
    plane->landed = time(&plane->landed);
}
