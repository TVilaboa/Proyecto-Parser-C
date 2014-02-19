#include <stdio.h>
#include <stdlib.h>
#include "student.h"

#define ELEMENTS    200
#define STUDENTS    50

int main()
{
    STUDENT_FINAL final_exams[ELEMENTS]; //Original array with the grades of students
    STUDENT_FINAL average [STUDENTS]; //Array needed to show averages per student

    init_finals(final_exams);

    printf("NUMBER OF EXAMS PER STUDENT \n");
    print_exams_per_student(final_exams);
    printf("\n");

    printf("AVERAGE PER STUDENT \n");
    print_average_per_student(final_exams);
    printf("\n");

    averages(final_exams,average);
    printf("\n");
    printf("BEST 10 AVERAGES (ascending order) \n)");
    BubbleSort(average,STUDENTS);
    print_best_10_averages(average);
    printf("\n");

    return 0;
}
 
