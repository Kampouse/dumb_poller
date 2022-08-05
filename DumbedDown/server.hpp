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
		server(int port);
		server(std::string path);
		~server(){};
		void add_client(void);
		void clear_fd(int i);
		void get_data_from_client(int i);
		void run();
};