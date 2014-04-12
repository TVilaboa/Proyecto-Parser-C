#include<stdio.h>
 #include<conio.h>
 #include<stdlib.h>


typedef struct
 {
         call Data;
        struct Node* next;
 }Node;

 int siz=0;
 Node* rear;
 Node* front;

//equals to pop, delete first value from queue and returns it
//active=2 means its not an actual Call, but in order to avoid a bug and don’t let the queue empty I
//need to simulate a call
 Node* delQueue()
{
       Node *temp, *var=rear;
      if(siz >1)
      {
             rear = rear->next;
             siz-=1;
             return var;
      }
      else if(siz== 1){
           call call;
         call.active=2;
            push(call);
            siz-=1;
            rear = rear->next;

             return var;
      } else {
      printf("\nQueue Empty");

      }

}

void push( call value)
{
     Node *temp;
     temp=( Node *)malloc(sizeof( Node));
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
                printf("un call");
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

