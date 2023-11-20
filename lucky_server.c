#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_QUESTION_SCORE 5

void handle_client(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_addr_len = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // Handle the client
        handle_client(client_socket);

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

void handle_client(int client_socket) {
    char buffer[1024];
    int score = 0;

    // Sample questions
    const char* questions[] = {
        "What is the capital of France? (1-5)\n",
        "In which year did World War II end? (1-5)\n",
        "How many continents are there? (1-5)\n",
        "What is the square root of 25? (1-5)\n"
    };

    // Ask four questions
    for (int i = 0; i < 4; ++i) {
        // Send the question
        send(client_socket, questions[i], strlen(questions[i]), 0);

        // Receive the answer
        recv(client_socket, buffer, sizeof(buffer), 0);
        int answer = atoi(buffer);

        // Validate the answer
        if (answer >= 1 && answer <= 5) {
            score += answer;
        } else {
            printf("Invalid answer received. Ignoring.\n");
        }
    }

    // Determine luck based on the total score
    if (score <= 8) {
        send(client_socket, "You have bad luck today.\n", 26, 0);
    } else if (score <= 12) {
        send(client_socket, "You're quite lucky.\n", 21, 0);
    } else {
        send(client_socket, "You're very lucky today.\n", 25, 0);
    }
}
