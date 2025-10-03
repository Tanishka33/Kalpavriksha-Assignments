#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

int main() {
    char inputExpression[MAX_SIZE];     // store user input
    char cleanedExpression[MAX_SIZE];   // store cleaned expression
    int exitCode = 0;

    printf("Enter a mathematical expression: ");
    fgets(inputExpression, sizeof(inputExpression), stdin);

    int length = strlen(inputExpression);
    if (length > 0 && inputExpression[length - 1] == '\n') {
        inputExpression[length - 1] = '\0';
    }

    // STEP 2: Clean expression and validate
    int operatorIndex = 0; 
    for (int i = 0; inputExpression[i] != '\0'; i++) {
        char currentChar = inputExpression[i];
        if (isspace(currentChar)) {
            continue; // skip spaces
        } else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-' ||
                   currentChar == '*' || currentChar == '/') {
            cleanedExpression[operatorIndex++] = currentChar; 
        } else {
            printf("Error: Invalid expression.\n");
            exitCode = 1;
            goto finish;
        }
    }
    cleanedExpression[operatorIndex] = '\0';
    printf("Cleaned expression is %s\n", cleanedExpression);

    // STEP 3: Use stacks to parse numbers and operators
    int numberStack[MAX_SIZE];
    char operatorStack[MAX_SIZE];
    int numberTop = -1;
    int operatorTop = -1;
    int currentNumber = 0;
    int isBuildingNumber = 0;

    for (int i = 0; cleanedExpression[i] != '\0'; i++) {
        char currentChar = cleanedExpression[i];
        if (isdigit(currentChar)) {
            currentNumber = currentNumber * 10 + (currentChar - '0');
            isBuildingNumber = 1;
        } else {
            if (isBuildingNumber) {
                numberStack[++numberTop] = currentNumber; 
                currentNumber = 0;
                isBuildingNumber = 0;
            }
            
            if (currentChar == '*' || currentChar == '/') {
                char op = currentChar;
                int nextNumber = 0;

                
                i++;
                while (isdigit(cleanedExpression[i])) {
                    nextNumber = nextNumber * 10 + (cleanedExpression[i] - '0');
                    i++;
                }
                i--; 

                if (op == '*') numberStack[numberTop] = numberStack[numberTop] * nextNumber;
                else {
                    if (nextNumber == 0) {
                        printf("Error: Division by zero.\n");
                        exitCode = 1;
                        goto finish;
                    }
                    numberStack[numberTop] = numberStack[numberTop] / nextNumber;
                }
            } else {
                operatorStack[++operatorTop] = currentChar; 
            }
        }
    }
    if (isBuildingNumber) {
        numberStack[++numberTop] = currentNumber; 
    }

    // STEP 4: Evaluate remaining + and - operators
    int result = numberStack[0];
    int numIndex = 1;
    for (int i = 0; i <= operatorTop; i++) {
        if (operatorStack[i] == '+') result += numberStack[numIndex++];
        else if (operatorStack[i] == '-') result -= numberStack[numIndex++];
    }

    printf("Result: %d\n", result);

finish:
    return exitCode;
}

