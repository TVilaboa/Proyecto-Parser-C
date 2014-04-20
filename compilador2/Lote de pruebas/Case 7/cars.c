#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 7\queue.h"


int main()
{
    int counter = 0;
    double diff_time;
    double average;

    front = NULL;
    int menu = 0;

    do{
        addCar();
        if(sizeofqueue() > 20){ // it needs an assembly line with a minimum of 20 cars
                                //to start poping cars

            double time_elapsed;

            car assembled;
          Node *node = delQueue();
            assembled = node->data;
            set_completion_time(&assembled);

            printf("\n TIME FOR ASSEMBLY \n");
            printf("%s", asctime(localtime(&assembled.start)));
            printf("%s", asctime(localtime(&assembled.completion)));
            printf("\n");

            time_elapsed = difftime(assembled.completion,assembled.start);
            printf("\n Time Elapsed: %f \n", time_elapsed);

            diff_time += time_elapsed;

            addCar();
            counter ++; //it represents the number of cars which have successfully been assembled
       }

        printf(" \nEnter 1 to continue the simulation and enter 2 to finish and print average time for assembly.\n");
        scanf("%d",&menu);

    }while(menu == 1);



    average = diff_time / counter;
    printf("\n Average assembly time for one car: %f seconds\n", average);

    return 0;
}

/**
Adds a car to the simulation
*/
void addCar(){
    create_car();

    return 0;
}
