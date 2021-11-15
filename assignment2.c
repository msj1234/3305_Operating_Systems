//CS3305A Assignment 2
//Msaidja
// October 8 2021

/*creating pipes in c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define size 30

//  to run program: gcc assignment2.c -o assignment2
//                 ./assignment2 "CS" "3305" " is fun!"
//
int main(int argc, char **argv){

        char buffer[size];
        //check if user enters X, Y or Z. if they dont exit
        if(argc != 4){
                printf("arguments invalid");
                exit(0);
        }
        //create pipe
        int port[2];
        pid_t pid;

        if(pipe(port) < 0){
                perror("Pipe error");
                exit(0);
        }
        //create fork 
        pid = fork();

        if(pid < 0){
                perror("fork error");
                exit(0);
        }

	//parent process
	if (pid> 0){
                printf("Pipe created successfully between parent (PID %d) and child\n", getpid());

                //read first command line argument
                char* X = argv[1];

                //forcing parent to stop so child can complete
                wait(NULL);


                char endStr[size] = "";
                strcat(endStr, X);
                strcat(endStr, buffer); //concat x with c

                write(port[0], endStr, size);

                char* Y = argv[2];
                char* Z = argv[3];
                strcat(endStr, Y);
                strcat(endStr, Z);
                printf("Parent (PID %d) reads concatenated result from the pipe (Z' = \"%s\")\n", getpid(), endStr);


        }
	 //child
        else{
                printf("Parent (PID %d) created a child (PID %d)\n", getppid(), getpid());
                printf("parent (PID %d) recieves X = \"%s\" from user \n", getppid(), argv[1]);
                printf("parent (PID %d) writes X = \"%s\" to the pipe \n", getppid(), argv[1]);

                char* Y = argv[2];
                char* Z = argv[3];

                printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", getpid(), Y, Z);

                
                 char concatedStr[size] = "";

		 //concate X
                 strcat(concatedStr,argv[1]);
		 //concate Y with Z
                 strcat(concatedStr,Y);
                 strcat(concatedStr,Z);

                 printf("child (PID %d) concatenates Y and Z to generate Y'= \"%s\"\n", getpid(), concatedStr);

                //read from pipe
                 read(port[1], concatedStr, sizeof(concatedStr));
                 printf("child (PID %d) reads X from pipe = \"%s\"\n", getpid(), argv[1]);


                 printf("child (PID %d) concatenates X and Y' to generate Z' = \"%s\"\n",getpid(), concatedStr);

                 printf("child (PID %d) writes Z' into the pipe\n", getpid());
        }

	return 0;
}
