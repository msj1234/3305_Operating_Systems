#include <stdio.h>
#include <errno.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include<math.h>

// Msaidja
// C3305A assignment 3
// October 25 2021
//to execute gcc assignment3.c -lpthread ( or -pthread)

//function reads X and Y from pipe and then adds them
void *sum(void *thread_id){

        // sum x and Y

        int x, y, s;
        s = x+y;
       return NULL;
}

//function determies if its odd or even
void *odd_even(void *thread_id){

        int *s = (int*) thread_id;
        bool ans = false; //set flag to false
         if(*s % 2 == 0){
                 ans = true;
                 return (void*)ans; //number is even
         }else{
                 ans = false;
                 return (void*)ans;
         }
                 return 0;
}
//identifies how many digits are in the s
void *digit_count(void *thread_id){

        int *s = (int*) thread_id;
        long counter = 0;
        while(*s != 0){

                //determine how many digits are in s
                *s = *s/10;
                counter++;

        }
        return (void*) counter;
}

//main method
int main( int argc, char *argv[] )  {


        printf("Program name %s\n", argv[0]);
        //convert command line arguments to integers
        int arg1 = atoi(argv[1]); //ascii to int
        int arg2 = atoi(argv[2]);


   //create pipe for communication between parent process and threads
        int port[2]; //create pipe

        pid_t pid;

        //create fork
        pid = fork();

        if(pipe(port) <0){
                perror("pipe error");
                exit(0);
   }

        //parent process will read x and y from commnd window.

        if(pid>0){

                printf("Parent (PID %d) recieves X = %d and Y= %d from user\n",getpid(), arg1, arg2);
                //create threads
                pthread_t thread1, thread2, thread3;

                int thread_id_1 = 100;
                int thread_id_2 = 101;
                int thread_id_3 = 102;

                //create thread 100 (thread 2 and 3 are created later on)
                pthread_create(&thread1, NULL, sum, &thread_id_1);

          //write X and Y to pipe

                write(port[1], &argv[1], sizeof(arg1));
                write(port[1], &argv[2], sizeof(arg2));
                printf("parent (PID %d) writes X = %d and Y = %d to the pipe\n", getpid(), arg1, arg2);


                //thread 100 reading from pipe
                read(port[0], &argv[1], sizeof(arg1));
                read(port[0], &argv[2], sizeof(arg2));
                printf("thread (TID %d) reads X= %d and Y = %d from the pipe\n", thread_id_1, arg1, arg2);

		//thread 100 writing into pipe
                int s = arg1 + arg2;
                write(port[1], &s, sizeof(s));
                printf("thread (TID %d) writes X + Y = %d to the pipe\n", thread_id_1, s);

                //create Thread 101
                pthread_create(&thread2, NULL, odd_even, &thread_id_2);
                //read from pipe
                read(port[0], &s, sizeof(s));
                printf("thread (TID %d) reads X + Y = %d to the pipe\n", thread_id_2, s);

                if(!odd_even(&s))
                        printf("thread (TID %d) identifies X + Y = %d as an odd number\n", thread_id_2, s);
                else
                        printf("thread (TID %d) identifies X + Y = %d as an even number\n", thread_id_2, s);

                //write into the pipe
                write(port[1], &s, sizeof(s));
                printf("thread (TID %d) writes X + Y = %d to the pipe\n", thread_id_2, s);

                //create thread 102
                pthread_create(&thread3, NULL, digit_count, &thread_id_3);
                //read to pipe
                read(port[0], &s, sizeof(s));
                printf("thread (TID %d) reads X + Y = %d to the pipe\n", thread_id_3, s);


                //digit count
                int *count = 0;
                int newS = s;
                count = digit_count(&newS);

                printf("thread (TID %d) identifies X + Y = %d as a %d digit number\n", thread_id_2, s, count);


                pthread_join(thread1, NULL);
                pthread_join(thread2, NULL);
                pthread_join(thread3, NULL);
        }

        //child process
        if(pid == 0){}
}

