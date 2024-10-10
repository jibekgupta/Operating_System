#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t alarm_count = 0; // To count SIGALRM signals
volatile sig_atomic_t running = 1;      // To control the main loop

// Function prototypes
void handle_alarm(int signum); 
void handle_sigint(int signum); 

void handle_alarm(int signum) { // Signal handler for SIGALRM
    alarm_count++; // Increment the count of alarms
    printf("Hello World!\n");
}

void handle_sigint(int signum) { // Signal handler for SIGINT (CTRL+C)
    running = 0; // Stop the main loop
}

int main() {
    // Register signal handlers
    signal(SIGALRM, handle_alarm); // Handle SIGALRM
    signal(SIGINT, handle_sigint);  // Handle SIGINT

    // Schedule a SIGALRM for 1 second
    alarm(1);

    // Main loop
    while (running) {
        pause(); // Wait for signals
        if (alarm_count > 0) {
            printf("Turing was right!\n");
            alarm_count--; // Reset the count after printing
            alarm(1);      // Re-set the alarm for the next second
        }
    }

    // Print total time executed (number of alarms)
    printf("Total time executed: %d seconds\n", alarm_count);
    return 0; // Clean exit
}
