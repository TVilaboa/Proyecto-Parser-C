#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#define TAMSTACK 6
#define MAXSTACK TAMSTACK-1 //actualmente no toma en cuenta el -1
#define TAMQUEUE 6
#define MAXQUEUE TAMQUEUE-1





/*stack functions*/
int full(int,int []);
int empty(int,int []);
void push(int,int [], int);
void pop(int,int [],int *);


/*queue functions*/
void enqueue(int,int,int,int [],int, int);
void dequeue(int,int,int,int [],int,int *);
int isFull(int,int,int,int [],int);
int isEmpty(int,int,int,int [],int);


int main()
{
   int topp,topq;
   int itemp[TAMSTACK],itemq[TAMSTACK];
   int frontc,frontd,rearc,reard,lenghtc,lenghtd,cantc,cantd;
   int itemc [TAMSTACK],itemd [TAMSTACK];
   int data=0 ,opc=0 ,element=0 ,flag=0;
   topp=0;
   rearc=0;
    frontc=0;
    lenghtc=0;
    cantc=0;
   do
   {
      system("cls");//Clean the screen
      printf("\nGENERAL MENU");
      printf("\n1-> Insert an element in the stack");
      printf("\n2-> Delete an element in the stack");
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
		 if(!full(topp,itemp))
		 {
		 printf("\nInput the element to be inserted in the stack: ");
		 scanf("%d",&data);
		 push(topp,itemp,data);
		 printf("\nElement inserted...");
		 }
		 else
		 {
		 printf("\nERROR: Stack full");
		 }
		 break;

		 case 2:
		 if(!empty(topp,itemp))
		 {
		 pop(topp,itemp,&data);
		 printf("\nThe element deleted is %d",data);
		 }
		 else
		 {
		 printf("\nERROR: Empty stack");
		 }
		 break;

		case 3:
		 if(!empty(topp,itemp))
		 {
		 printf("Delete the element selected: ");
		 scanf("%d",&element);

		if(topp!= 1){
		 topq=0;
		 do
		 {
		 pop(topp,itemp,&data);
				 if (data != element)
				 {
				 push(topq,itemq,data);
				 }
		 }while(!empty(topp,itemp));

		 do
		 {
				 pop(topq,itemq,&data);
				 push(topp,itemp,data);
		 }while(!empty(topq,itemq));
		 }
		 else if(data == element){pop(topp,itemp,&data);}
			   else {printf("the element is not in the stack");}
		 }
		 else
		 {
		 printf("\nERROR: Stack empty");
		 }
		 break;


		 case 4:
		 if(!empty(topp,itemp))
		 {
		 topq=0;
		 do
		 {
				 pop(topp,itemp,&data);
				 printf("\n%d",data);
				 push(topq,itemq,data);
		 }while(!empty(topp,itemp));
		 do
		 {
				 pop(topq,itemq,&data);
				 push(topp,itemp,data);
		 }while(!empty(topq,itemq));
		 }
		 else
		 {
		 printf("\nERROR: Stack empty");
		 }
		 break;

		 case 5:
		 //insert an element in the queue
		 if(!isFull(frontc,rearc,lenghtc,itemc,cantc)) // the queue is full
		 {
		 printf("\nInput the element to be inserted in the queue: ");
		 scanf("%d",&data);
		 enqueue(frontc,rearc,lenghtc,itemc,cantc,data);
		 printf("\nElement inserted...");
		 }
		 else
		 {
		 printf("\nERROR: Queue is full");
		 }
		 break;

		 case 6:
		 if (!isEmpty(frontc,rearc,lenghtc,itemc,cantc))
		 {
			dequeue(frontc,rearc,lenghtc,itemc,cantc,&data);
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
int full(int top,int item[])
{
   return(top==MAXSTACK);
}

int empty(int top,int item[])
{
   return(top==0);
}

void push(int top,int item[],int dato)
{
   if(!full(top,item))
   {
      (top)++;
      item[top]=dato;  //elemento[1]=dato
   }
   else
      printf("\nOVERFLOW");
}



/*implementation of the queue functions*/

void enqueue(int front,int rear,int lenght,int item2[],int cant, int x)
{
	if (cant!=TAMQUEUE && rear<=(TAMQUEUE-1))
	{
		item2 [rear] = x;
		(rear)++;
		(cant)++;
	}
	else
	{
		if (rear==TAMQUEUE && cant<TAMQUEUE)
			{
			rear = 0;
			item2 [rear] = x;
			(rear)++;
			(cant)++;
			}
			else
			      printf("\nOVERFLOW");

	}
}


void dequeue(int front,int rear,int lenght,int item2[],int cant, int *x)
{
	(cant)--;
	if (front==(TAMQUEUE-1))
	{
		front=0;
		*x = item2[(TAMQUEUE-1)];
	}
	*x = item2[(front)++];
}


int isFull(int front,int rear,int lenght,int item2[],int cant)
{
	return(cant==MAXQUEUE);
}


int isEmpty(int front,int rear,int lenght,int item2[],int cant)
{
	return (front==rear);
}

void pop(int top,int item[],int *dato)
{
   if(!empty(top,item))
   {
      *dato=item[top];
      (top)--;
   }
   else
      printf("\nUNDERFLOW");
   }
