#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define INTERNET_PROTOCOL 0

int init_serv_socket()
{
    int opt = 1; // socket option
    int server_fd;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | 0, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt"); // print error message
        exit(EXIT_FAILURE);   // exit with failure
    }

    return server_fd;
}

void bind_to_port(int server_fd, struct sockaddr_in client_addr)
{
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
        perror("bind failed"); // print error message
        exit(EXIT_FAILURE);    // exit with failure
    }
}

void listen_for_connections(int server_fd)
{
    // listen for connections on a socket
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");   // print error message
        exit(EXIT_FAILURE); // exit with failure
    }
}

int accept_connection(int server_fd, struct sockaddr_in client_addr)
{
    int new_socket;
    int addrlen = sizeof(client_addr); // socket address length
    // accept a connection on a socket
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");   // print error message
        exit(EXIT_FAILURE); // exit with failure
    }
    return new_socket;
}

int main(int argc, char const *argv[])
{
    int server_fd = init_serv_socket(); // socket file descriptor
    int new_socket;                     // socket file descriptor
    int valread;                        // read return value

    struct sockaddr_in client_addr;           // socket address: family, port, address
    client_addr.sin_family = AF_INET;         // IPv4
    client_addr.sin_addr.s_addr = INADDR_ANY; // localhost
    client_addr.sin_port = htons(PORT);       // port 8080

    // sent message variable
    int addrlen = sizeof(client_addr); // socket address length
    char buffer[1024] = {0};           // buffer for read
    char *hello = "Server\t| Hello back!";

    bind_to_port(server_fd, client_addr); // bind to port 8080
    listen_for_connections(server_fd);    // listen for connections on a socket
    printf("Listening on port %d\n", PORT);

    new_socket = accept_connection(server_fd, client_addr); // accept a connection on a socket
    printf("Connection accepted\n");

    // read from a file descriptor
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);                    // print buffer
    send(new_socket, hello, strlen(hello), 0); // send hello message
    printf("Hello message sent by server\n");  // print message

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
