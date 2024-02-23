#include <iostream> // for standard I/O
#include <string> // for string manipulation
#include <list> // for doubly-linked list
#include <sys/fcntl.h> // for file control options
#include <sys/types.h> // for various types used in system calls
#include <sys/socket.h> // for socket system calls
#include <netinet/in.h> // for internet address family
#include <arpa/inet.h> // for internet address to numeric conversion
#include <fcntl.h> // for file control options
#include <errno.h> // for errno
#include <unistd.h> // for various utility functions
#include "poll.h" // custom poll function header
#include <stdio.h> // for standard I/O
#include <stdlib.h> // for standard lib functions
#include <string.h> // for string functions
#include <vector> // for vector

#include "Exceptions.hpp" // custom exceptions header
#include "config_structs.hpp" // custom config structs header
#include "parser.hpp" // custom parser header
#include "utils.hpp" // custom utils header

// Server constants
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF

class server {
public:
    std::vector<pollfd> poll_set; // pollfd vector to store file descriptors and events
    struct sockaddr_in server_addr; // socket address structure for the server
    int nfds; // number of file descriptors
    int server_fd; // server file descriptor

    server() {};
    server(int port);
    server(std::string path);
    ~server(){};
    void run(); // method to run the server
};

// Constructor for server class with port parameter
server::server(int port) {
    nfds = 0;
    poll_set.reserve(100); // reserve memory for vector

    // Initialize socket address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Create server file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(server_fd, F_SETFL, O_NONBLOCK); // Set non-blocking mode
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr)); // Allow reuse of address
    bzero(&(server_addr.sin_zero), 8); // Zero out unused parts of the structure

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Start listening for incoming connections
    listen(server_fd, 100);

    // Initialize pollfd structure for server and add it to the poll_set vector
    pollfd serv;
    serv.fd = server_fd;
    serv.events = POLLIN | POLLHUP | POLLERR;
    serv.revents = 0;
    poll_set.push_back(serv);
    nfds = 1;
}

// Constructor for server class with file path parameter
server::server(std::string path) {}

// Method to run the server
void server::run() {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Wait for events on file descriptors
    poll(poll_set.data(), poll_set.size(), 100);

    // Iterate through the poll_set vector
    for (std::vector<pollfd>::iterator it = poll_set.begin(); it != poll_set.end(); it++) {
        if (it->revents & POLLIN) {
            if (it->fd == server_fd) {
                // Accept new connection
                client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
                if (client_fd < 0) {
                    perror("accept");
                    exit(1);
                }
                fcntl(client_fd, F_SETFL, O_NONBLOCK); // Set non-blocking mode

                // Initialize pollfd structure for client and add it to the poll_set vector
                pollfd client;
                client.fd = client_fd;
                client.events = POLLIN | POLLHUP | POLLERR;
                client.revents = 0;
                poll_set.push_back(client);
                nfds++;
            } else {
                // Handle data from client
                char buf[BUF_SIZE];
                int ret = recv(it->fd, buf, BUF_SIZE, 0);
                std::cout << buf << std::endl;
                if (ret < 0) {
                    perror("recv");
                    exit(1);
                } else if (ret == 0) {
                    // Connection closed by client
                    close(it->fd);
                    poll_set.erase(it);
                    nfds--;
                } else {
                    // Send HTTP response
                    const char *http_response = "HTTP/1.1 200 OK\r\n"
                                                "Content-Type: text/html\r\n"
                                                "Content-Length: 13\r\n"
                                                "\r\n"
                                                "Hello World!";
                    send(it->fd, http_response, ret, 0);
                    // Close connection if content is full
                    close(it->fd);
                    it
