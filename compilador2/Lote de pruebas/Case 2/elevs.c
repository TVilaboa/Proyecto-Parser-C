#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 2\elevator.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 2\call.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 2\treeElevators.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 2\queuecalls.h"

int cantElevators;
//Elevators calls are stored and processed when 5 secs are simulated
int main()
{
    node * root;
    root =NULL;
    printf("Welcome to Elevators 2.0\n");
    printf("Please enter the numbers of elevators to simulate.\n");
    scanf("%d",&cantElevators);
    elevator elevators[cantElevators];
    int i=0;
    for(i;i<cantElevators;i++){
             elevator elevator;
                elevator.floor=0;
    elevator.busy=0;
            elevators[i]=elevator;
    }
    i=0;
    for(i;i<25;i++){
             call call;
            call.floor=i;
        call.active=0;
        call.delay=0;
        insert(&root,call);
    }
    printf("Simulation begins.\n");
    int menu;
    int cantCalls=0;
    do{printf("\n\n1- See elevator floor \n2- Call a elevator \n3- Simulate 5 seconds \n4- End simulation \n\n");
    scanf("%d",&menu);
    switch(menu){
    case 1: seeElevator(elevators); break;
    case 2: storeCall(cantCalls); cantCalls=cantCalls+1; break;
    case 3: simulate(root,elevators);break;}
    }
     while(menu<4);
     print_inorder(root);
    return 0;
}


void storeCall(int number){
         call call;
        call.number=number;
        push(call);
}
void simulate(node * tree, elevator elevators[]){
    int i=0;
     for(i;i<cantElevators;i++){
        if(elevators[i].floor ==0 && elevators[i].busy==0 ){
            call call;
          Node *nextCall=delQueue();
           call=nextCall->Data;
           if(call.active != 2){
                callElevator(elevators[i],tree,call,i);
           }


        }
    }

    traverseTree(tree,elevators);
}

void traverseTree(node * tree, elevator elevators[])
{
    if (tree)
    {
        traverseTree(tree->left,elevators);
        if(tree->data.active ==1){
                if(tree->data.floor != elevators[tree->data.elevatorCalled].floor && tree->data.goingToDest ==0){
            tree->data.delay=tree->data.delay + 1;
            elevators[tree->data.elevatorCalled].floor=elevators[tree->data.elevatorCalled].floor + 1;
                    } else {
                        tree->data.goingToDest=1;
                        tree->data.delay=tree->data.delay + 1;
                    }
                    if(tree->data.floorDest < elevators[tree->data.elevatorCalled].floor){
                       elevators[tree->data.elevatorCalled].floor=elevators[tree->data.elevatorCalled].floor - 1;
                    } else if(tree->data.floorDest > elevators[tree->data.elevatorCalled].floor){
                    elevators[tree->data.elevatorCalled].floor=elevators[tree->data.elevatorCalled].floor + 1;
                    } else {
                       tree->data.active=0;
                       elevators[tree->data.elevatorCalled].floor=0;
                       elevators[tree->data.elevatorCalled].busy=0;
                    }
        }
        traverseTree(tree->right,elevators);
    }
}

void seeElevator( elevator elevators[]){
   printf("Please enter the number of elevator to see.\n");
    int  elev;
    scanf("%d",&elev);
    printf("Elevator %d is in floor %d",elev,elevators[elev].floor);
}

void callElevator(elevator elevator,node * root, call call,int i){
    printf("\nCall number %d \n",call.number);
    printf("Please enter the floor the elevator is being called from.\n");
    int  floor;
    scanf("%d",&floor);
    printf("Elevator %d is assigned.",i);
    printf("Please enter the floor the elevator has to go.\n");
    int  floorDest;
    scanf("%d",&floorDest);
    elevator.busy=1;
    createCall(&call,floor,i,floorDest,call.number);
     editCall(root,call);

}

void editCall(node * root, call call){
    node *tmp;
    tmp = search(&root, call);
    if (tmp)
    {

        tmp->data.elevatorCalled=call.elevatorCalled;
        tmp->data.active=1;
        tmp->data.floorDest=call.floorDest;
    }
    else
    {
        printf("Data Not found in tree.\n");
    }

}


