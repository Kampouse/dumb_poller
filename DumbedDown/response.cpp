#include "response.hpp"
#include <sstream>


std::string  response::build_response(void)
{
	std::string content;
	time_t rawtime;
    struct tm * timeinfo;
    char time_string[80];
	int content_length;
	std::stringstream ss;
	std::string content_type ;

	if(status_code != 200)
{
	content  = 	local_info.find_error_page( error_page[status_code]);
	std::cout << "error " << content << std::endl;
	content_length = content.length();
	content_type = "text/html";
}
else if(status_code == 200)
{
	content = local_info.find_content();
	content_length = content.length();
	content_type = local_info.find_type();
}
	ss << content_length;
	std::string content_length_str = ss.str();
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_string,80,"%a, %b %d %H:%M:%S %Y",timeinfo);
	std::string response = "HTTP/1.1 " + status + "\r\n";
	response += "Date: " + std::string(time_string) + "\r\n";
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

