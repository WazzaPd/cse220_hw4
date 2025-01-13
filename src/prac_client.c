#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT 8080

int main (){

    //create socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //connect to server
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    if(connection_status == -1){
        printf("There was an error making a connection to the remote socket\n");
    }

    // recieve data from the server
    char server_response[256];
    recv(network_socket, &server_response, 256, 0);

    //print out the server's response
    printf("%s", server_response);

    close(network_socket);

    return 0;
}