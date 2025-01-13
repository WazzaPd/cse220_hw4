#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT 8080

int main (){

    char server_message[256] = "You have reached the server\n";
    
    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to a specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, 2);

    int client_socket;
    //second parameter will contain the client_socket information
    listen(server_socket, 2);
    while(1){

        client_socket = accept(server_socket, NULL, NULL);

        send(client_socket, server_message, 256, 0);
    }

    close(server_socket);

    return 0;
}