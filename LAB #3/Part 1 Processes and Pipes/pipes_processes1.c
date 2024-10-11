// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str1[] = "howard.edu";
    char fixed_str2[] = "gobison.org";
    char input_str[100];
    char concat_str[200]; 
    pid_t p; 
  
    // Create first pipe
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe 1 Failed" ); 
        return 1; 
    } 

    // Create second pipe
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe 2 Failed" ); 
        return 1; 
    } 

    
    p = fork(); // Create child process 
     
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process (P1)
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);

        // Step 1: take input from the user and send to P2
        printf("Input: ");
        scanf("%s", input_str);
        
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);
        
  
        // Wait for child to send a string 
        wait(NULL); 
  
        read(fd2[0], concat_str, sizeof(concat_str)); 
        close(fd2[0]);

        // print the concatenated string
        printf("Concatenated string : %s\n", concat_str);

        // prompt for the second input in P1
        char second_input[100];
        printf("Input : ");
        scanf("%s", second_input);

        // concatenated with "gobison.org" and print the result
        strcat(concat_str, fixed_str2);
        printf("Concatenated string: %s\n", concat_str);
        
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe  
        read(fd1[0], concat_str, sizeof(concat_str)); 
        close(fd1[0]);
  
        // Concatenate a fixed string with it 
        strcat(concat_str, fixed_str1);
        

        write(fd2[1], concat_str, strlen(concat_str)+1);
        close(fd2[1]);

        exit(0); 
    }

    return 0; 
} 