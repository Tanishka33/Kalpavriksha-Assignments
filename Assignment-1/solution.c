#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char inputExpression[100];     // store user input
    char cleanedExpression[100];   // store cleaned expression
    int exitCode = 0;

    printf("Enter a mathematical expression: ");
    fgets(inputExpression, sizeof(inputExpression), stdin);

    int length = strlen(inputExpression);
    if (length > 0 && inputExpression[length - 1] == '\n') {
        inputExpression[length - 1] = '\0';
    }

    // STEP 2: Clean expression and validate
    int operatorIndex = 0; // index for cleanedExpression
    for (int i = 0; inputExpression[i] != '\0'; i++) {
        char currentChar = inputExpression[i];
        if (isspace(currentChar)) {
            continue; // skip spaces
        } else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') {
            cleanedExpression[operatorIndex++] = currentChar; // store valid characters
        } else {
            printf("Error: Invalid expression.\n");
            exitCode = 1;
            goto finish;
        }
    }
    cleanedExpression[operatorIndex] = '\0';
    printf("Cleaned expression is %s\n", cleanedExpression);

    // STEP 3: Parse numbers and operators
    int numberList[50];
    char operatorList[50];
    int numberCount = 0;
    int operatorCount = 0;
    int currentNumber = 0;
    int isBuildingNumber = 0;

    for (int m = 0; cleanedExpression[m] != '\0'; m++) {
        char currentChar = cleanedExpression[m];
        if (isdigit(currentChar)) {
            currentNumber = currentNumber * 10 + (currentChar - '0');
            isBuildingNumber = 1;
        } else {
            if (isBuildingNumber) {
                numberList[numberCount++] = currentNumber;
                currentNumber = 0;
                isBuildingNumber = 0;
            }
            operatorList[operatorCount++] = currentChar;
        }
    }

    if (isBuildingNumber) {
        numberList[numberCount++] = currentNumber;
    }

    // STEP 4: Evaluate expression with precedence (* and / first)
    for (int n = 0; n < operatorCount; n++) {
        if (operatorList[n] == '*' || operatorList[n] == '/') {
            if (operatorList[n] == '/') {
                if (numberList[n + 1] == 0) {
                    printf("Error: Division by zero.\n");
                    exitCode = 1;
                    goto finish;
                }
                numberList[n] = numberList[n] / numberList[n + 1];
            } else {
                numberList[n] = numberList[n] * numberList[n + 1];
            }

            // shift arrays after evaluation
            for (int j = n + 1; j < numberCount - 1; j++) {
                numberList[j] = numberList[j + 1];
            }
            for (int j = n; j < operatorCount - 1; j++) {
                operatorList[j] = operatorList[j + 1];
            }

            numberCount--;
            operatorCount--;
            n--; // recheck same index
        }
    }

    // handle + and -
    int result = numberList[0];
    for (int p = 0; p < operatorCount; p++) {
        if (operatorList[p] == '+') {
            result += numberList[p + 1];
        } else if (operatorList[p] == '-') {
            result -= numberList[p + 1];
        }
    }

    printf("Result: %d\n", result);

finish:
    return exitCode;
}

