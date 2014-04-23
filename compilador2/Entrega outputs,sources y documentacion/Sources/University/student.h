#include <time.h>

//#define ELEMENTS    200       Ya estan definidas en University.c, entonces el parser las
//                              reemplaza por su valor y crashea al quedar #define 50 50
//#define STUDENTS    50
#define GRADES  10

/**
Determines a student with a code and a grade of a final
*/
typedef struct{
    int  student_code;
    int  grade;
}STUDENT_FINAL;

/**
    Builds an Array with a capacity of [200] which holds both the university code and
    the grades of the students
*/
void init_finals(STUDENT_FINAL finals [])
{
    int i = 0;
    for(i = 0; i < ELEMENTS; i++){
        finals[i].student_code = (rand() % STUDENTS) + 1;
        printf("Student Code %02d \n", finals[i].student_code);
        finals[i].grade = (rand() % GRADES) + 1;
        printf("Grade %02d \n", finals[i].grade);
        printf("\n");
    }
}

/**
Searches how many exams a student has given up-to-date
*/
int search_exams_per_student(int code, STUDENT_FINAL finals [])
{
    int counter = 0;
    int i;
    for(i = 0; i < ELEMENTS; i++){
        if(finals[i].student_code == code){
            counter ++;
        }
    }

    return counter;
}

/**
Prints number of exams per student
*/
void print_exams_per_student(STUDENT_FINAL finals [])
{
    int i;
    for (i = 1; i < STUDENTS + 1; i++){
        printf("STUDENT %02d has given %02d finals up-to-date \n",i,search_exams_per_student(i,finals));
    }
}

/**
Finds the average of a student using its code
*/
int average_per_student(int code, STUDENT_FINAL finals [])
{
    int counter = 0;
    int i;
    for(i = 0 ; i < ELEMENTS; i++){
        if (finals[i].student_code == code){
            counter += (finals[i].grade);
        }
    }
    int average = counter / search_exams_per_student(code,finals);
    return average;
}

/**
Prints averages for all students
*/
void print_average_per_student(STUDENT_FINAL finals [])
{
    int i;
    for (i = 1; i < STUDENTS + 1; i++){
        printf("STUDENT %02d average is %02d \n",i,average_per_student(i,finals));
    }
}

/**
Sets and array that holds for a specific student code it's average
*/
void averages(STUDENT_FINAL finals[],STUDENT_FINAL averages[]){
    int i;
    for(i = 1; i < STUDENTS + 1; i++){
        averages[i - 1].student_code = i;
        //printf("STUDENT %02d \n",averages[i - 1].student_code);
        averages[i - 1].grade = average_per_student(i,finals);
        //printf("AVERAGE %02d \n",averages[i - 1].grade );
    }
}

/**
Sorts this array in an ascending order
*/
void BubbleSort(STUDENT_FINAL average[], int array_size)
{
     int i, j;
     STUDENT_FINAL temp;
     for (i = 0; i < (array_size - 1); ++i)
     {
          for (j = 0; j < array_size - 1 - i; ++j )
          {
               if (average[j].grade > average[j+1].grade)
               {
                    temp = average[j+1];
                    average[j+1] = average[j];
                    average[j] = temp;
               }
          }
     }
}

/**
Prints last 10 positions of the array (average) after it has been sorted
*/
void print_best_10_averages(STUDENT_FINAL average[])
{
   int i;

   for (i = 40; i < STUDENTS  ; i++)
      printf("STUDENT: %d AVERAGE: %d \n",
               average[i].student_code, average[i].grade);
}
