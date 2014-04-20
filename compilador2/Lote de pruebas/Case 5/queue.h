#include<stdio.h>
 #include<conio.h>
 #include<stdlib.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 5\truck.h"

 typedef struct
 {
         truck Data;
        struct Node* next;
 }Node;

int siz=0;
 Node* rear;
 Node* front;

//equals to pop, delete first value from queue and returns it
 Node* delQueue()
{
        Node *temp, *var=rear;
      if(var!=NULL)
      {
             rear = rear->next;
             siz-=1;
             return var;
      }
      else
      printf("\nQueue Empty");
}

void push( truck value)
{
      Node *temp;
     temp=(Node *)malloc(sizeof( Node));
     temp->Data=value;
     if (front == NULL)
     {
           front=temp;
           front->next=NULL;
           rear=front;
     }
     else
     {
           front->next=temp;
           front=temp;
           front->next=NULL;
     }
     siz+=1;
}

void display()
{
     Node *var=rear;
     if(var!=NULL)
     {
           printf("\nElements are as:  ");
           while(var!=NULL)
           {
                printf("un truck");
                var=var->next;
           }
     printf("\n");
     }
     else
     printf("\nQueue is Empty");
}

int sizeofqueue(){
    return siz;
}
