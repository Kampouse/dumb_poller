#include "server.hpp"	

server::server(server_info servInfo)
{
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(servInfo.port);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serveInfo = servInfo;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr));
	bzero(&(server_addr.sin_zero), 8);
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(1);
	}
	listen(server_fd, 100);
	pollfd serv;
	serv.fd = server_fd;
	serv.events = POLLIN | POLLHUP | POLLERR;
	serv.revents = 0;
	poll_set.push_back(serv);
}

void   server::clear_fd (int i)
{
			close(poll_set[i].fd);
				poll_set[i].fd = -1;
				poll_set.erase(poll_set.begin() + i);
}

void server::add_client (void)
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd;
	pollfd client;
	if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
	{
		perror("accept");
		exit(1);
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	client.fd = client_fd;
	client.events = POLLIN | POLLHUP | POLLERR;
	client.revents = 0;
	poll_set.push_back(client);
}


location_info find_page( server &serv , std::string &path)
{
std::string::size_type  start =  path.find("GET");
std::string::size_type  end =  path.find("HTTP") - 4;
std::string page = path.substr(start + 3, end);
std::string::size_type  start_page =  page.find("/");
page = page.substr(start_page, end);
location_info local_info =  serv.serveInfo.locations[page];

std::cout << local_info << std::endl;
 if(local_info.root == "")
 {
  std::cout << "404" << std::endl;
  return local_info;
 }
 else
 {
  std::cout << "200" << std::endl;
  return local_info;
 }
}

void buid_response(location_info &local_info)
{
	std::string response = "HTTP/1.1 200 OK\r\n";
	
	response += "Content-Type: " + local_info.find_type() + "\r\n";
	//response += "Content-Length: " + std::to_string(local_info.size) + "\r\n";
	response += "\r\n";
	response += local_info.root;
}







void server::get_data_from_client(int i)
{
	   char buf[BUF_SIZE];
	   std::string data;
		std::cout << "client_fd: " << poll_set[i].fd << std::endl;
		int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);

		std::cout << buf << std::endl;
		if(ret < 0){perror("recv"); exit(1); }
		else if(ret == 0){ clear_fd(i); }
		else
		{
			data = buf;
			find_page(*this, data);
		}
		/*else
		{
			std::cout << "recv: " << buf << std::endl; 
			data = buf;
			find_page (*this, data);
		  const char *http_response = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\nContent-Length: 13\r\n\r\n Hello World!";
		  send (poll_set[i].fd, http_response, ret, 0);
		  clear_fd(i);
		}
		*/
}
void server::run()
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
}

