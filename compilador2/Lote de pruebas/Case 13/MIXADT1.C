#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#define TAM 6
#define MAX TAM-1
#define TAM2 6
#define MAX2 TAM2-1


typedef struct
{
   int top;
   int item[TAM];
}stack;


typedef struct
{
   int front;
   int rear;
   int lenght;
   int item2[TAM2];
   int cant;
}queue;


/*stack functions*/
int full(stack *);
int empty(stack *);
void push(stack *, int);
void pop(stack *,int *);


/*queue functions*/
void enqueue(queue *, int);
void dequeue(queue *,int *);
int isFull(queue *);
int isEmpty(queue *);


int main()
{
   stack p,t;
   queue c,d;
   int data=0 ,opc=0 ,element=0 ,flag=0;
   p.top=0;
   c.rear=0, c.front=0, c.lenght=0, c.cant=0;
   do
   {
      system("cls");//Clean the screen
      printf("\nGENERAL MENU");
      printf("\n1-> Insert an element in the stack");
      printf("\n2-> Delecte an element in the stack");
      printf("\n3-> Delete the element x in the stack");
	  printf("\n4-> List the elements of the stack");
	  printf("\n5-> Insert an elements in the queue");
	  printf("\n6-> Print and delete an element in the queue");
	  printf("\n7-> List the queue");
	  printf("\n8-> Quit");
	  printf("\n\nOption: ");
	  scanf("%d",&opc);
	  switch(opc)
	  {
		 case 1:
		 if(!full(&p))
		 {
		 printf("\nInput the element to be inserted in the stack: ");
		 scanf("%d",&data);
		 push(&p,data);
		 printf("\nElement inserted...");
		 }
		 else
		 {
		 printf("\nERROR: Stack full");
		 }
		 break;

		 case 2:
		 if(!empty(&p))
		 {
		 pop(&p,&data);
		 printf("\nThe element deleted is %d",data);
		 }
		 else
		 {
		 printf("\nERROR: Empty stack");
		 }
		 break;

		case 3:
		 if(!empty(&p))
		 {
		 printf("Delete the element selected: ");
		 scanf("%d",&element);

		if(p.top!= 1){
		 t.top=0;
		 do
		 {
		 pop(&p,&data);
				 if (data != element)
				 {
				 push(&t,data);
				 }
		 }while(!empty(&p));

		 do
		 {
				 pop(&t,&data);
				 push(&p,data);
		 }while(!empty(&t));
		 }
		 else if(data == element){pop(&p,&data);}
			   else {printf("the element is not in the stack");}
		 }
		 else
		 {
		 printf("\nERROR: Stack empty");
		 }
		 break;


		 case 4:
		 if(!empty(&p))
		 {
		 t.top=0;
		 do
		 {
				 pop(&p,&data);
				 printf("\n%d",data);
				 push(&t,data);
		 }while(!empty(&p));
		 do
		 {
				 pop(&t,&data);
				 push(&p,data);
		 }while(!empty(&t));
		 }
		 else
		 {
		 printf("\nERROR: Stack empty");
		 }
		 break;

		 case 5:
		 //insert an element in the queue
		 if(!isFull(&c)) // the queue is full
		 {
		 printf("\nInput the element to be inserted in the queue: ");
		 scanf("%d",&data);
		 enqueue(&c,data);
		 printf("\nElement inserted...");
		 }
		 else
		 {
		 printf("\nERROR: Queue is full");
		 }
		 break;

		 case 6:
		 if (!isEmpty(&c))
		 {
			dequeue(&c,&data);
			printf("\n%d",data);
		 }
		 else
		 {
			printf("\nERROR: Queue empty");
		 }
		 break;


		 //print and delete the element in the queue
		 break;

		 case 7:
		 //list the queue
		 break;

		 case 8:
		 flag=1;
		 break;

         default:
         printf("\nInvalid option...");
      }
      if(!flag)
      {
         printf("\n\nPress a key to continue...");
            getch();
      }
   }while(!flag);

   return 0;
}


/*implementation of the stack functions*/
int full(stack *p)
{
   return(p->top==MAX);
}

int empty(stack *p)
{
   return(p->top==0);
}

void push(stack *p,int dato)
{
   if(!full(p))
   {
      (p->top)++;
      p->item[p->top]=dato;  //elemento[1]=dato
   }
   else
      printf("\nOVERFLOW");
}

void pop(stack *p,int *dato)
{
   if(!empty(p))
   {
      *dato=p->item[p->top];
      (p->top)--;
   }
   else
      printf("\nUNDERFLOW");
   }


/*implementation of the queue functions*/

void enqueue(queue *c, int x)
{
	if (c->cant!=TAM2 && c->rear<=(TAM2-1))
	{
		c->item2 [c->rear] = x;
		(c->rear)++;
		(c->cant)++;
	}
	else
	{
		if (c->rear==TAM2 && c->cant<TAM2)
			{
			c->rear = 0;
			c->item2 [c->rear] = x;
			(c->rear)++;
			(c->cant)++;
			}
			else
			      printf("\nOVERFLOW");

	}
}


void dequeue(queue *c, int *x)
{
	(c->cant)--;
	if (c->front==(TAM2-1))
	{
		c->front=0;
		*x = c->item2[(TAM2-1)];
	}
	*x = c->item2[(c->front)++];
}


int isFull(queue *c)
{
	return(c->cant==MAX2);
}


int isEmpty(queue *c)
{
	return (c->front==c->rear);
}
