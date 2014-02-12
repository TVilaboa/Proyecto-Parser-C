#include<stdio.h>
#include<stdlib.h>//Para que funcione system("cls"), que genera que la pantalla se borre.

#define TAM 6
#define MAX TAM-1
#define TAM2 6
#define MAX2 TAM2-1

 
typedef struct
{
   int tope;
   int item[TAM];
}pila;


typedef struct
{
   int frente;
   int fondo;
   int tamanio;
   int item2[TAM2];
   int cant;
}cola;


/*metodos de pila*/
int full(pila *);
int empty(pila *);
void push(pila *, int);
void pop(pila *,int *);
 

/*metodos de cola*/
void encolar(cola *, int);
void desencolar(cola *,int *);
int estallena(cola *);
int vacia(cola *);

 
int main()
{
   pila p,t;
   cola c,d;
   int dato=0 ,opc=0 ,elemento=0 ,flag=0;
   p.tope=0;
   c.fondo=0, c.frente=0, c.tamanio=0, c.cant=0;
   do
   {
      system("cls");//Borrar pantalla
      printf("\nMENU-GENERAL");
      printf("\n1-> Insertar elemento en la pila");
      printf("\n2-> Eliminar elemento en la pila");
      printf("\n3-> Eliminar elemento x en la pila");
	  printf("\n4-> Visualizar la pila");
	  printf("\n5-> Insertar elemento en la cola");
	  printf("\n6-> Visualizar y eliminar elemento en la cola");
	  printf("\n7-> Visualizar la cola");
	  printf("\n8-> Salir");
	  printf("\n\nDe su opci�n : ");
	  scanf("%d",&opc);
	  switch(opc)
	  {
		 case 1:
		 if(!full(&p)) // si pila no esta llena
		 {
		 printf("\nDe el elemento a insertar: ");
		 scanf("%d",&dato);
		 push(&p,dato);
		 printf("\nElemento insertado...");
		 }
		 else
		 {
		 printf("\nERROR: Pila llena");
		 }
		 break;

		 case 2:
		 if(!empty(&p))
		 {
		 pop(&p,&dato);
		 printf("\nEl elemento eliminado es %d",dato);
		 }
		 else
		 {
		 printf("\nERROR: Pila vac�a");
		 }
		 break;

		case 3:
		 if(!empty(&p))
		 {
		 printf("eliminar elemento seleccionado: ");
		 scanf("%d",&elemento);

		if(p.tope != 1){
		 t.tope=0;
		 do
		 {
		 pop(&p,&dato);
				 if (dato != elemento)
				 {
				 push(&t,dato);
				 }
		 }while(!empty(&p));

		 do
		 {
				 pop(&t,&dato);
				 push(&p,dato);
		 }while(!empty(&t));
		 }
		 else if(dato == elemento){pop(&p,&dato);}
			   else {printf("el elemento no se encuentra en la pila");}
		 }
		 else
		 {
		 printf("\nERROR: Pila vac�a");
		 }
		 break;


		 case 4:
		 if(!empty(&p))
		 {
		 t.tope=0;
		 do
		 {
				 pop(&p,&dato);
				 printf("\n%d",dato);
				 push(&t,dato);
		 }while(!empty(&p));
		 do
		 {
				 pop(&t,&dato);
				 push(&p,dato);
		 }while(!empty(&t));
		 }
		 else
		 {
		 printf("\nERROR: Pila vac�a");
		 }
		 break;

		 case 5:
		 //insertar elemento en la cola
		 if(!estallena(&c)) // si la cola no esta llena
		 {
		 printf("\nDe el elemento a insertar: ");
		 scanf("%d",&dato);
		 encolar(&c,dato);
		 printf("\nElemento insertado...");
		 }
		 else
		 {
		 printf("\nERROR: Cola llena");
		 }
		 break;

		 case 6:
		 if (!vacia(&c))
		 {
			desencolar(&c,&dato);
			printf("\n%d",dato);
		 }
		 else
		 {
			printf("\nERROR: Cola vacia");
		 }
		 break;


		 //visualizar y eliminar elemento de la cosa
		 break;

		 case 7:
		 //visualizar la cola
		 break;

		 case 8:
		 flag=1;
		 break;
 
         default:
         printf("\nOpci�n no v lida...");
      }
      if(!flag)
      {
         printf("\n\nPresione una tecla para continuar...");
         getch();
      }
   }while(!flag);
 
   return 0;
}


/*implementation of the stack methods*/
int full(pila *p)
{
   return(p.tope==MAX);
}
 
int empty(pila *p)
{
   return(p.tope==0);
}
 
void push(pila *p,int dato)
{
   if(!full(p))
   {
      (p.tope)++;
      p.item[p.tope]=dato;  //elemento[1]=dato
   }
   else
      printf("\nOVERFLOW");
}
 
void pop(pila *p,int *dato)
{
   if(!empty(p))
   {
      *dato=p.item[p.tope];
      (p.tope)--;
   }
   else
      printf("\nUNDERFLOW");
   }

/*implementation of the queue methods*/
void encolar(cola *c, int x)
{
	if (c.cant!=TAM2 && c.fondo<=(TAM2-1))
	{
		c.item2 [c.fondo] = x;
		(c.fondo)++;
		(c.cant)++;
	}	
	else
	{
		if (c.fondo==TAM2 && c->cant<TAM2)
			{
			c.fondo = 0;
			c.item2 [c.fondo] = x;
			(c.fondo)++;
			(c.cant)++;
			}		
			else
			      printf("\nOVERFLOW");
				
	}
}


void desencolar(cola *c, int *x)
{
	(c.cant)--;
	if (c.frente==(TAM2-1))
	{
		c.frente=0;
		*x = c.item2[(TAM2-1)];
	}
	*x = c.item2[(c.frente)++];
}


int estallena(cola *c)
{
	return(c.cant==MAX2);
}


int vacia(cola *c)
{
	return (c.frente==c.fondo);
}