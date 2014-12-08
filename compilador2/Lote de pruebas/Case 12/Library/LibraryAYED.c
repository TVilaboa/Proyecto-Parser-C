#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Toto\Dropbox\test cases\3 test case\Library\books.h"
#include "C:\Users\Toto\Dropbox\test cases\3 test case\Library\List.h"



List mylist;

void addBook(){
     book_structure book;
    createBook(&book);
    insertback(&mylist,&book);
    FILE *f1;
   f1 = fopen ("c:\\books.txt", "wt");
         fprintf (f1,"%s\n",book.name);
         fprintf(f1,"%s\n",book.author.name);
         fprintf(f1,"%s\n",book.author.lastname);
         fprintf(f1,"%s\n",book.category);
         fprintf(f1,"%d\n\n",book.code);
   fclose (f1);
    printf("\nEl nombre del libro es %s \n",book.name);
    printf("El codigo del libro es %d \n",book.code);
    printf("El nombre del autor es %s \n",book.author.name);
    printf("El apellido del autor es %s \n",book.author.lastname);
    printf("La categoria del libro es %s \n ", book.category);
    }



void deletebook(){
     book_structure book;
    createBook(&book);
    int contador=0;
    for(contador;contador<length(mylist);contador++){
             book_structure * book2;
            void * ptr= getitem(mylist,contador);
            book2=( book_structure *)ptr;

      if(book2->code == book.code){
        break;
      }
    }
    setitem(&mylist,contador,NULL);
}

void editbook(){
     book_structure book;
    createBook(&book);
    printf("Ingrese los datos nuevos");
     book_structure book2;
    createBook(&book2);
    int contador=0;
    for(contador;contador<length(mylist);contador++){
       book_structure * book2;
            void * ptr= getitem(mylist,contador);
            book2=( book_structure *)ptr;

      if(book2->code == book.code){
        break;
      }
    }
    setitem(&mylist,contador,(void *)&book2);
}

void prestarbook(){
     book_structure *book;
    createBook(&book);
    int contador=0;
    for(contador;contador<length(mylist);contador++){
       book_structure * book2;
            void * ptr= getitem(mylist,contador);
            book2=( book_structure *)ptr;

      if(book2->code == book->code){
        break;
      }
    }
    book=getitem(mylist,contador);
     book->taken=1;
    loan_structure loan;
    loan.book=book;
    printf("\nIngrese el nombre del estudiante sin espacios \n");
    scanf("%s",loan.student);
    FILE *f1;
   f1 = fopen ("c:\\historialloan.txt", "wt");
         fprintf (f1, "%s\n",loan.book->name);
         fprintf(f1,"%s\n\n",loan.student);
   fclose (f1);
}

void devolverbook(){
     book_structure *book;
    createBook(&book);
     loan_structure loan;
    loan.book=book;
    printf("\nIngrese el nombre del estudiante sin espacios \n");
    scanf("%s",loan.student);
    int contador=0;
    for(contador;contador<length(mylist);contador++){
      book_structure * book2;
            void * ptr= getitem(mylist,contador);
            book2=( book_structure *)ptr;

      if(book2->code == book->code){
        break;
      }
    }
    book=getitem(mylist,contador);
     book->taken=0;

}


void historialloan(){
    printf("\n   Historial prestamos     \n\n");
     FILE *f1;
   char line[40];
   f1 = fopen ("c:\\historialloan.txt", "r");
   while(feof(f1)==0){
         fscanf(f1, "%s\n", line);
         printf("%s\n",line);
   }
   fclose (f1);
}

void Listbooks(){
    printf("\n   Lista de libros   \n\n");
    FILE *f1;
   char line[40];
   f1 = fopen ("c:\\books.txt", "r");
   while(feof(f1)==0){
         fscanf(f1, "%s\n", line);
         printf("%s\n",line);
   }
   fclose (f1);
}


void loadListbooks(){
     printf("\n  Cargando Lista de libros   \n\n");
    FILE *f1;
     book_structure book;
    f1 = fopen ("c:\\books.txt", "r");
    if(f1!=NULL){
    while(feof(f1)==0){
         fscanf(f1, "%s\n", book.name);
         fscanf(f1, "%s\n", book.author.name);
         fscanf(f1, "%s\n", book.author.lastname);
         fscanf(f1, "%s\n", book.category);
         fscanf(f1, "%d\n\n", &book.code);
         insertback(&mylist,(void *) &book);
   }}
   fclose (f1);
}


void Listloan(){
     book_structure *book;
    printf(" Lista de prestamos \n\n");
    int contador=0;
    for(contador;contador<length(mylist);contador++){
             book_structure * book2;
            void * ptr= getitem(mylist,contador);
            book2=( book_structure *)ptr;


      if(book2->taken == 1){
         book=getitem(mylist,contador);
         printf("\nEl nombre del libro es %s \n",book->name);
    printf("El codigo del libro es %d \n",book->code);
    printf("El nombre del autor es %s \n",book->author.name);
    printf("El apellido del autor es %s \n",book->author.lastname);
    printf("La categoria del libro es %s \n\n ", book->category);
      }
    }

}
int main()
{
    initlist(&mylist);
    int menu;
    loadListbooks();
    do{printf("\n\n1- Agregar libro \n2- Borrar libro \n3- Editar libro \n4- Devolver libro \n5- Prestar libro\n6- Historial prestamos\n7- Lista libros\n8- Lista prestamos\n\n");
    scanf("%d",&menu);
    switch(menu){
    case 1: addBook(); break;
    case 2: deletebook(); break;
    case 3: editbook();break;
    case 4: devolverbook();break;
    case 5: prestarbook();break;
    case 6: historialloan(); break;
    case 7: Listbooks(); break;
    case 8: Listloan();break;
    }}
     while(menu<9);


    return 0;
}


