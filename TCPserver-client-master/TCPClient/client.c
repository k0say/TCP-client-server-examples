#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

main(int argc, char const *argv[])  {
    
    // create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    puts("Socket created");

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status == -1) {
        printf("There was an error making a connection to the remote socket \n\n");
    }
    puts("Connected\n");

    char message[1000], server_response[256];;

    // keep communicating with server
    while(1)    {
        printf("Enter Message: ");
        scanf("%s", message);

        // send some data
        if(send(network_socket, message, strlen(message), 0) < 0)   {
            puts("Send failed");
            return 1;
        }

        // receive a reply from the server
        if( recv(network_socket , server_response , sizeof(server_response) , 0) < 0)   {
			puts("recv failed");
			break;
		}
        puts("Server reply: ");
        puts(server_response);
    }

    // recieve data from the server
    //char server_response[256];
    //recv(network_socket, &server_response, sizeof(server_response), 0);

    // print out the server's response
    //printf("The server sent the data: %s\n", server_response);

    // and then close the socket
    close(network_socket);
    return 0;
}
