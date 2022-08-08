#include "response.hpp"
#include <sstream>
std::string  response::build_response(void)
{
	time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
	std::stringstream ss;
	std::string content = local_info.find_content();
	int content_length = content.length();
	ss << content_length;
	std::string content_length_str = ss.str();
	std::string content_type = local_info.find_type();
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%a, %b %d %H:%M:%S %Y",timeinfo);
//build  a response depending on the type of the page and server status
//if the page is not found, return a 404 error
//if the page is found, return a 200 ok
//if the server is not running, return a 503 error
	std::string response = "HTTP/1.1 " + this->status +"\r\n";
	response += "Date: " + std::string(buffer) + "\r\n";
	response += "Content-Type: " + content_type + "\r\n";
	response += "Content-Length: " + content_length_str  + "\r\n";
	response += "\r\n";
	response += content;
	return response;
	}

response::response():path(""){}
response::response(location_info local_info,std::map<int,std::string> error_page, std::string &path):error_page(error_page),path(path)
{
	this->local_info = local_info;
	this->path = path;
	this->local_info = local_info;
	if (local_info.root == "")
	{
		this->status = "404 Not Found";
		this->status_code = 404;
	}
	else
	{
		this->status = "200 OK";
		this->status_code = 200;

	}
}

