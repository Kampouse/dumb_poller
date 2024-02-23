#include "server.hpp"    // Include the server header file
#include <sstream>       // Include stringstream library
#include <filesystem>    // Include filesystem library

// Constructor for the server class
server::server(server_info servInfo)
{
    // Set the server address information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(servInfo.port);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Initialize class variables
    serveInfo = servInfo;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr));
    bzero(&(server_addr.sin_zero), 8);

    // Bind the server socket to the specified address and port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        return;
    }

    // Start listening for incoming connections
    listen(server_fd, 100);

    // Initialize the pollfd structure for the server socket
    pollfd serv;
    serv.fd = server_fd;
    serv.events = POLLIN | POLLHUP | POLLERR;
    serv.revents = 0;

    // Add the server socket to the poll_set vector
    poll_set.push_back(serv);

    // Initialize the contents vector with supported file extensions
    contents.push_back(".css");
    contents.push_back(".html");
    contents.push_back(".js");
    contents.push_back(".png");
    contents.push_back(".jpg");
    contents.push_back(".jpeg");
    contents.push_back(".gif");
    contents.push_back(".ico");
}

// Function to remove a socket from the poll_set vector
void   server::clear_fd (int i)
{
    close(poll_set[i].fd);
    poll_set[i].fd = -1;
    poll_set.erase(poll_set.begin() + i);
}

// Function to add a new client socket to the poll_set vector
void server::add_client (void)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd;
    pollfd client;

    // Accept a new connection and get the client socket descriptor
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
    {
        perror("accept");
        return;
        //exit(1);
    }

    // Set the client socket to non-blocking mode
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // Initialize the pollfd structure for the client socket
    client.fd = client_fd;
    client.events = POLLIN | POLLHUP | POLLERR;
    client.revents = 0;

    // Add the client socket to the poll_set vector
    poll_set.push_back(client);
}

// Function to handle incoming data from a client
void server::get_data_from_client(int i)
{
    char buf[BUF_SIZE];
    std::string data;
    int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);

    // Check for errors in the received data
    if(ret < 0){ return;}
    else if(ret == 0){clear_fd(i);}
    else
    {
        // Convert the received data to a string
        data = buf;

        // Extract the requested path from the received data
        std::string path = data.substr(data.find("/"), data.find("HTTP") - 4);

        // Iterate through the contents vector to find the requested file extension
        for (unsigned int i = 0; i < contents.size(); i++)
        {
            if (path.find(contents[i]) != std::string::npos)
            {
                // Set the root path based on the requested file extension
                std::string root_ext;
                if(contents[i] == ".css")
                    root_ext = "/styles";
                else if(contents[i] == ".html")
                    continue;
                else if(contents[i] == ".js")
                    root_ext = "/scripts";
                else
                    root_ext = "/images";

                // Construct the full path to the requested file
                std::string pathed = trim(this->serveInfo.locations["/"].root + root_ext +  path);

                // Open the requested file
                std::ifstream file;
                file.open(pathed.c_str());

                // Check if the file was opened successfully
                if (!file.is_open())
                {
                    // Handle the case where the file is not found
                    std::cout << "file not found" << std::endl;
                }
                else
                {
                    // Read the contents of the file
                    file.close();
                    std::string content_type = content_typer(contents,i);
                    resp = response(pathed, content_type);
                    return;
                }
            }
        }

        // If no matching file extension is found, call the find_page function
        resp =  response(find_page(*this, data),this->serveInfo.error_pages,data);

        // Set the pollfd revents to POLLOUT to send the response
        // poll_set[i].revents = 0 | POLLOUT | POLLHUP | POLLERR;
    }
}

// Function to send the
