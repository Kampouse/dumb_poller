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
			   int server_fd;

		server(){};
		server(int port)
	{
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
	}
		server(std::string path);
		~server(){};


		void add_client(void)
	{
		struct sockaddr_in client_addr;
		socklen_t	client_addr_len = sizeof(client_addr);
		int			client_fd ;
		pollfd		client;
		if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
		{
		std::cout << client_fd;
			perror("accept");
			exit(1);
		}
		fcntl(client_fd, F_SETFL, O_NONBLOCK);
		client.fd = client_fd;
		client.events = POLLIN | POLLHUP | POLLERR;
		client.revents = 0;
		poll_set.push_back(client);
	}
    void clear_fd(int i)
	{
			close(poll_set[i].fd);
			poll_set[i].fd = -1;
			poll_set.erase(poll_set.begin() + i);
	}

	void get_data_from_client(int i)
	{
	   char buf[BUF_SIZE];
		std::cout << "client_fd: " << poll_set[i].fd << std::endl;
		int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
		std::cout << buf << std::endl;
		if(ret < 0){perror("recv"); exit(1); }
		else if(ret == 0){ clear_fd(i); }
		else
		{
			std::cout << "recv: " << buf << std::endl; 
		  const char *http_response = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\nContent-Length: 13\r\n\r\n Hello World!";
		  send (poll_set[i].fd, http_response, ret, 0);
		  clear_fd(i);
		}
	}
		void run()
	{
			poll (poll_set.data(),poll_set.size() , 100);
            for(unsigned long i = 0; i <  poll_set.size();i++)
			{
				if(poll_set[i].revents & POLLIN)
				{
					if(poll_set[i].fd == server_fd)
						add_client();
					else
                        get_data_from_client(i);
				}
			}
	};

};
