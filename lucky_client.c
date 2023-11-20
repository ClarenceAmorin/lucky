#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Receive and display questions, send answers
    for (int i = 0; i < 4; ++i) {
        // Receive and display the question
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s", buffer);

        // Get user input and send the answer
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Receive and display luck determination
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    // Close the socket
    close(client_socket);

    return 0;
}
