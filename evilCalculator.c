#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/*
CS4920 Port Pirates group project
Trojan Horse Calculator application

Ryan Du Plooy and Murphy Schaff

This program contains a bunch of examples of malicious software, from being a trojan horse, contains a backdoor, etc.

*/

#define STRING_MAX 20
//function prototypes
int getInt();
void getOperation(int digit1, int digit2);
bool again();
void calculator();

/*
Function that runs the calculator and does the actual math
*/
void calculator(){
    bool runCalc = true;
    int input = 0;
    int digit1 = 0;
    int digit2 = 0;
    int valid = 0;
    int result;
    char op;

    while (runCalc){
        puts("Welcome to the best ever calculator!");
        //enter first number, then second number, then operation
        puts("Enter the first digit:");
        digit1 = getInt();
        puts("Enter second digit:");
        digit2 = getInt();
        puts("Enter operation type '+', '-', '*', '/':");
        getOperation(digit1, digit2);

        //Open backdoor to stored information
        
        //checks if user would like to run another calcualtion
        if (again() == false){
            runCalc = false;
        }
    }

}

/*
Accepts user input of a valid integer value, returns value
*/
int getInt(){
    bool run = true;
    int integer = 0;
    int valid = 0;
    while(run){
        valid = scanf("%d", &integer);
        if (valid != 1){
            puts("Please enter a valid integer value");
        } else {
            run = false;
        }
    }
    return integer;
}

/*
Accepts user input of a valid operation type
*/
void getOperation(int digit1, int digit2){
    int valid = 0;
    int result = 0;
    bool run = true;
    char op;

    while (run){
        valid = scanf("%s", &op);
        if (valid != 1){
            puts("Please enter a valid stirng/char input");
        } else {
            if (strcmp(&op, "+") == 0){
                result = digit1 + digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(&op, "-") == 0){
                result = digit1 - digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(&op, "*") == 0){
                result = digit1 * digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(&op, "/") == 0) {
                result = digit1 / digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(&op, "~") == 0){
                //trigger backdoor
                puts("backdoor activated!");
                run = false;
            } else{
                puts("You did not enter a valid operation");
            }
        }
    }
}
/*
Prompts the user if they want to do another calculation
*/
bool again(){
    int valid = 0;
    bool run = true;
    bool again = true;
    char input;
    puts("Would you like to do another calculation? Enter 'y' or 'n'");
    while (run){
        valid = scanf("%s", &input);
        if (valid != 1){
            puts("Please enter 'y' or 'n'");
        } else {
            if (strcmp(&input, "n") == 0) {
                again = false;
                run = false;
            } else if (strcmp(&input, "y") == 0) {
                again = true;
                run = false;
            } else {
                puts("Please enter 'y' or 'n'");
            }
        }
    }
    return again;
}

int main(){
    //Trojan Horse Calculator
    calculator(); 
}