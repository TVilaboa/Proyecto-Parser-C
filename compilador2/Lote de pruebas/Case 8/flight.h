#include <time.h>

typedef struct
{
    char company;
    int number;
    time_t arrival;
}FLIGHT;

/**
Sorts by company
*/
void BubbleSort_company(FLIGHT information[], int array_size)
{
     int i, j;
     FLIGHT temp;
     for (i = 0; i < (array_size - 1); ++i)
     {
          for (j = 0; j < array_size - 1 - i; ++j )
          {
               if (information[j].company > information[j+1].company)
               {
                    temp = information[j+1];
                    information[j+1] = information[j];
                    information[j] = temp;
               }
          }
     }
}

void BubbleSort_number(FLIGHT information[],int from, int to){
     int i, j;
     FLIGHT temp;
     for (i = from; i < to ; i++)
     {
          for (j = i; j > from ; j-- )
          {
               if (information[j].number < information[j-1].number)
               {
                    temp = information[j];
                    information[j] = information[j-1];
                    information[j-1] = temp;
               }
          }
     }
}

void print_information(FLIGHT information[], int counter){
    int i;
    for(i = 0; i < counter; i++){
        printf("\n FLIGHT %c %02d \n", information[i].company, information[i].number);
        printf("%s", asctime(localtime(&information[i].arrival)));
    }
}
