#include <iostream>
#include <string>
#include <list>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include "poll.h" 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF



int nonblock(int sockfd)
{
 
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
	return 0;
}






int ft_polling(void)
{

	 int client_fd = 0;
	 fd_set select_fd;
	 struct timeval instant_timeout;
	 struct  sockaddr_in server_addr;
	 struct  sockaddr_in client_addr;
	 int client_addr_len;

	 int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int  nfds = 0;
 pollfd  poll_set[100];

	 server_addr.sin_family = AF_INET;
	 server_addr.sin_port = htons(SERVER_PORT);
	 server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr));
	 bzero(&(server_addr.sin_zero), 8);
	 nonblock(server_fd);
	if	(bind(server_fd, 
	(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	 {
		 perror("bind");
		 exit(1);
	 }
	listen(server_fd,100);
	poll_set[0].fd = server_fd;
	poll_set[0].events = POLLIN;
	poll_set[0].revents = 0;
	nfds = 1;
while(1)
{
	poll (poll_set, nfds, -1);
	std::cout << "wtf" << std::endl;
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
				printf("recv %d bytes from %d\n", ret, poll_set[poll_index].fd);
				char http_header[] = "HTTP/1.1 200 OK\r\n\"Content-Type: text/html\r\n\r\n";
				send(poll_set[poll_index].fd, http_header, sizeof(http_header), 0);
				close(poll_set[poll_index].fd);
				poll_set[poll_index].fd = -1;
				std::cout<<buf<<std::endl;
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




int main(void)
{
std::list<int> fd_list = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
fd_list.push_back(1);
	ft_polling();
	return 0;


}
