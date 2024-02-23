#include "parser.hpp"  // Include header file for this parser class
#include "config_structs.hpp"  // Include header file for configuration data structures
#include <bits/stdc++.h>  // Include standard library for Linux
#include <cstdlib>  // Include standard library for C
#include <exception>  // Include exception handling library

// Constructor and Destructor for the parser class
parser::parser() {}

parser::~parser() {}

// Overloaded constructor that takes a file path as an argument
parser::parser(std::string _path) {
	parsefile(_path);  // Call the parsefile function with the given file path
}

// Function to trim whitespace from a string
static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(WHITESPACES);
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of(WHITESPACES);
    return str.substr(first, (last - first + 1));
}

// Extract file content into a vector of strings
void parser::extractfile()
{
	std::string line;
	size_t pos;
	while (getline(config_file_fd, line))
	{
		line = trim(line);
		while ((pos = line.find_first_of("{}")) != std::string::npos || line.length())
		{
			if (pos == std::string::npos)
			{
				ext_file.push_back(line);
				break;
	
