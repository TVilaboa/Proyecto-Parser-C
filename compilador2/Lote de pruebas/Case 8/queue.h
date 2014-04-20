 #include<stdio.h>
 #include<conio.h>
 #include<stdlib.h>
 #include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 8\plane.h"


 typedef struct
 {
         plane data;
        struct Node* next;
 }Node;

 Node *rear;
 Node *front;

int siz=0;

/**
Pops from queue the first element
*/
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

/**
Pushes the element on the back of the queue
*/
void push( plane value)
{
     Node *temp;
     temp=(Node *)malloc(sizeof( Node));
     temp->data=value;
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

/**
Prints how the queue is at the time
*/
void display()
{
     Node *var=rear;
     if(var!=NULL)
     {
           printf("\nElements are as: \n ");
           while(var!=NULL)
           {
                printf("one plane on line \n");
                var=var->next;
           }
     printf("\n");
     }
     else
     printf("\nQueue is Empty \n");
}

int sizeofqueue(){
    return siz;
}
