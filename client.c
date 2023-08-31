// Local client socket
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define INTERNET_PROTOCOL 0
#define MAXLINE 1024
#define LOCALHOST "127.0.0.1"

int init_client_socket(struct sockaddr_in serv_addr);
int connect_to_server(int client_fd, struct sockaddr_in serv_addr);

int init_client_socket(struct sockaddr_in serv_addr)
{
    int client_fd;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // Creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, LOCALHOST, &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    return client_fd;
}

int connect_to_server(int client_fd, struct sockaddr_in serv_addr)
{
    int status;
    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    // sent message variables
    int status;
    int valread;
    char *hello = "Client\t| Hello!";
    char buffer[MAXLINE] = {0};

    // server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // client socket
    int client_fd = init_client_socket(serv_addr);

    // connect to server
    status = connect_to_server(client_fd, serv_addr);

    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent by client\n");
    valread = read(client_fd, buffer, 1024);
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}
