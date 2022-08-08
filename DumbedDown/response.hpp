#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include "config_structs.hpp"
class response {
	public:

	response();
	response(location_info serv, std::map<int,std::string> error_page,std::string &path);
	std::string build_response(void);
	private: 
	 location_info  local_info;
	 std::map<int,std::string> error_page;
	 std::string path;
	 std::string status;
	 int status_code;
};
#endif
