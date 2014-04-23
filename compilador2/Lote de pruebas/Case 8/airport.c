#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 8\queue.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 8\flight.h"

#define SIZE    100


double get_average(double time,int counter);

int main()
{
    FLIGHT information[SIZE];
    int counter = 0; //counter to calculate actual waiting time

    double average;
    double time = 0;

    int counter_A = 0;
    int counter_B = 0;
    int counter_C = 0;

    front = NULL;
    int menu = 0;

    do{

        addplane();
        if(sizeofqueue() > 5){ // 5 planes must be over flying at ALL times

            double time_elapsed; // time for every flying between request and actual
                                    // landing

            plane flight;
             Node *node = delQueue();
            flight = node->data;
            set_landing_time(&flight);

            information[counter].company = flight.company;
            information[counter].number = flight.number;
            information[counter].arrival = flight.landed;

            printf("\n Company %c \n", flight.company);
            printf("\n Plane %02d \n",flight.number);

            if(flight.company == 'A'){
                counter_A ++;
            }if(flight.company == 'B'){
                counter_B ++;
            }if(flight.company == 'C'){
                counter_C ++;
            };

            printf("\n FLIGHT \n");
            printf("%s", asctime(localtime(&flight.request)));
            printf("%s", asctime(localtime(&flight.landed)));
            printf("\n");

            time_elapsed = difftime(flight.landed,flight.request);
            printf("\n Time elapsed between request and Landing: %f \n", time_elapsed);

            time += time_elapsed;

            addplane();
            counter ++;
       }

        printf(" \n 1) Set a landing and receive a request \n");
        printf("\n 2) Print Airport Information \n");
        scanf("%d",&menu);

    }while(menu == 1);

    BubbleSort_company(information,counter);
    BubbleSort_number(information,0,counter_A);
    BubbleSort_number(information,counter_A,(counter - counter_C));
    BubbleSort_number(information,(counter - counter_C) , counter );
    average = get_average(time,counter);

    printf("\n FLIGHTS INFORMATION \n");
    print_information(information,counter);
    printf("\n \n");
    printf("\n Average waiting time for landing is: %f  seconds \n", average);

    return 0;
}

/**
Adds a plane to the simulation
*/
void addplane(){
    create_plane();

    return 0;
}

double get_average(double time, int counter){
    double average = time / counter;
    return average;
}
