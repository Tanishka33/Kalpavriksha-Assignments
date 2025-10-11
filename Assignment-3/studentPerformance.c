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

void inputDetails(studentPerformance student[], int n)
{
    for (int index = 0; index < n; index++)
    {
        printf("Enter the detail of students \n", &student[index]);

        printf("Roll Number :");
        scanf("%d", &student[index].rollNumber);

        printf("Name : ");
        scanf("%s", student[index].name);

        printf("Marks 1 : ");
        scanf("%d", &student[index].marks[0]);

        printf("Marks 2 : ");
        scanf("%d", &student[index].marks[1]);

        printf("Marks 3 : ");
        scanf("%d", &student[index].marks[2]);
    }

}

void calculateResults(studentPerformance student[], int n)
{
    for (int index = 0; index < n; index++)
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

void displayResult(studentPerformance student[], int n)
{
    for (int index = 0; index < n; index++)
    {
    printf(" RollNumber: %d \n Name: %s \n Total: %.2f \n Average: %.2f \n Grade: %c \n  ",
               student[index].rollNumber,
               student[index].name,
               student[index].totalScore,
               student[index].average,
               student[index].grade);

        switch (student[index].grade)
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

        printf("\n \n");
    }
}

void printRollNumber(studentPerformance student[], int index, int n)
{
    if (index == n)
    {
        return;
    }
    printf("%d ", student[index].rollNumber);

    printRollNumber(student, index+1, n);
}

int main()
{
    int n;
    printf("Enter the number of students to analyze: ");
    scanf("%d", &n);

    studentPerformance student[n];

    inputDetails(student, n);
    calculateResults(student, n);
    displayResult(student, n);

    printf("List of Roll Number (via recursion): ");
    printRollNumber(student, 0, n);
    printf("\n");

    return 0;
}