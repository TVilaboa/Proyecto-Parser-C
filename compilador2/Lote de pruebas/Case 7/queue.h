#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include "car.h"


 struct Node
 {
        struct car data;
        struct Node* next;
 }*rear, *front;

int siz=0;

/**
Pops from queue the first element
*/
struct Node* delQueue()
{
       struct Node *temp, *var=rear;
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
void push(struct car value)
{
     struct Node *temp;
     temp=(struct Node *)malloc(sizeof(struct Node));
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
     struct Node *var=rear;
     if(var!=NULL)
     {
           printf("\nElements are as: \n ");
           while(var!=NULL)
           {
                printf("one car on line \n");
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
