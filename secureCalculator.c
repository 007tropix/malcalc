#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
/*
CS4920 Port Pirates group project
Normal, more secure version of the calculator

Ryan Du Plooy and Murphy Schaff

This is an example of what a more secure version of our malicious trojan horse calculator would look like

*/
#define STRING_MAX 20
#define FILE_LINE_MAX 100


//function prototypes
bool getOperation(int digit1, int digit2);
void calculator();
void formatString(char string[]);
bool validateInt(const char* buff, int* validInt);
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                       Calculator Functionaltiy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/*
Function that runs the calculator and does the actual math
*/
void calculator(){
    bool runCalc = true;
    bool run = true;
    bool again = true;
    int mode = -1;
    int digit1 = 0;
    int digit2 = 0;
    int result;
    unsigned int valid = 0;
    char op;
    char input[STRING_MAX] = " ";
    puts("What would you like to do first? Your options are:\n0: Do a calcuation\n1: Exit calculator\n2: Administrator Settings");
    while (run){
        fgets(input, STRING_MAX, stdin);
        validateInt(input, &mode);
        if (mode == 0) {
            again = true;
            run = false;
        } else if (mode == 1) {
            again = false;
            run = false;
            runCalc = false;
        } else if (mode == 2){
            //run admin
            puts("You have entered admin mode, if there was one");
            //runCalc = administrator(0);
            run = false;
        } else {
            puts("Please enter 0,1,2.");
        }
    }
    while (runCalc){
        run = true;
        puts("New Calculation:");
        //enter first number, then second number, then operation
        puts("Enter the first digit:");
        fgets(input, STRING_MAX, stdin);
        validateInt(input, &digit1);
        puts("Enter second digit:");
        fgets(input, STRING_MAX, stdin);
        validateInt(input, &digit2);
        puts("Enter operation type '+', '-', '*', '/':");
        run = getOperation(digit1, digit2);

        if (!run) {
            again = false;
        } else {
            //Checks for next move {0: another calculation, 1: exit, 2: enter sudo settings}
            puts("What would you like to do next? Your options are:\n0: Another calcuation\n1: Exit calculator\n2: Administrator Settings");
            while (run){
                fgets(input, STRING_MAX, stdin);
                mode = -1;
                validateInt(input, &mode);
                if (mode == 0) {
                    again = true;
                    run = false;
                } else if (mode == 1) {
                    again = false;
                    run = false;
                } else if (mode == 2){
                    //run admin
                    puts("You have entered admin mode, if there was one");
                    //again = administrator(0);
                    run = false;
                } else {
                    puts("Please enter 0,1,2.");
                }
            }
        }
        //checks if user would like to run another calcualtion
        if (again == false){
            runCalc = false;
        }
    }
    puts("Thank you for using the worlds best calculator!!!");
}
/*
Accepts user input of a valid operation type
*/
bool getOperation(int digit1, int digit2){
    int result = 0;
    bool run = true;
    bool rtn = true;
    char input[STRING_MAX] = " ";

    while (run){
        if (fgets(input, STRING_MAX, stdin) == NULL){
            puts("Please enter a valid stirng/char input");
        } else {
            formatString(input);
            if (strcmp(input, "+") == 0){
                result = digit1 + digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(input, "-") == 0){
                result = digit1 - digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(input, "*") == 0){
                result = digit1 * digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else if (strcmp(input, "/") == 0) {
                result = digit1 / digit2;
                fprintf(stdout, "Result:%d\n", result);
                run = false;
            } else{
                puts("You did not enter a valid operation. Please enter either '+', '-', '*', '/':");
            }
        }
    }
    return rtn;
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                       Misc. Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
Accepts user input of a valid integer value, returns value
*/
bool validateInt(const char* buff, int* validInt){
	char* end;
	errno = 0;
	bool isValid = false;
	//converting the character array into a long value, excluding the parts that could not be turned into a long
	long intTest = strtol(buff, &end, 10);
    formatString(buff);
	//first check is to see if there is a number entered at all
	if (end == buff) {
		fprintf(stderr, "%s: not a decimal number\n", buff);
	}
	//next check is to make sure there were no characters added after any numbers
	else if ('\0' != *end && '\n' != *end) {
		fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
	}
	//next check is to see if the number is within range of the long data type
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	//next check is to see if the number is greater than the max int value
	else if (intTest > INT_MAX) {
		fprintf(stderr, "%ld greater than INT_MAX\n", intTest);
	}
	//next check is to see if the number is less than the min int value
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%ld less than INT_MIN\n", intTest);
	}
	else {
		*validInt = (int)intTest;
		isValid = true;
	}
	return isValid;
}
/*
Formats string by removing new line character
*/
void formatString(char string[]) {
	//removes the new line character from the string/number
	for (unsigned int i = 0; i < STRING_MAX; i++) {
		if (string[i] == '\n') {
			string[i] = '\0';
		}
	}

}

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                                Main
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int main(){
    puts("Welcome to the best ever calculator!");
    calculator();
}