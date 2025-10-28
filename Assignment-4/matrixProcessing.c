#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** creatingMatrix(const int matrixSize);
void fillRandomMatrix(int** matrix, const int matrixSize);
void rotateMatrixClockwise(int** matrix, const int matrixSize);
void printMatrix(int * const * matrix, const int matrixSize);
void smootheningFilter(int** matrix, const int matrixSize);
void freeMatrix(int** matrix, const int matrixSize);

int **creatingMatrix(const int matrixSize)
{
    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int index = 0; index < matrixSize; index++)
    {
        *(matrix + index) = (int *)malloc(matrixSize * sizeof(int));
    }
    return matrix;
}

void fillRandomMatrix(int **matrix, const int matrixSize)
{
    srand(time(NULL));

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            *(*(matrix + rowIndex) + colIndex) = rand() % 256;
        }
    }
}

void printMatrix(int * const * matrix, const int matrixSize)
{
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            printf("%4d ", *(*(matrix + rowIndex) + colIndex));
        }
        printf("\n");
    }
    printf("\n");
}

void freeMatrix(int **matrix, const int matrixSize)
{
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        free(*(matrix + rowIndex));
    }
    free(matrix);
}

void rotateMatrixClockwise(int **matrix, const int matrixSize)
{
    for (int rowIndex = 0; rowIndex < matrixSize / 2; rowIndex++)
    {
        int *temp = *(matrix + rowIndex);
        *(matrix + rowIndex) = *(matrix + (matrixSize - rowIndex - 1));
        *(matrix + (matrixSize - rowIndex - 1)) = temp;
    }

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int colIndex = rowIndex + 1; colIndex < matrixSize; colIndex++)
        {
            int temp = *(*(matrix + rowIndex) + colIndex);
            *(*(matrix + rowIndex) + colIndex) = *(*(matrix + colIndex) + rowIndex);
            *(*(matrix + colIndex) + rowIndex) = temp;
        }
    }
}

void smootheningFilter(int **matrix, const int matrixSize)
{
    int *previousRow = (int *)malloc(matrixSize * sizeof(int));
    int *currentRow = (int *)malloc(matrixSize * sizeof(int));

    for (int colIndex = 0; colIndex < matrixSize; colIndex++)
    {
        *(previousRow + colIndex) = *(*(matrix + 0) + colIndex);
    }

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            *(currentRow + colIndex) = *(*(matrix + rowIndex) + colIndex);
        }

        for (int colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            int sum = 0;
            int count = 0;

            for (int rowShift = -1; rowShift <= 1; rowShift++)
            {
                for (int colShift = -1; colShift <= 1; colShift++)
                {
                    int neighbourRowIndex = rowIndex + rowShift;
                    int neighbourColIndex = colIndex + colShift;

                    if (neighbourRowIndex >= 0 && neighbourRowIndex < matrixSize && neighbourColIndex >= 0 && neighbourColIndex < matrixSize)
                    {
                        int value;

                        if (neighbourRowIndex == rowIndex - 1)
                        {
                            value = *(previousRow + neighbourColIndex);
                        }
                        else if (neighbourRowIndex == rowIndex)
                        {
                            value = *(currentRow + neighbourColIndex);
                        }
                        else
                        {
                            value = *(*(matrix + neighbourRowIndex) + neighbourColIndex);
                        }

                        sum += value;
                        count++;
                    }
                }
            }
            *(*(matrix + rowIndex) + colIndex) = sum / count;
        }
        int *temp = previousRow;
        previousRow = currentRow;
        currentRow = temp;
    }
    free(previousRow);
    free(currentRow);
}

int main()
{
    int matrixSize;
    int status = 0; 

    printf("Enter matrix size (2-10): \n");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("Invalid size! Should be between 2 and 10.\n");
        status = 1; 
    } 
    else {
        int **matrix = creatingMatrix(matrixSize);
        fillRandomMatrix(matrix, matrixSize);

        printf("Original Matrix:\n");
        printMatrix(matrix, matrixSize);

        rotateMatrixClockwise(matrix, matrixSize);
        printf("Rotated Matrix:\n");
        printMatrix(matrix, matrixSize);

        smootheningFilter(matrix, matrixSize);
        printf("3x3 Smoothening Filter Applied:\n");
        printMatrix(matrix, matrixSize);

        freeMatrix(matrix, matrixSize);
    }

    return status;
}
