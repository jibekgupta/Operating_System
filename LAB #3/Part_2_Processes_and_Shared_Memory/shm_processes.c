#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void ParentProcess(int []);
void ChildProcess(int []);

int main(void) {
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error ***\n");
        exit(1);
    }
    printf("Process has received a shared memory of two integers...\n");

    ShmPTR = (int *) shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1) {
        printf("*** shmat error ***\n");
        exit(1);
    }
    printf("Process has attached the shared memory...\n");

    // Initialize BankAccount and Turn
    ShmPTR[0] = 0;    // BankAccount
    ShmPTR[1] = 0;    // Turn
    printf("Orig Bank Account = %d\n", ShmPTR[0]);

    // Seed random number generator with time
    srand(time(NULL));

    pid = fork();
    if (pid < 0) {
        printf("*** fork error ***\n");
        exit(1);
    }
    else if (pid == 0) {
        ChildProcess(ShmPTR);
        exit(0);
    }
    else {
        ParentProcess(ShmPTR);
    }

    wait(&status);
    
    shmdt((void *) ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);
    
    return 0;
}

void ParentProcess(int SharedMem[]) {
    int account, balance;
    int i;
    for(i = 0; i < 25; i++) {
        // Sleep random time (0-5 seconds)
        sleep(rand() % 6);
        
        while(SharedMem[1] != 0);  // Wait for turn
        
        account = SharedMem[0];
        
        if(account <= 100) {
            // Generate random amount between 0-100
            balance = (rand() % 51) * 2;  // Even numbers 0-100
            
            if(balance > 0) {
                account += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", 
                       balance, account);
            } else {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }
        } else {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", 
                   account);
        }
        
        SharedMem[0] = account;
        SharedMem[1] = 1;  // Child's turn
    }
}

void ChildProcess(int SharedMem[]) {
    int account, balance;
    int i;
    for(i = 0; i < 25; i++) {
        // Sleep random time (0-5 seconds)
        sleep(rand() % 6);
        
        while(SharedMem[1] != 1);  // Wait for turn
        
        account = SharedMem[0];
        
        // Generate random amount needed between 1-50
        balance = rand() % 50 + 1;
        printf("Poor Student needs $%d\n", balance);
        
        if(balance <= account) {
            account -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", 
                   balance, account);
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }
        
        SharedMem[0] = account;
        SharedMem[1] = 0;  // Parent's turn
    }
}