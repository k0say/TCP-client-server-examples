#include <sys/socket.h> //per socket(), bind()  connect()
#include <unistd.h> // per close()
#include <arpa/inet.h> // per sockaddrin
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 512
#define PROTOPORT 27015

void errorhandler(char *errorMessage)   {
    printf("%s", errorMessage);
}

int main(void)
{
    // SOCKET CREATION
    int c_socket;
    c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_socket < 0)
    {
        errorhandler("socket creation failer.\n");
        close(c_socket);
        return -1;
    }

    // CREATE SERVER ADDRESS
    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // server ip
    sad.sin_port = htons(PROTOPORT); // server port

    // CONNECT TO SERVER
    if(connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0)  {
        errorhandler("Failed to connect.\n");
        close(c_socket);
        return -1;
    }

    char* input_string = "prova";
    int string_len = strlen(input_string);

    // SEND DATA TO SERVER
    if(send(c_socket, input_string, string_len, 0) != string_len)   {
        errorhandler("send() sent a different number of bytes than expected\n");
        close(c_socket);
        return -1;
    }

    // RECEIVE DATA FROM SERVER
    int bytes_rcvd;
    int total_bytes_rcvd = 0;
    char buf[BUFFERSIZE];
    printf("Received: ");

    while(total_bytes_rcvd < string_len)    {
        if((bytes_rcvd = recv(c_socket, buf, BUFFERSIZE -1, 0)) <= 0)   {
            errorhandler("recv() failed or connection closed prematurely\n");
            close(c_socket);
            return -1;
        }
        total_bytes_rcvd += bytes_rcvd;
        buf[bytes_rcvd] = '\0'; // add \0 so printf knows where to stop
        printf("%s", buf);      // print the echo buffer
    }

    // CLOSE THE CONNECTION
    close(c_socket);
    printf("\n");
    system("pause");
    
    return 0;
}
