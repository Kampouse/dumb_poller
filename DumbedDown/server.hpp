#pragma once

#include <iostream>
#include <string>
#include <list>
#include <sys/fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include "poll.h" 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <vector>

#include"Exceptions.hpp"
#include"config_structs.hpp"
#include"parser.hpp"
#include"utils.hpp"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF



class   server {

	public:
				std::vector<pollfd> poll_set;
			   struct sockaddr_in server_addr;
			   int nfds;
		   int server_fd;

		server(){};
		server(int port)
	{
		nfds = 0;
		 server_addr.sin_family = AF_INET;
		 server_addr.sin_port = htons(port);
		 server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
		 server_fd = socket(AF_INET, SOCK_STREAM, 0);
		 fcntl(server_fd, F_SETFL, O_NONBLOCK);
		 setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr));
		 bzero(&(server_addr.sin_zero), 8);
		if	(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		 {
			 perror("bind");
			 exit(1);
		 }
		listen(server_fd,100);
	   pollfd serv;
	   serv.fd = server_fd;
	   serv.events =  POLLIN | POLLHUP | POLLERR;
	   serv.revents = 0;
	   poll_set.push_back(serv);
	   nfds = 1;
	}
		server(std::string path);
		~server(){};
		void run()
	{
		int client_fd ;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
			poll (poll_set.data(),poll_set.size() , 100);
            for(unsigned long i = 0; i <  poll_set.size();i++)
			{
				if(poll_set[i].revents & POLLIN)
				{
				std::cout << "pollin" << std::endl;
					if(poll_set[i].fd == server_fd)
					{
						std::cout << "new client" << std::endl; 
						client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
						if(client_fd < 0)
						{
						std::cout << client_fd;
							perror("accept");
							exit(1);
						}
						fcntl(client_fd, F_SETFL, O_NONBLOCK);
						pollfd client;
						client.fd = client_fd;
						client.events = POLLIN | POLLHUP | POLLERR;
						client.revents = 0;
						poll_set.push_back(client);
					}
					else
					{
						char buf[BUF_SIZE];
					std::cout << "client_fd: " << poll_set[i].fd << std::endl;
						int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
						std::cout << buf << std::endl;
						if(ret < 0)
						{
							perror("recv");
							exit(1);
						}
						else if(ret == 0)
						{
							close(poll_set[i].fd);
							poll_set[i].fd = -1;
							poll_set.erase(poll_set.begin() + i);
							nfds--;
						}
						else
						{
							std::cout << "recv: " << buf << std::endl; 
                          const char *http_response = "HTTP/1.1 200 OK\r\n"
													"Content-Type: text/html\r\n"
													"Content-Length: 13\r\n"
													"\r\n"
													"Hello World!";
                          send (poll_set[i].fd, http_response, ret, 0);
						  close (poll_set[i].fd);
							poll_set[i].fd = -1;
							poll_set.erase(poll_set.begin() + i);
						}
					}
					
				}
			}
	};

};
