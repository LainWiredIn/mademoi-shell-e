# README

# mademoi-shell-e

---
## <u>Objective</u>

* The objective of this project is to implement a fully-functional "Shell", that mimics the behaviour of the Linux-based terminal. In other words, the objective is to create a user-defined interactive shell program that can create and manage new processes. The project has been coded in the "C Programming Language" and due to her elegance(and her fair share of pride), has been very aptly named, <i>"mademoishelle"</i>.

---
## <u>Files Included</u>

### 1. makefile

* Used to compile the entire project using a single command, "make".
* Includes details for compiling all the code (with the option to add appropriate flags and linker options).
* Other than the "-g" flag for debugging, no other flags have been added by default. If required, they can be added by curious users.

### 2. main.c

* The driver code. 
* This is where execution for the program starts. Includes the main function and the "Shelldon" function: which is used to take newline separated inputs and print the shell prompt.
* Also defines the global variables.
* Appropriate Signal handlers for shell present here.

### 3. execute.c

* Takes the newline-separated strings as inputs, parses them, and calls the appropriate sub-routine for executing user commands.
* The "repeat" functionality has been implemented directly inside the execute function.
* It is responsible for taking care of redirection and calling the appropriate routines.

### 4. cd.c

* Contains the "change_directory" function, which is used to change the directory. 
* Supports the flags: “.”, “..”, “-” and “~”, apart from the usual functionality of changing directory.

### 5. echo.c

* Contains the "echo_echo" function, which takes a string as input and prints it on the terminal after appropriate formatting.
* All tabs and spaces have been replaced by a single space.

### 6. pwd.c

* Contains the "present_working_directory" function, which is used to retrieve the present working directory for various purposes.

### 7. ls.c

* Contains the "list_dir" and "ls" functions which are used to list the files in input directory. If directory is not specified, then files of the current working directory are listed.
* The "list_dir" function is used to further parse the string to get information about the flags mentioned, as well as all the directories mentioned.
* The "ls" function is used to list the files.
* The flags "-a", "-l", "-al", "-la" have all been implemented.
* The "total" of ls -l is printed at the bottom.

### 8. prompt.c

* Used the print the prompt on the terminal to satisfy the display requirement of the shell.
* Calls the "print_path" function for getting appropriately parsed path.

### 9. path.c

* Contains the "print_path" function used for getting the appropriate path of the working directory.
* Can be used by other functions for priting path as well.

### 10. syscom.c

* Contains the function used to run the system commands.
* Makes use of the execvp() command to replace the current process image with a new process image.
* execvp finds the program from its name, and also allows for taking an array of arguments.
* Depending on the presence of "&", new processes are run in foreground and background.

### 11. pinfo.c

* Used to print the process information on the terminal. 
* Checks the /proc/[pid]/stat and /proc/[pid]/exe paths for appropriate information.
* There was a confusion regarding process status codes with "+", whether to compare the 0th and 7th index stat entries or the 4th and 7th. More information is required. Submitted implementation is with 4th and 7th.

### 12. history.c

* Used to store upto 20 non-redundant newline-separated commands in a queue.
* Contains appropriate functions for implementing the queue and "display"-ing the 10 recent commands.
* Also takes numerical argument for user specified number of commands.
* It doesn't store the commands across sessions. This however, can be easily implemented and instructions are present as comments in main.c.

### 13. README<area>.md

* You are reading it right now!

### 14. pipe.c

* Used for implementing pipe functionality.
* Has its very own pipe_execute function for executing commands.

### 15. jobs.c

* Used to implement the user-defined "jobs" command.
* "-r", "-s", "-rs", "-sr" flags have been implemented.

### 16. fgbg.c

* Used to implement the "fg \<job number\>" and "bg \<job number\>" user defined commands.
* Different functions for both have been implemented.

### 17. sig.c

* Used to implement the "sig \<job number\> \<signal number\>" user-defined command.
* Throws an error if no job of that number exists.


### Other files

* Appropriate header files have been included for declaring global variables and to speed up the compilation & execution process.
---

## Usage(How to compile and run)

* Before compiling, make sure that you are using a Linux-based Operating System, like Ubuntu or Debian. Also, make sure that you have the "gcc compiler" installed on your machine(look into build-essential if you are on Linux).

* For compiling the program, open the terminal. Make sure you are in the directory called "2021121003"(Well, you can also compile from outside it but why the added hassle?). Then enter the following command - 
```
make
``` 
Yup, that's it. 

* An output file by the name of "mademoishelle" is created in the same directory. To run it, enter the following command - 
```
./mademoishelle
``` 
This will start the execution of the <i>"mademoishelle"</i> program.

---

## <u>Assumptions made</u>
- mademoishelle does NOT handle background processing for built-in commands (ls, echo, cd, pwd, jobs, etc).
- Arguments to system commands are not longer than 1024 bytes.
- "&" appears at the end of the system commands' arguments to signal the creation of background process.
- "Process ID" is only printed for the background processes. If the process is to be run in the foreground, PID is not printed on the terminal.
- The shell has a way of deciphering between files and commands: We assume that the commands passed to SHELLDON are either Built-In Commands, or are existing files or non-existing files.
- It is assumed that a positive intger is always provided as the first argument to "repeat".
- "getenv("PWD")" has also been used at one place, since its use was not prohibited.
- ls -l prints the year for all files.
- Only one input redirection and one output redirection is allowed per command.
- 1024 bytes is the size limit of the filename.
- ">>" appends with a newline.
- Redirection symbols and input/output filenames should be separated by spaces
- THIS WORKS : cat < inp_params.txt | ./sort.o | cat > 1.txt
- It is also fairly assumed that system commands have only one argument.
- Redirection commands cannot be in background, i.e., no "&" after \< or \>.
- "total" is printed in the end for ls -l, which counts the sum of the size(in bytes) for each of the lsited files and then divides that number by 512(to give number of 512-byte size blocks).
- Our shell essentially runs on top of the usual shell as a foreground process. Then it can either give up control and run another foreground process, or not give up control and spawn background processes.
- Maximum number of background processes stored in jobs = 64.
- fg and bg only consider first entered number to be the job id, subsequent numbers entered(if any) are ignored.
- Stressing on the fact that user defined commands cannot be run in the background and cannot be pushed to the background either.
- R, S and D states are considered "Running".
- Background processing in pipes is discouraged.
- Command length is limited to 1024 bytes for system commands.
- Path printing is done using "PATH_MAX" as the limit.

---

## <u>Authors</u>
Made with love by Manav Chaudhary. <br> For further support, you can contact yours truly on the following mail id: manav.chaudhary@research.iiit.ac.in.

