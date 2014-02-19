#include<stdio.h>
 #include<conio.h>
 #include<stdlib.h>

 struct Node
 {
        struct call Data;
        struct Node* next;
 }*rear, *front;

 int siz=0;

//equals to pop, delete first value from queue and returns it
//active=2 means its not an actual Call, but in order to avoid a bug and don’t let the queue empty I
//need to simulate a call
struct Node* delQueue()
{
       struct Node *temp, *var=rear;
      if(siz >1)
      {
             rear = rear->next;
             siz-=1;
             return var;
      }
      else if(siz== 1){
          struct call call;
         call.active=2;
            push(call);
            siz-=1;
            rear = rear->next;

             return var;
      } else {
      printf("\nQueue Empty");

      }

}

void push(struct call value)
{
     struct Node *temp;
     temp=(struct Node *)malloc(sizeof(struct Node));
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
     struct Node *var=rear;
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

