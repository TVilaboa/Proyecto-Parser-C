#include <time.h>

struct truck{
    struct school schools[500];
    int hours;
};

//create a truck with 500 schools,each with random code between 1 and 9999 and initialize hours value
void createTruck(int hour){
    struct truck truck;
    int i=0;
            for(i;i<500;i++){
            int random_number = rand() % 9999;

                    struct school school;
                     createSchool(&school,random_number,1);
                     truck.schools[i]=school;

}

   truck.hours=hour;
    push(truck);
}
