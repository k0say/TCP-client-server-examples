#include <sys/socket.h> //per socket(), bind()  connect()
#include <unistd.h> // per close()
#include <arpa/inet.h> // per sockaddrin
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PROTOPORT 27015
#define QLEN 6

void errorhandler(char *errorMessage)   {
    printf("%s", errorMessage);
}

int main()
{
    int port;

    if(argc > 1)    {
        port = atoi(argv[1]);
    }
    else
        port = PROTOPORT;
    if(port < 0)    {
        printf("bad port number %s \n", argv[1]);
        return 0;
    }

    // SOCKET CREATION
    int my_socket;
    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (my_socket < 0)
    {
        errorhandler("socket creation failer.\n");
        return -1;
    }

    // SET ADDRESS IN THE SOCKET
    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sad.sin_port = htons(port);

    if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0)
    {
        errorhandler("bind() failed.\n");
        close(my_socket);
        return -1;
    }

    // SET LISTENING SOCKET
    if (listen(my_socket,QLEN) < 0) {
        errorhandler("listen() failed.\n");
        close(my_socket);
        return -1;
    }

    // ACCEPT NEW CONNECTION
    struct sockaddr_in cad; // structures for the clien address
    int client_socket;
    int client_len;

    printf("Waiting for a client to connect...");

    while(1)    {
        client_len = sizeof(cad);
        if((client_socket = accept(my_socket, (struct sockaddr*)&cad, &client_len)) < 0)    {
            errorhandler("accept() failed.\n");
            close(my_socket);
            //return 0;
            return -1;
        }
        printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
    }
}
