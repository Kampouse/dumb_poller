#ifndef RESPONSE_HPP // Include guard to prevent multiple inclusions
#define RESPONSE_HPP

#include <iostream> // Include the iostream library for input/output operations
#include <string> // Include the string library for string manipulations
#include "config_structs.hpp" // Include the config_structs header file

// Define the response class
class response {
public:

    // Default constructor
    response();

    // Constructor with 3 parameters
    response(location_info serv, std::map<int,std::string> error_page,std::string &path);

    // Constructor with 2 parameters
    response (std::string &path,std::string &type);

    // Function to build the response
    std::string build_response(void);

private: 

    // Instance variable to store location information
    location_info  local_info;

    // Instance variable to store error page information
    std::map<int,std::string> error_page;

    // Instance variable to store the path
    std::string path;

    // Instance variable to store the type
    std::string type;

    // Instance variable to store the status
    std::string status;

    // Instance variable to store the status code
    int status_code;
};

#endif // Include guard end
