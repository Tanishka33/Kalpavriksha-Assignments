#include<stdio.h>
#include<string.h>
#include<ctype.h> // for isdigit and isspace

int main(){
    char expression[100]; //max length of string is 100
    char cleaned[100];

    printf("Enter a mathematical expression: ");
    fgets(expression, sizeof(expression), stdin); // read full line including spaces

    // To remove trailing newlines 
    size_t len=strlen(expression);
    if(len>0 && expression[len-1]=='\n'){
        expression[len-1] = '\0';
    }

    //STEP:2 CLEAN EXPRESSION AND VALIDATE
    int j=0; //index for cleaned[]
    for(int i=0;expression[i]!='\0';i++){
        char ch = expression[i];

        if(isspace(ch)){
            continue;
        } else if(isdigit(ch) || ch=='+' || ch == '-' || ch == '*' || ch == '/'){
            //if valid character copy it to cleaned[]
            cleaned[j++] = ch;
        } else {
            // invalid character is found
            printf("Error: Invalid expression. \n");
            return 0;
        }
    }

    cleaned[j] = '\0';
    printf("Cleaned expression is %s\n", cleaned);

    //STEP 3: Parse numbers and operators
    int numbers[50]; //store numbers
    char operators[50]; //store operators
    int num_count = 0; //count of numbers
    int op_count = 0; //count of operators
    int num = 0; // temp for multidigit number
    int building_number = 0;

    for(int i=0; cleaned[i]!='\0';i++){
        char ch = cleaned[i];
        if(isdigit(ch)){
            //building multidigit
            num = num*10 + (ch-'0');
            building_number = 1;
        } else{
            //operator found then store previous number
            if(building_number){
                numbers[num_count++] = num;
                num = 0;
                building_number=0;
            }
            //store operator
            operators[op_count++]=ch;
        }
    }

    //storing the last number
    if(building_number){
        numbers[num_count++]=num;
    }
/* TO PRINT NUMBERS AND OPERATOR STACK 
    printf("Numbers: ");
    for(int i=0;i<num_count;i++)
        printf("%d ", numbers[i]);
    printf("\nOperators: ");
    for(int i=0;i<op_count;i++)
        printf("%c ", operators[i]);
    printf("\n");
*/

    //STEP:4 evaluate expression with precedance
    // first parse will handle '*' and '/'
    for(int i=0; i<op_count;i++){
        if(operators[i]=='*' || operators[i]=='/'){
            if(operators[i]=='/'){
                if(numbers[i+1]==0){
                        printf("Error: Division by zero.\n"); // if next number is 0 then it's error
                        return 0;
                    }
                    numbers[i] = numbers[i]/numbers[i+1];
                } else{ // if operators[i] == '*'
                    numbers[i] = numbers[i]*numbers[i+1];
                }

                //now shift numbers and operators to left
                for(int j=i+1; j<num_count-1; j++){
                    numbers[j]=numbers[j+1];
                }
                for(int j=i; j<op_count-1; j++){
                    operators[j] = operators[j+1];
                }

                num_count--; // one number is used
                op_count--; // one operator is used
                i--; // stay at same index for next iteration
            }
        }

    int result = numbers[0];
    for(int i=0; i<op_count; i++){
        if(operators[i]=='+') result+=numbers[i+1];
        else if(operators[i]=='-') result-=numbers[i+1];
    }

    printf("Result: %d\n", result);

    return 0;
}