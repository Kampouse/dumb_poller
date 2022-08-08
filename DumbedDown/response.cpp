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
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Date: " + std::string(buffer) + "\r\n";
	response += "Content-Type: " + content_type + "\r\n";
	response += "Content-Length: " + content_length_str  + "\r\n";
	response += "\r\n";
	response += content;
	return response;
	}

response::response():path("")
{
	
}
response::response(location_info local_info, std::string &path)
{
	this->path = path;
	this->local_info = local_info;
}


