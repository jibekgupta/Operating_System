#include <netinet/in.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
  
#define PORT 9001
#define MAX_COMMAND_LINE_LEN 1024
#define SERVER_IP "127.0.0.1"

static int global_sockID = -1;

void handle_signal(int sig) {
  printf("\nReceived signal %d. Cleaning up...\n", sig);
  if (global_sockID != -1) {
    const char *exit_cmd = "exit";
    send(global_sockID, exit_cmd, strlen(exit_cmd), 0);
    close(global_sockID);
  }
  exit(EXIT_SUCCESS);
}

char* getCommandLine(char *command_line) {
    if (!command_line) {
        return NULL;
    }

    do {
        memset(command_line, 0, MAX_COMMAND_LINE_LEN);

        if (fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) {
            perror("Error reading input");
            return NULL;
        }

        size_t len = strlen(command_line);
        if (len > 0 && command_line[len - 1] == '\n') {
            command_line[len - 1] = '\0';
        }
    } while (command_line[0] == '\n');

    return command_line;
}

void display_menu() {
  printf("COMMANDS:\n---------\n");
  printf("1. print - Print the linked list\n"); 
  printf("2. get_length - Get the length of the linked list\n");
  printf("3. add_back <val> - Add a value to the back of the linked list\n");
  printf("4. add_front <val> - Add a value to the front of the linked list\n");
  printf("5. add_position <idx> <value> - Add a value at a specific index\n"); 
  printf("6. remove_back - Remove the last element of the linked list\n");
  printf("7. remove_front - Remove the first element of the linked list\n");
  printf("8. remove_position <index> - Remove an element at a specific index\n");
  printf("9. get <index> - Get the value at a specific index\n");
  printf("10. exit - Exit the client\n");
  printf("\n");
}

int main(int argc, char const* argv[]) {
  struct sockaddr_in servAddr;
  char buf[MAX_COMMAND_LINE_LEN];
  char responseData[MAX_COMMAND_LINE_LEN];
  char *token, *cp = NULL;

  // Set up signal handlers
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  // Create socket
  int sockID = socket(AF_INET, SOCK_STREAM, 0); 
  global_sockID = sockID;  // Set global socket for signal handler
  if (sockID < 0) {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET; 
  servAddr.sin_port = htons(PORT); 
  
  // Convert IP address from string to binary form 
  if (inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr) <= 0) {
    perror("Invalid address/Address not supported");
    close(sockID);
    exit(EXIT_FAILURE);
  }

  // Connect to server
  if (connect(sockID, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
    perror("Connection failed");
    close(sockID);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server at %s:%d\n", SERVER_IP, PORT);
  display_menu();
    
  while(1) {
    printf("\nEnter Command (or menu): ");
    
    if (getCommandLine(buf) == NULL) {
      printf("\nError reading command or EOF encountered\n");
      break;
    }

    // Skip empty commands
    if (strlen(buf) == 0) {
      continue;
    }

    // Handle menu command locally
    if(strcmp(buf,"menu") == 0){
      display_menu();
      continue;
    }
      
    // Send command to server
    if (send(sockID, buf, strlen(buf), 0) == -1) {
      perror("Error sending data to server");
      break;
    }

    // Parse command for local processing
    cp = strdup(buf);
    if (!cp) {
      perror("Memory allocation failed");
      break;
    }

    token = strtok(cp, " ");
    if (strcmp(token, "exit") == 0) {
      free(cp);
      printf("Exiting...\n");
      break;
    }

    // Receive server response
    memset(responseData, 0, MAX_COMMAND_LINE_LEN);
    ssize_t received = recv(sockID, responseData, sizeof(responseData) -1, 0);

    if (received > 0) {
      responseData[received] = '\0'; // Ensure null termination
      printf("SERVER RESPONSE: %s\n", responseData);
    } else if (received == 0) {
      printf("Server disconnected\n");
      free(cp);
      break;
    } else {
      perror("Error receiving data from server");
      free(cp);
      break;
    }

    free(cp);
    cp = NULL;
  }

  // Close socket before exiting 
  close(sockID); 
  return 0; 
}