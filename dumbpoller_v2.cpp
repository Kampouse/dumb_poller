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
		poll_set.reserve(100);
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
		poll_set.push_back({server_fd,POLLIN | POLLHUP | POLLERR, 0});
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
            for(std::vector<pollfd>::iterator it = poll_set.begin(); it != poll_set.end(); it++)
			{
				if(it->revents & POLLIN)
				{
					if(it->fd == server_fd)
					{
						client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
						if(client_fd < 0)
						{
							perror("accept");
							exit(1);
						}
						fcntl(client_fd, F_SETFL, O_NONBLOCK);
						poll_set.push_back({client_fd,POLLIN | POLLHUP | POLLERR, 0 });
					}
					else
					{
						char buf[BUF_SIZE];
						int ret = recv(it->fd, buf, BUF_SIZE, 0);
						std::cout << buf << std::endl;
						if(ret < 0)
						{
							perror("recv");
							exit(1);
						}
						else if(ret == 0)
						{
							close(it->fd);
							poll_set.erase(it);

							nfds--;
						}
						else
						{
                          const char *http_response = "HTTP/1.1 200 OK\r\n"
													"Content-Type: text/html\r\n"
													"Content-Length: 13\r\n"
													"\r\n"
													"Hello World!";
                          send (it->fd, http_response, ret, 0);

							printf("%s\n", buf);
							it->fd = -1;
						   poll_set.erase(it);

                         if(it == poll_set.end())
							break;
						}
					}
				}
				}
	};

};

int nonblock(int sockfd)
{
 
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
	return 0;
}

/*
int ft_polling(void)
{
	 int client_fd = 0;
	 fd_set select_fd;
	 struct timeval instant_timeout;
	 struct  sockaddr_in server_addr;
	 struct  sockaddr_in client_addr;
	 int client_addr_len;
	 int  nfds = 0;
		nfds = 1;

while(1)
{
	poll (poll_set, nfds, -1);
for(int poll_index = 0; poll_index < nfds; poll_index++)
{
	if(poll_set[poll_index].revents & POLLIN)
	{
		if(poll_set[poll_index].fd == server_fd)
		{
			socklen_t client_addr_len = sizeof(struct sockaddr_in);
			client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
			nonblock(client_fd);
			poll_set[nfds].fd = client_fd;
			poll_set[nfds].events = POLLIN;
			poll_set[nfds].revents = 0;
			nfds++;
		}
		else
		{
			char buf[BUF_SIZE];

			int ret = recv(poll_set[poll_index].fd, buf, BUF_SIZE, 0);
			std::cout<<"request?:"<<buf <<std::endl;
			if(ret == 0)
			{
				close(poll_set[poll_index].fd);
				poll_set[poll_index].fd = -1;
			}
			else if(ret < 0)
			{
				perror("recv");
				close(poll_set[poll_index].fd);
				poll_set[poll_index].fd = -1;
			}
			else
			{
				const	char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>\r\n";
				send(poll_set[poll_index].fd, response, ret, 0);
				 close (poll_set[poll_index].fd);
				 poll_set[poll_index].fd = -1;
			}
		}
		
	}
	else if( poll_set[poll_index].revents & POLLERR)
	{
		close(poll_set[poll_index].fd);
		poll_set[poll_index].fd = -1;
	}
	else if( poll_set[poll_index].revents & POLLHUP)
	{
		close(poll_set[poll_index].fd);
		poll_set[poll_index].fd = -1;
	}
	else if( poll_set[poll_index].revents & POLLNVAL)
	{
		close(poll_set[poll_index].fd);
		poll_set[poll_index].fd = -1;
	}
}

}
return 0;
}

*/

int main(void)
{

	std::cout << "server as started! " << std::endl;
	server  serv = server(SERVER_PORT);
	server  serv2 = server(8889);
while(1)
{
					serv.run();
					serv2.run();
}



	return 0;


}

