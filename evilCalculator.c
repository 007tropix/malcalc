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
void findUser(unsigned int userType);
bool newUser();
bool userAuth();
void formatString(char string[]);
bool administrator(unsigned int backdoor);

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                       Regular User Authentication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
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
                findUser(0);
                auth = true;
            } else {
                auth = newUser();
            }
        }

        if (auth){
            puts("Access granted");
            //calls calculator when access is granted.
            calculator();
        }
    }

}
/*
Finds and authenticates existing user
*/
void findUser(unsigned int userType){
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

        //checks either the normal users file or sudoers file
        if (userType == 0) {
            fileptr = fopen("users/passwords.txt", "r");
        } else {
            fileptr = fopen("users/sudoers.txt", "r");
        }
        //compares user input to to that in the file
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
                puts("User not found in file. Try again");
            } else {
                puts("found existing user");
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                       Administrator Panel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
bool administrator(unsigned int backdoor){
    int entry;
    char checkUser[STRING_MAX];
    char checkPass[STRING_MAX];
    bool userauth = false;
    bool checkEntry = true;
    bool checkLeave = true;
    FILE* fileptr;
    //You can only login as administrator, cannot create account
    if (backdoor == 0) {
        findUser(1);
    }
    puts("Welcome 'Administrator'!");

    while (checkEntry) {
        puts("Here is what you can do, enter for the following options:\n0: Exit back to calculator\n1: Print all user's usernames and passwords\n2: Print all stolen data to file\n3: Maybe more?");
        entry = getInt();
        //User wants to leave admin mode, can either close program or enter back to calculator
        if (entry == 0){
            puts("Enter values to:\n0: Close calculator program\n1: Perform another calculation");
            checkEntry = false;
            while (checkLeave) {
                entry = getInt();
                if (entry == 0){
                    checkLeave = false;
                    return false;
                } else if (entry == 1){
                    checkLeave = false;
                    return true;
                } else {
                    puts("You did not enter a valid input. Please enter 0,1");
                }
            }
        //Admin wants to print all usernames and passwords in the passwords.txt file
        } else if (entry == 1) {
            puts("Printing all usernames and passwords to console:\n");
            fileptr = fopen("users/passwords.txt", "r");
            if (fileptr != NULL){
                while (fgets(checkUser, STRING_MAX, fileptr) != NULL) {
                    fgets(checkPass, STRING_MAX, fileptr);
                    formatString(checkUser);
                    formatString(checkPass);
                    fprintf(stdout, "Username: '%s' Password: '%s'\n", checkUser, checkPass);
                }
                puts("\nEnd user stream");
            } else {
                puts("unable to open passwords file. Thats not good...");
            }
            fclose(fileptr);
        //Admin wants to print all stolen data from the system (i.e. running tree file and that stuff)
        } else if (entry == 2) {
            puts("This is where all stolen data would be printed. If I had any...");
        } else {
            puts("Please enter a valid option");
        }
    }
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
    int input = 0;
    int digit1 = 0;
    int digit2 = 0;
    int result;
    unsigned int valid = 0;
    char op;
    puts("What would you like to do first? Your options are:\n0: Do a calcuation\n1: Exit calculator\n2: Administrator Settings");
    while (run){
        input = getInt();
        if (input == 0) {
            again = true;
            run = false;
        } else if (input == 1) {
            again = false;
            run = false;
        } else if (input == 2){
            //run admin
            runCalc = administrator(0);
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
        digit1 = getInt();
        puts("Enter second digit:");
        digit2 = getInt();
        puts("Enter operation type '+', '-', '*', '/':");
        getOperation(digit1, digit2);

        //Checks for next move {0: another calculation, 1: exit, 2: enter sudo settings}
        puts("What would you like to do next? Your options are:\n0: Another calcuation\n1: Exit calculator\n2: Administrator Settings");
        while (run){
            input = getInt();
            if (input == 0) {
                again = true;
                run = false;
            } else if (input == 1) {
                again = false;
                run = false;
            } else if (input == 2){
                //run admin
                again = administrator(0);
                run = false;
            } else {
                puts("Please enter 0,1,2.");
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
                administrator(1);
            } else{
                puts("You did not enter a valid operation");
            }
        }
    }
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                       Misc. Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
Accepts user input of a valid integer value, returns value
*/
int getInt(){
    bool run = true;
    int integer = 0;
    unsigned int valid = 0;
    while(run){
        valid = scanf("%d", &integer);
        while ((getchar()) != '\n');
        if (valid != 1){
            puts("Please enter a valid integer value");
        } else {
            run = false;
        }
    }
    return integer;
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

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                                Main
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int main(){
    //Trojan Horse Calculator
    //calculator();
    puts("Welcome to the best ever calculator!");
    userAuth();
}