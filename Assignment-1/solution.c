#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

int removeWhitespace(char input[], char cleaned[]) {
    int operatorIndex = 0;
    int status = 0;

    for (int index = 0; input[index] != '\0'; index++) {
        char currentChar = input[index];
        if (isspace(currentChar)) {
            continue;
        } else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-' ||
                   currentChar == '*' || currentChar == '/') {
            cleaned[operatorIndex++] = currentChar;
        } else {
            status = -1;
            break;
        }
    }
    cleaned[operatorIndex] = '\0';
    return status;
}

int parseAndEvaluate(char cleaned[], int numberStack[], char operatorStack[], int *numberTop, int *operatorTop) {
    int currentNumber = 0;
    int isBuildingNumber = 0;
    int status = 0;

    for (int index = 0; cleaned[index] != '\0'; index++) {
        char currentChar = cleaned[index];
        if (isdigit(currentChar)) {
            currentNumber = currentNumber * 10 + (currentChar - '0');
            isBuildingNumber = 1;
        } else {
            if (isBuildingNumber) {
                numberStack[++(*numberTop)] = currentNumber;
                currentNumber = 0;
                isBuildingNumber = 0;
            }

            if (currentChar == '*' || currentChar == '/') {
                char op = currentChar;
                int nextNumber = 0;

                index++;
                while (isdigit(cleaned[index])) {
                    nextNumber = nextNumber * 10 + (cleaned[index] - '0');
                    index++;
                }
                index--;

                if (op == '*') {
                    numberStack[*numberTop] *= nextNumber;
                } else {
                    if (nextNumber == 0) {
                        status = -1; 
                        break;
                    }
                    numberStack[*numberTop] /= nextNumber;
                }
            } else {
                operatorStack[++(*operatorTop)] = currentChar;
            }
        }
    }

    if (status == 0 && isBuildingNumber) {
        numberStack[++(*numberTop)] = currentNumber;
    }

    return status;
}

int evaluatePlusMinus(int numberStack[], char operatorStack[], int numberTop, int operatorTop) {
    int result = numberStack[0];
    int numIndex = 1;

    for (int index = 0; index <= operatorTop; index++) {
        if (operatorStack[index] == '+') {
            result += numberStack[numIndex++];
        } else if (operatorStack[index] == '-') {
            result -= numberStack[numIndex++];
        }
    }
    return result;
}

int main() {
    char inputExpression[MAX_SIZE];
    char cleanedExpression[MAX_SIZE];
    int numberStack[MAX_SIZE];
    char operatorStack[MAX_SIZE]; 
    int numberTop = -1, operatorTop = -1;
    int result = 0;
    int status = 0;

    printf("Enter a mathematical expression: ");
    fgets(inputExpression, sizeof(inputExpression), stdin);

    int length = strlen(inputExpression);
    if (length > 0 && inputExpression[length - 1] == '\n') {
        inputExpression[length - 1] = '\0';
    }

    status = removeWhitespace(inputExpression, cleanedExpression);
    if (status != 0) {
        printf("Error: Invalid expression.\n");
    } else {
        printf("Cleaned expression is %s\n", cleanedExpression);

        status = parseAndEvaluate(cleanedExpression, numberStack, operatorStack, &numberTop, &operatorTop);
        if (status != 0) {
            printf("Error: Division by zero.\n");
        } else {
            result = evaluatePlusMinus(numberStack, operatorStack, numberTop, operatorTop);
            printf("Result: %d\n", result);
        }
    }

    return 0;
}
