#include <stdio.h>

#include <stdlib.h>



typedef struct

{

   void *data;

   struct List *next;

}List;

void insert(List **, void *, unsigned int);

void print(List *); //pointer to function still not supported on parser /*, void (*)(void *)*/

void printstr(void *);

void insert(List **p, void *data, unsigned int n)

{

  List *temp;

   int i;

   temp = malloc(sizeof(List));

   temp->data = malloc(n);

   for (i = 0; i < n; i++)

      *(char *)(temp->data + i) = *(char *)(data + i);

   temp->next = *p;

   *p = temp;

}



void print(List *p) //*, void (*f)(void *)

{

   while (p)

   {
        printf(" \"%s\"", p->data);
      //*(*f)(p->data);

      p = p->next;



   }

}


// void printstr(void *str)

//{

   //printf(" \"%s\"", (char *)str);

//}




