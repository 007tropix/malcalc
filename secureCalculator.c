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
#define STRING_MAX 50
#define FILE_LINE_MAX 100


//function prototypes
bool getOperation(int digit1, int digit2);
void calculator();
void formatString(char* string);
bool validateInt(char* buff, int* validInt);
bool userAuth();
bool findUser();
bool newUser();
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                         User Authentication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/*
Runs the user authentication protocol
*/
bool userAuth(){
    int mode;
    unsigned int valid = 0;
    char input[STRING_MAX] = " ";
    bool auth = false;
    bool getMode = true;
    
    //gets user mode, either existing or create
    while (getMode) {
        puts("Attempting to authenticate as user. Please enter '0' for existing user or '1' for new user");
        fgets(input, STRING_MAX, stdin);
        validateInt(input, &mode);
        if (mode == 0){
            auth = findUser(0);
            getMode = false;
        } else if (mode == 1) {
            auth = newUser();
            getMode = false;
        } else {
            puts("Please enter a '0' for existing user or a '1' for new user");
        }
    }
    //Provides access to calculator
    if (auth){
        puts("Access granted");
        //calls calculator when access is granted.
        calculator();
    }

}
/*
Finds and authenticates existing user
*/
bool findUser(){
    FILE* fileptr;
    unsigned int count = 0;
    unsigned int tot = 5;
    char username[STRING_MAX];
    char password[STRING_MAX];
    char checkUser[STRING_MAX];
    char checkPass[STRING_MAX];

    bool searchUser = true;

    //Allows the user to attempt to login
    while (searchUser) {
        puts("Please enter your username (or 'q' to quit):");
        fgets(username, STRING_MAX, stdin);
        formatString(username);
        //allows for user to quit login sequence
        if (strcmp(username, "q") == 0) {
            return false;
        }
        puts("Please enter your password:");
        fgets(password, STRING_MAX, stdin);
        formatString(password);
        //checks either the normal users file or sudoers file
        fileptr = fopen("users/secure_passwords.txt", "r");

        //compares user input to to that in the file
        if (fileptr == NULL){
            puts("Unable to open passwords file");
        } else {
            //gets each individual username and password from the passwords file, checks against user input
            while (fgets(checkUser, STRING_MAX, fileptr) != NULL) {
                fgets(checkPass, STRING_MAX, fileptr);
                formatString(checkUser);
                formatString(checkPass);
                /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                Where each hashed password needs to be converted and compared against input
                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                if (strcmp(username, checkUser) == 0 && strcmp(password, checkPass) == 0) {
                    searchUser = false;
                }
            }
            //searchUser still true, means it will run loop again
            if (searchUser == true){
                count++;
                if (count == tot) {
                    searchUser = false;
                    puts("Access from calculator denied. Try again.");
                    return false;
                } else {
                    fprintf(stdout, "User not found in file. You have %d attempts remaining\n", tot - count);
                }
            } else {
                puts("found existing user");
                return true;
            }
        }
        fclose(fileptr);
    }
}
/*
Allows for the creation of new users
*/
bool newUser(){
    FILE* fileptr;
    int passlen = 0;
    char username[STRING_MAX];
    char password[STRING_MAX];
    char usercheck[STRING_MAX];
    char hold[STRING_MAX];
    bool checkPass = true;
    bool checkUser = true;

    puts("Welcome new user!");

    //Makes sure that the username is unique to all others on file
    fileptr = fopen("users/secure_passwords.txt", "r");
    while (checkUser) {
        checkUser = false;
        puts("Please enter your username");
        fgets(username, STRING_MAX, stdin);
        formatString(username);
        //checks all usernames on file to see if they are the same
        while (fgets(usercheck, STRING_MAX, fileptr) != NULL) {
            fgets(hold, STRING_MAX, fileptr);
            formatString(usercheck);
            if (strcmp(username, usercheck) == 0) {
                fprintf(stdout, "Username %s has already been used. Please enter another one\n", username);
                checkUser = true;
            }
        }
    }
    fclose(fileptr);
    //Makes sure password is of certian length
    while (checkPass) {
        puts("Please enter your password. It must be at least 14 characters in length");
        fgets(password, STRING_MAX, stdin);
        formatString(password);
        passlen = strlen(password);
        if (passlen < 14) {
            puts("Please enter a password length greater than or equal to 14.");
        } else if (passlen > STRING_MAX) {
            fprintf(stdout, "Please enter a password length that is less than %d", STRING_MAX);
        } else {
            checkPass = false;
        }
    }
    /*
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    This is where the password needs to be hashed and returned
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */

    fileptr = fopen("secure_passwords.txt", "a");
    if (fileptr == NULL){
        puts("Unable to open passwords file");
    } else {
        //adds username and password to passwords.txt file
        fputs(username, fileptr);
        fputs("\n", fileptr);
        fputs(password, fileptr);
        fputs("\n", fileptr);
    }
    fclose(fileptr);
    return true;

}
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
bool validateInt(char* buff, int* validInt){
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
}const 
/*
Formats string by removing new line character
*/
void formatString(char* string) {
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
    userAuth();
    puts("Thank you for using the worlds best calculator!!!");
}