#CS4920 Port-Pirates project MakeFile
#Ryan Du Plooy and Murphy Schaff

dependencies:
	@echo "Installing all dependencies. Sudo password required."
	@echo "Installing tree...."
	@sudo apt install tree

clean:
	@echo "Cleaning Files...."
	@rm data/*.txt
	@rm data/passwords.txt
	@rm data/sudoers.txt
	@rm *.tar
	
	@echo "Creating default users for evilCalculator..."
	@echo "default" > users/passwords.txt
	@echo "default" >> users/passwords.txt
	
	@echo "murphy" > users/sudoers.txt
	@echo "hi" >> users/sudoers.txt

compile:
	@echo "Compiling Evil and Secure calculators..."
	@gcc -o calculator evilCalculator.c
	@gcc secureCalculator.c
