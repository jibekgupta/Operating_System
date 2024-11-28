#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "list.h"
#include <arpa/inet.h>

#define PORT 9001
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define ACK "ACK"

// Global variables for cleanup
static int server_socket = -1;
static int client_socket = -1;
static list_t *global_list = NULL;

// Cleanup function for graceful shutdown
void cleanup() {
  if (client_socket != -1) {
    close(client_socket);
    client_socket = -1;
  }
  if (server_socket != -1) {
    close(server_socket);
    server_socket = -1;
  }
  if (global_list != NULL) {
    list_free(global_list);
    global_list = NULL;
  }
}

// Signal handler to gracefully terminate
void handle_signal(int sig) {
  printf("\nReceived signal %d. Shutting down server...\n", sig);
  cleanup();
  exit(EXIT_SUCCESS);
}

// Function to safely send response to client
int send_response(int socket, const char *response) {
  size_t len = strlen(response);
  ssize_t sent = send(socket, response, len, 0);
  if (sent < 0) {
    perror("Error sending response");
    return -1;
  }
  return 0;
}

// Process client command and return response
void process_command(char *command, list_t *list, char *response, size_t response_size) {
  char *token = strtok(command, " ");
  int val, idx;

  if (!token) {
    snprintf(response, response_size, "Invalid command");
    return;
  }

  if (strcmp(token, "exit") == 0) {
    snprintf(response, response_size, "Server shutting down...");
    return;
  }
  else if (strcmp(token, "print") == 0) {
    // Convert list to string
    char *list_str = listToString(list);
    if (list_str) {
      snprintf(response, response_size, "%s", list_str);
      free(list_str);  // Free dynamically allocated string
    } else {
      snprintf(response, response_size, "Error: Cannot print list");
    }
  }
  else if (strcmp(token, "get_length") == 0) {
    int length = list_length(list);
    snprintf(response, response_size, "%d", length);
  }
  else if (strcmp(token, "add_back") == 0) {
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Value required for add_back");
      return;
    }
    val = atoi(token);
    if (list_add_to_back(list, val) == 0) {
      snprintf(response, response_size, "%s", ACK);
    } else {
      snprintf(response, response_size, "Error adding to back");
    }
  }
  else if (strcmp(token, "add_front") == 0) {
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Value required for add_front");
      return;
    }
    val = atoi(token);
    if (list_add_to_front(list, val) == 0) {
      snprintf(response, response_size, "%s", ACK);
    } else {
      snprintf(response, response_size, "Error adding to front");
    }
  }
  else if (strcmp(token, "add_position") == 0) {
    // Parse index and value
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Index required for add_position");
      return;
    }
    idx = atoi(token);
    
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Value required for add_position");
      return;
    }
    val = atoi(token);
    
    if (list_add_at_index(list, val, idx) == 0) {
      snprintf(response, response_size, "%s", ACK);
    } else {
      snprintf(response, response_size, "Error adding at index");
    }
  }
  else if (strcmp(token, "remove_back") == 0) {
    int removed_val;
    if (list_remove_from_back(list, &removed_val) == 0) {
      snprintf(response, response_size, "%d", removed_val);
    } else {
      snprintf(response, response_size, "Error removing from back");
    }
  }
  else if (strcmp(token, "remove_front") == 0) {
    int removed_val;
    if (list_remove_from_front(list, &removed_val) == 0) {
      snprintf(response, response_size, "%d", removed_val);
    } else {
      snprintf(response, response_size, "Error removing from front");
    }
  }
  else if (strcmp(token, "remove_position") == 0) {
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Index required for remove_position");
      return;
    }
    idx = atoi(token);
    
    int removed_val;
    if (list_remove_at_index(list, &removed_val, idx) == 0) {
      snprintf(response, response_size, "%d", removed_val);
    } else {
      snprintf(response, response_size, "Error removing at index");
    }
  }
  else if (strcmp(token, "get") == 0) {
    token = strtok(NULL, " ");
    if (!token) {
      snprintf(response, response_size, "Error: Index required for get");
      return;
    }
    idx = atoi(token);
    
    int value;
    if (list_get_elem_at(list, &value, idx) == 0) {
      snprintf(response, response_size, "%d", value);
    } else {
      snprintf(response, response_size, "Error getting element");
    }
  }
  else {
    snprintf(response, response_size, "Unknown command");
  }
}

int main(int argc, char const* argv[]) {
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  char buffer[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  // Set up signal handlers
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  // Create server socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options for reuse
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }
    
  // Configure server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
    
  // Bind socket to the specified IP and port 
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // Listen for connections 
  if (listen(server_socket, MAX_CLIENTS) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", PORT);

  while (1) {
    // Initialize linked list for each client connection
    global_list = list_alloc();
    if (!global_list) {
      perror("Failed to allocate list");
      continue;
    }

    // Accept client connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
      perror("Accept failed");
      list_free(global_list);
      continue;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Handle client commands
    while (1) {
      memset(buffer, 0, BUFFER_SIZE);
      memset(response, 0, BUFFER_SIZE);
            
      ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
      if (bytes_received <= 0) {
        if (bytes_received == 0) {
          printf("Client disconnected\n");
        } else {
          perror("Receive failed");
        }
        break;
      }
            
      buffer[bytes_received] = '\0';
      printf("Received command: %s\n", buffer);
            
      // Process command and send response
      process_command(buffer, global_list, response, BUFFER_SIZE);
      if (send_response(client_socket, response) < 0) {
        break;
      }
            
      // Check if exit command received
      if (strncmp(buffer, "exit", 4) == 0) {
        printf("Exit command received. Shutting down...\n");
        cleanup();
        exit(EXIT_SUCCESS);
      }
    }
        
    // Close client socket and free list
    close(client_socket);
    list_free(global_list);
    client_socket = -1;
    global_list = NULL;
  }
    
  cleanup();
  return 0;
}