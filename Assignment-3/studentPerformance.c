#include <stdio.h>

typedef struct Student
{
    int rollNumber;
    char name[30];
    int marks[3];
    float totalScore;
    float average;
    char grade;
} Student;

void getStudentData(Student student[], const int studentCount);
void calculateStudentResults(Student student[], const int studentCount);
void displayResult(Student student[], const int studentCount);
void printRollNumber(Student student[], int index, const int studentCount);
void printPerformance(char grade);

void getStudentData(Student student[], const int studentCount)
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

void calculateStudentResults(Student student[], const int studentCount)
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

        else 
        {
            student[index].grade = 'F';
        }
    }
}

void printStudentResults(Student student[], const int studentCount)
{
    for (int index = 0; index < studentCount; index++)
    {
    printf(" RollNumber: %d \n Name: %s \n Total: %.2f \n Average: %.2f \n Grade: %c\n  ",
               student[index].rollNumber,
               student[index].name,
               student[index].totalScore,
               student[index].average,
               student[index].grade);

        if(student[index].grade != 'F')
        {
            printf("Performance: ");
            printPerformanceStars(student[index].grade);
        }
        printf("\n\n");
   }
}

void printRollNumber(Student student[], int index, const int studentCount)
{
    if (index == studentCount)
    {
        return;
    }
    printf("%d ", student[index].rollNumber);
    printRollNumber(student, index+1, studentCount);
}

void printPerformanceStars(char grade)
{
    int starCount = 0;
    switch (grade)
        {
        case 'A':
            starCount = 5;
            break;
        case 'B':
            starCount = 4;
            break;
        case 'C':
            starCount = 3;
            break;
        case 'D':
            starCount = 2;
            break;
        case 'F':
            break;
        default:
            break;
        }
        
        for(int index = 0; index < starCount; index++)
        {
            printf("*");
        }
}

int main()
{
    int studentCount;
    printf("Enter the number of students to analyze: ");
    scanf("%d", &studentCount);

    Student student[studentCount];

    getStudentData(student, studentCount);
    calculateStudentResults(student, studentCount);
    printStudentResults(student, studentCount);

    printf("List of Roll Number (via recursion): ");
    printRollNumber(student, 0, studentCount);
    printf("\n");

    return 0;
}
