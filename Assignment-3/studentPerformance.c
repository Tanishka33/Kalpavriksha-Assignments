#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

typedef struct StudentPerformanceAnalyzer
{
    int rollNumber;
    char name[30];
    int marks[3];
    float totalScore;
    float average;
    char grade;
} studentPerformance;

void inputDetails(studentPerformance student[], int studentCount);
void calculateResults(studentPerformance student[], int studentCount);
void displayResult(studentPerformance student[], int studentCount);
void printRollNumber(studentPerformance student[], int index, int studentCount);
void printPerformance(char grade);

void inputDetails(studentPerformance student[], int studentCount)
{
    for (int index = 0; index < studentCount; index++)
    {
        scanf("%d %s %d %d %d",
            &student[index].rollNumber,
            student[index].name,
            &student[index].marks[0],
            &student[index].marks[1],
            &student[index].marks[2]);
    }

}

void calculateResults(studentPerformance student[], int studentCount)
{
    for (int index = 0; index < studentCount; index++)
    {
        student[index].totalScore = student[index].marks[0] + student[index].marks[1] + student[index].marks[2];

        student[index].average = student[index].totalScore / 3.0;

        if (student[index].average >= 85)
        {
            student[index].grade = 'A';
        }

        else if (student[index].average >= 70)
        {
            student[index].grade = 'B';
        }

        else if (student[index].average >= 50)
        {
            student[index].grade = 'C';
        }

        else if (student[index].average >= 35)
        {
            student[index].grade = 'D';
        }

        else if (student[index].average < 35)
        {
            student[index].grade = 'F';
        }
    }
}

void displayResult(studentPerformance student[], int studentCount)
{
    for (int index = 0; index < studentCount; index++)
    {
    printf(" RollNumber: %d \n Name: %s \n Total: %.2f \n Average: %.2f \n Grade: %c \n  ",
               student[index].rollNumber,
               student[index].name,
               student[index].totalScore,
               student[index].average,
               student[index].grade);

        printf("\n \n");

        if(student[index].grade == 'F')
        {
            printf("\n");
            continue;
        }

        printPerformance(student[index].grade);
        printf("\n");
   }
}

void printRollNumber(studentPerformance student[], int index, int studentCount)
{
    if (index == studentCount)
    {
        return;
    }
    printf("%d ", student[index].rollNumber);

    printRollNumber(student, index+1, studentCount);
}

void printPerformance(char grade)
{
    switch (grade)
        {
        case 'A':
            printf("Performance: *****");
            break;
        case 'B':
            printf("Performance: ****");
            break;
        case 'C':
            printf("Performance: ***");
            break;
        case 'D':
            printf("Performance: **");
            break;
        case 'F':
            break;
        default:
            break;
        }
}



int main()
{
    int studentCount;
    printf("Enter the number of students to analyze: ");
    scanf("%d", &studentCount);

    studentPerformance student[studentCount];

    inputDetails(student, studentCount);
    calculateResults(student, studentCount);
    displayResult(student, studentCount);

    printf("List of Roll Number (via recursion): ");
    printRollNumber(student, 0, studentCount);
    printf("\n");

    return 0;
}
