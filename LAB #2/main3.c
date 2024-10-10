#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main() {
    pid_t pid1, pid2;
    int status;
    int i;  // Declare the loop variable outside the loop
    
    // Seed the random number generator
    srand(time(NULL));

    // Fork the first child process
    pid1 = fork();
    
    if (pid1 == -1) {
        perror("Failed to fork first child");
        return 1;
    }
    
    if (pid1 == 0) {
        // First child process
        int iterations = rand() % 30;  // Random number of iterations up to 30
        for (i = 0; i < iterations; i++) {  
            int sleep_time = rand() % 10;  // Random sleep time up to 10 seconds
            printf("Child Pid: %d is going to sleep for %d seconds!\n", getpid(), sleep_time);
            sleep(sleep_time);
            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
        }
        printf("Child Pid: %d has completed\n", getpid());
        exit(0);  // Ensure child process exits after finishing the loop
    }
    
    // Fork the second child process
    pid2 = fork();
    
    if (pid2 == -1) {
        perror("Failed to fork second child");
        return 1;
    }
    
    if (pid2 == 0) {
        // Second child process
        int iterations = rand() % 30;  // Random number of iterations up to 30
        for (i = 0; i < iterations; i++) {  
            int sleep_time = rand() % 10;  // Random sleep time up to 10 seconds
            printf("Child Pid: %d is going to sleep for %d seconds!\n", getpid(), sleep_time);
            sleep(sleep_time);
            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
        }
        printf("Child Pid: %d has completed\n", getpid());
        exit(0);  // Ensure child process exits after finishing the loop
    }

    // Parent process waits for both child processes to complete
    pid_t terminated_pid;
    while ((terminated_pid = wait(&status)) > 0) {
        printf("Child Pid: %d has completed\n", terminated_pid);
    }

    return 0;
}
