#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include "config_structs.hpp"
class response {
	public:

	response();
	response(location_info serv, std::string &path);
	std::string build_response(void);
	private: 
	 location_info  local_info;
	 std::string path;

};
#endif
