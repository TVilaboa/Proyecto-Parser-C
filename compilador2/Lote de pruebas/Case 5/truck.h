#include <time.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 5\school.h"

typedef struct {
     school schools[500];
    int hours;
}truck;

//create a truck with 500 schools,each with random code between 1 and 9999 and initialize hours value
void createTruck(int hour){
     truck truck;
    int i=0;
            for(i;i<500;i++){
            int random_number = rand() % 9999;

                     school school;
                     createSchool(&school,random_number,1);
                     truck.schools[i]=school;

}

   truck.hours=hour;
    push(truck);
}
