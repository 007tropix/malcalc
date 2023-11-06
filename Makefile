#CS4920 Port-Pirates project MakeFile
#Ryan Du Plooy and Murphy Schaff

all: dependencies clean compile

dependencies:
	@echo "Installing all dependencies. Sudo password required."
	@echo "Installing tree...."
	@sudo apt install tree
	@echo "Installing gcc...."
	@sudo apt install gcc

clean:
	@echo "Cleaning Files...."
	@rm data/*.txt
	@rm users/passwords.txt
	@rm users/sudoers.txt
	@rm *.tar

	@echo "Creating default users for evilCalculator..."
	@echo "default" > users/passwords.txt
	@echo "default" >> users/passwords.txt
	@chmod 600 users/secure_passwords.txt
	
	@echo "murphy" > users/sudoers.txt
	@echo "hi" >> users/sudoers.txt

compile:
	@echo "Compiling Evil and Secure calculators..."
	@gcc -o calculator evilCalculator.c
	@gcc -o secureCalculator secureCalculator.c
