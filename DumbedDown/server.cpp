#include "server.hpp"	
#include <sstream>
#include <filesystem>

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

static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(WHITESPACES);
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of(WHITESPACES);
    return str.substr(first, (last - first + 1));
}

static location_info find_page(server &serv, std::string &path)
{
	unsigned long pos = -1;
	std::vector <std::string> allowed_requests;
	allowed_requests.push_back("GET");
	allowed_requests.push_back("POST");
	allowed_requests.push_back("HEAD");
	allowed_requests.push_back("PUT");
	allowed_requests.push_back("DELETE");
	 //refactor to  avoid this
	for (unsigned int i = 0; i < allowed_requests.size(); i++)
	{
		if (path.find(allowed_requests[i]) != std::string::npos)
		{
			pos  = i;
			break;
		}
	}
	std::string::size_type  start =  path.find(allowed_requests[pos]);
	std::string::size_type  end =  path.find("HTTP") - 4;
	std::string page = path.substr(start + allowed_requests[pos].size(), end);
	std::string::size_type  start_page =  page.find("/");
	std::ifstream file;
	page = page.substr(start_page, end);
	return ( serv.serveInfo.locations[page]);
}

static std::string content_typer(std::vector<std::string> &content_type,int index)
{
	std::string content_type_str;
	std::string temp;
	if(index > 2)
	{
		 content_type_str = "img/";
		temp = content_type[index].substr(1, content_type[index].size());
		content_type_str += temp;
	}
	else
	{
		content_type_str = "text/";
		temp = content_type[index].substr(1, content_type[index].size());
		content_type_str += temp;
	}
	return content_type_str;
}

void server::get_data_from_client(int i)
{
		char buf[BUF_SIZE];
		std::string data;
		int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
		if(ret < 0){perror("recv");exit(1);}
		else if(ret == 0){clear_fd(i);}
		else
		{
			data = buf;
			std::string path = data.substr(data.find("/"), data.find("HTTP") - 4);
			std::vector <std::string> contents;
			contents.push_back(".css");
			contents.push_back(".html");
			contents.push_back(".js");
			contents.push_back(".jpg");
			contents.push_back(".png");
			contents.push_back(".gif");
			contents.push_back(".jpeg");
			contents.push_back(".bmp");
		for (unsigned int i = 0; i < contents.size(); i++)
		{
           if (path.find(contents[i]) != std::string::npos)
		   {
			   std::cout << this->serveInfo.locations["/"].root << path << std::endl;
			   std::string pathed = trim(this->serveInfo.locations["/"].root + path);
			   std::ifstream file;
			   file.open(pathed.c_str());
			   if (!file.is_open())
			   {
				   //  what to send to request when the content is not found
				   std::cout << "file not found" << std::endl;
			   }
			   else
			   {
				   file.close();
				   std::string content_type = content_typer(contents, i);
				    resp = response(pathed, content_type);
					poll_set[i].revents = 0 | POLLIN | POLLHUP | POLLERR;
					return;
			   }
			}
		}
			resp =  response(find_page(*this, data),this->serveInfo.error_pages,data);
			poll_set[i].revents = 0 | POLLOUT | POLLHUP | POLLERR;
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
void server::get_data_from_server(int i)
{
	std::string http_response =  resp.build_response();
	int ret = send(poll_set[i].fd, http_response.c_str(), http_response.length(), 0);
	(void)ret;
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
			{
				get_data_from_client(i);
				if(poll_set[i].revents & POLLIN)
				{
					get_data_from_server(i);
				}
			}
		}
		if (poll_set[i].revents & POLLOUT )
		{
			get_data_from_server(i);
		}
	}
}
