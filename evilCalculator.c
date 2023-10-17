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
bool findUser();
bool newUser();
bool userAuth();
void formatString(char string[]);


/*
Runs the user authentication protocol
*/
bool userAuth(){
    unsigned int valid = 0;
    unsigned int input;
    bool auth = false;
    
    while (!auth){
        puts("Attempting to authenticate as user. Please enter '0' for existing user or '1' for new user");
        valid = scanf("%d", &input);
        while ((getchar()) != '\n');
        if (valid != 1){
            puts("Please enter a '0' for existing user or a '1' for new user");
        } else {
            if (input == 0){
                auth = findUser();
            } else {
                auth = newUser();
            }
        }

        if (auth){
            puts("Access granted");
        }
    }

}
/*
Finds and authenticates existing user
*/
bool findUser(){
    FILE* fileptr;
    char username[STRING_MAX];
    char password[STRING_MAX];
    char checkUser[STRING_MAX];
    char checkPass[STRING_MAX];
    bool searchUser = true;

    //Allows the user to attempt to login
    while (searchUser) {
        puts("Please enter your username:");
        scanf("%s", &username);
        puts("Please enter your password:");
        scanf("%s", &password);

        //compares user input to to that in the file
        fileptr = fopen("users/passwords.txt", "r");
        if (fileptr == NULL){
            puts("Unable to open passwords file");
        } else {
            //gets each individual username and password from the passwords file, checks against user input
            while (fgets(checkUser, STRING_MAX, fileptr) != NULL) {
                fgets(checkPass, STRING_MAX, fileptr);
                formatString(checkUser);
                formatString(checkPass);
                if (strcmp(&username, &checkUser) == 0 && strcmp(&password, &checkPass) == 0){
                    searchUser = false;
                }
            }
            if (searchUser == true){
                puts("Incorrect password, please try again");
            } else {
                puts("found existing user");
            }
        }
        fclose(fileptr);
    }
    return !searchUser;
}
/*
Allows for the creation of new users
*/
bool newUser(){
    FILE* fileptr;
    char username[STRING_MAX];
    char password[STRING_MAX];

    puts("Please enter your username");
    scanf("%s", &username);

    puts("Please enter your password");
    scanf("%s", &password);

    fileptr = fopen("users/passwords.txt", "a");
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
Function that runs the calculator and does the actual math
*/
void calculator(){
    bool runCalc = true;
    int input = 0;
    int digit1 = 0;
    int digit2 = 0;
    int result;
    unsigned int valid = 0;
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
    unsigned int valid = 0;
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
    int result = 0;
    unsigned int valid = 0;
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
    unsigned int valid = 0;
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
/*
Formats string by removing new line character
*/
void formatString(char string[]) {
	//removes the new line character from the string/number
	for (unsigned int i = 0; i < STRING_MAX; i++) {
		if (string[i] == '\n') {
			string[i] = NULL;
		}
	}

}

int main(){
    //Trojan Horse Calculator
    //calculator();
    userAuth();
}