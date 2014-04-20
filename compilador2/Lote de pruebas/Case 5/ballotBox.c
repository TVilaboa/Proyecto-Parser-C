#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 5\queue.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 5\Tree.h"





int counter=0;

int main()
{
    front=NULL;
    node *root;
    root=NULL;
    int menu=0;
    addTruck(); //lets suposse there is already 1 truck waiting
    do{
    counter+=1;
    addTruck();
    if(counter %3==0){
    process(&root);
    }
    printf("Enter 1 to continue the simulation and enter 2 to finish and print the report.\n");
    scanf("%d",&menu);
    } while (menu==1);
    report(root);


	return 0;
}


//add truck to queue with 60% prob
void addTruck(){
    if(rand()%10 <=6){
        createTruck(counter);

    }

    }


//process the first truck stored in queue and adds the schools from that truck to a tree in order to print them ordenated
void process(node **root){
    truck truck;
    Node *node=delQueue();
    truck=node->Data;
    printf("Truck %d waited %d hours\n",counter/3,counter - truck.hours);
    int i=0;
    if(sizeofqueue()>0){
    for(i;i<500;i++){


     school school=truck.schools[i];

         Node *tmp;
    tmp = search(root, school);
    if (tmp)
    {
        createSchool(&((*root)->data),(*root)->data.code,(*root)->data.cantboxes+1);

    }
    else
    {
        insert(root,school);

    }
    }
}
}

//print the tree inorden= ordenated report
void report(node *root){
    print_inorder(root);
}
