#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()  {
    char server_message[256] = {0};// = "You have reached the server!";

    int read_size;
    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    // define the server address
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP and port
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)    {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

    listen(server_socket, 6);

    //Accept and incoming connection
	puts("Waiting for incoming connections...");
	int c = sizeof(struct sockaddr_in);

    int client_socket;
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t*)&c);
    if (client_socket < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
while(1)    {
    // receive a message from a client
    while((read_size = recv(client_socket, server_message, 256, 0)) > 0)    {
        // send the message back to client 
        write(client_socket, server_message, strlen(server_message));
    }
}
    if(read_size == 0)  {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)    {
        perror("recv failed");
    }

    // send the message
    send(client_socket, server_message, sizeof(server_message), 0);

    // close the socket
    close(server_socket);
    close(client_socket);
    return 0;
}
