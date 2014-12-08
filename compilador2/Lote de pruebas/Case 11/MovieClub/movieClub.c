#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "C:\Users\Toto\Dropbox\test cases\3 test case\MovieClub\list.h"

typedef struct
{
    char* name;
   int* cashierInCome;
}Cashier;

typedef struct
{
   char* name;
}Movie;

int showMenu(){
    int option = 0;
          printf("\a");
    printf("\t  ingrese opcion:\n");
    printf("\t1 para ingresar nombre de pelicula (4 caracters)\n");
    printf("\t2 para vender una pelicula\n");
    printf("\t3 para ver la recaudacion total\n");
    printf("\t4 para guardar la recaudacion del dia de hoy\n");
    printf("\t5 para obtener la recaudacion del dia de ayer\n");
    printf("\t6 para ver lista de peliculas\n");
    printf("\t0 para salir\n");
    printf("\t  opcion = ");
    scanf("%d",&option);
    printf("\a");
    return option;
}

Cashier insertCashier(){
  Cashier cashier;
    printf("\tPara poder continuar necesita ingresar un cajero:\n");
    printf("\tingrese nombre: ");
    scanf("%s",&cashier.name);
    return cashier;
}

Movie insertMovie(){
  Movie movie;
    printf("\t  ingrese nombre de pelicula: ");
    scanf("%s",&movie.name);
    return movie;
}

int main()
{
    Cashier Cashier = insertCashier();
    List *lista;
    //system("cls");
    printf("\tOperando con el cajero: %s\n\n",&Cashier.name);
    Cashier.cashierInCome = 0;
    int n = Cashier.cashierInCome;
    int opcion = showMenu();
    while(opcion != 0){
    if(opcion == 1){
        Movie Movie = insertMovie();
        insert(&lista, &Movie.name, sizeof(Movie.name));
         printf("\t  Se ha insertado la pelicula: %s\n",&Movie.name);
    }else if(opcion == 2){
        Cashier.cashierInCome++;
        printf("\t  Se ha vendido una pelicula\n");
    }else if(opcion == 3){
         printf("\t  La recaudacion total fue de %d\n",Cashier.cashierInCome);
    }else if(opcion == 4){
   int n = Cashier.cashierInCome;

   FILE *f1;
   f1 = fopen ("c:\\recaudacion.txt", "wt");
         fprintf (f1, "%d\n", n);
         fwrite(n,100,100,f1);//testeo
   fclose (f1);
   printf("\t  Se ha almacenado la recaudacion en c:\\recaudacion.txt\n");
    }


else if(opcion == 5){
   FILE *f1;
   int recaudacion = 0;
   f1 = fopen ("c:\\recaudacion.txt", "r");
         fscanf(f1, "%d\n", &recaudacion);

   fclose (f1);
      printf("\t  La recaudacion de ayer fue de %d\n",recaudacion);
}else if(opcion == 6){
    printf("\t  las peliculas ingresadas son:");
 print(lista/*, printstr*/);
}    printf("\t  ");
    system("pause");
      system("cls");
    printf("\tOperando con el cajero: %s\n\n",&Cashier.name);
      opcion = showMenu();
    }



    return 0;
}
