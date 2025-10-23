#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int** creatingMatrix(int n);
void fillRandomMatrix(int** matrix, int n);
void rotateMatrixClockwise(int** matrix, int n);
void printMatrix(int** matrix, int n);
void smootheningFilter(int** matrix, int n);
void freeMatrix(int** matrix, int n);

int **creatingMatrix(int n)
{
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int index = 0; index < n; index++)
    {
        *(matrix + index) = (int *)malloc(n * sizeof(int));
    }
    return matrix;
}

void fillRandomMatrix(int **matrix, int n)
{
    srand(time(NULL));

    for (int row_index = 0; row_index < n; row_index++)
    {
        for (int col_index = 0; col_index < n; col_index++)
        {
            *(*(matrix + row_index) + col_index) = rand() % 256;
        }
    }
}

void printMatrix(int **matrix, int n)
{
    for (int row_index = 0; row_index < n; row_index++)
    {
        for (int col_index = 0; col_index < n; col_index++)
        {
            printf("%4d ", *(*(matrix + row_index) + col_index));
        }
        printf("\n");
    }
    printf("\n");
}

void freeMatrix(int **matrix, int n)
{
    for (int row_index = 0; row_index < n; row_index++)
    {
        free(*(matrix + row_index));
    }
    free(matrix);
}

void rotateMatrixClockwise(int **matrix, int n)
{
    for (int row_index = 0; row_index < n / 2; row_index++)
    {
        int *temp = *(matrix + row_index);
        *(matrix + row_index) = *(matrix + (n - row_index - 1));
        *(matrix + (n - row_index - 1)) = temp;
    }

    for (int row_index = 0; row_index < n; row_index++)
    {
        for (int col_index = row_index + 1; col_index < n; col_index++)
        {
            int temp = *(*(matrix + row_index) + col_index);
            *(*(matrix + row_index) + col_index) = *(*(matrix + col_index) + row_index);
            *(*(matrix + col_index) + row_index) = temp;
        }
    }
}

void smootheningFilter(int **matrix, int n)
{
    int *previous_row = (int *)malloc(n * sizeof(int));
    int *current_row = (int *)malloc(n * sizeof(int));

    for (int col_index = 0; col_index < n; col_index++)
    {
        *(previous_row + col_index) = *(*(matrix + 0) + col_index);
    }

    for (int row_index = 0; row_index < n; row_index++)
    {
        for (int col_index = 0; col_index < n; col_index++)
        {
            *(current_row + col_index) = *(*(matrix + row_index) + col_index);
        }

        for (int col_index = 0; col_index < n; col_index++)
        {
            int sum = 0;
            int count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int neighbour_row_index = row_index + di;
                    int neighbour_col_index = col_index + dj;

                    if (neighbour_row_index >= 0 && neighbour_row_index < n && neighbour_col_index >= 0 && neighbour_col_index < n)
                    {
                        int value;

                        if (neighbour_row_index == row_index - 1)
                        {
                            value = *(previous_row + neighbour_col_index);
                        }
                        else if (neighbour_row_index == row_index)
                        {
                            value = *(current_row + neighbour_col_index);
                        }
                        else
                        {
                            value = *(*(matrix + neighbour_row_index) + neighbour_col_index);
                        }

                        sum += value;
                        count++;
                    }
                }
            }
            *(*(matrix + row_index) + col_index) = sum / count;
        }
        int *temp = previous_row;
        previous_row = current_row;
        current_row = temp;
    }
    free(previous_row);
    free(current_row);
}

int main()
{

    int n;
    printf("enter matrix size (2-10): \n");
    scanf("%d", &n);

    if(n<2 || n>10){
        printf("Invalid size! Should be between 2 and 10.");
        return 1;
    }

    int **matrix = creatingMatrix(n);
    fillRandomMatrix(matrix, n);

    printf("Original : \n");
    printMatrix(matrix, n);

    rotateMatrixClockwise(matrix, n);
    printf("Rotated matrix:  \n");
    printMatrix(matrix, n);

    smootheningFilter(matrix, n);
    printf("3x3 Smoothining filter \n");
    printMatrix(matrix, n);

    freeMatrix(matrix, n);

    return 0;
}
