#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

// Function to remove whitespace and validate expression
int removeWhitespace(char input[], char cleaned[]) {
    int operatorIndex = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        char currentChar = input[i];
        if (isspace(currentChar)) {
            continue;
        } else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-' ||
                   currentChar == '*' || currentChar == '/') {
            cleaned[operatorIndex++] = currentChar;
        } else {
            return -1; 
        }
    }
    cleaned[operatorIndex] = '\0';
    return 0; 
}

// Function to parse numbers and operators, and evaluate * and /
int parseAndEvaluate(char cleaned[], int numberStack[], char operatorStack[], int *numberTop, int *operatorTop) {
    int currentNumber = 0;
    int isBuildingNumber = 0;

    for (int i = 0; cleaned[i] != '\0'; i++) {
        char currentChar = cleaned[i];
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

                i++;
                while (isdigit(cleaned[i])) {
                    nextNumber = nextNumber * 10 + (cleaned[i] - '0');
                    i++;
                }
                i--; 

                if (op == '*') numberStack[*numberTop] *= nextNumber;
                else {
                    if (nextNumber == 0) return -1; // division by zero
                    numberStack[*numberTop] /= nextNumber;
                }
            } else {
                operatorStack[++(*operatorTop)] = currentChar; // store + or -
            }
        }
    }

    if (isBuildingNumber) {
        numberStack[++(*numberTop)] = currentNumber;
    }

    return 0; 
}

// Function to evaluate remaining + and - operators
int evaluatePlusMinus(int numberStack[], char operatorStack[], int numberTop, int operatorTop) {
    int result = numberStack[0];
    int numIndex = 1;
    for (int i = 0; i <= operatorTop; i++) {
        if (operatorStack[i] == '+') result += numberStack[numIndex++];
        else if (operatorStack[i] == '-') result -= numberStack[numIndex++];
    }
    return result;
}

int main() {
    char inputExpression[MAX_SIZE];
    char cleanedExpression[MAX_SIZE];
    int numberStack[MAX_SIZE];
    char operatorStack[MAX_SIZE]; 
    int numberTop = -1, operatorTop = -1;
    int exitCode = 0;

    printf("Enter a mathematical expression: ");
    fgets(inputExpression, sizeof(inputExpression), stdin);

    int length = strlen(inputExpression);
    if (length > 0 && inputExpression[length - 1] == '\n') {
        inputExpression[length - 1] = '\0';
    }

    // STEP 1: Remove whitespace and validate
    if (removeWhitespace(inputExpression, cleanedExpression) == -1) {
        printf("Error: Invalid expression.\n");
        exitCode = 1;
        goto finish;
    }
    printf("Cleaned expression is %s\n", cleanedExpression);

    // STEP 2: Parse numbers and evaluate * and /
    if (parseAndEvaluate(cleanedExpression, numberStack, operatorStack, &numberTop, &operatorTop) == -1) {
        printf("Error: Division by zero.\n");
        exitCode = 1;
        goto finish;
    }

    // STEP 3: Evaluate remaining + and - operators
    int result = evaluatePlusMinus(numberStack, operatorStack, numberTop, operatorTop);
    printf("Result: %d\n", result);

finish:
    return exitCode;
}
