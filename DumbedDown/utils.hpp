#pragma once  <-- This directive prevents the file from being included multiple times in the same compilation unit

#include <vector>  <-- Include the vector library to create a vector of strings
#include <cstring> <-- Include the string library for string functions
#include <iostream> <-- Include the input/output library for console output

typedef std::vector<std::string> StringVector;  <-- Create a type alias for a vector of strings

// Function to split a string into a vector of strings based on a given delimiter
StringVector split(std::string line, std::string delimiter)
{
    StringVector strings;  <-- Create an empty vector of strings

    // Get the position of the first occurrence of the delimiter in the line
    size_t pos = line.find(delimiter);

    // While the delimiter is found in the line
    while (pos != std::string::npos)
    {
        // Extract the substring up to the delimiter
        std::string substring = line.substr(0, pos);

        // Add the substring to the vector of strings
        strings.push_back(substring);

        // Remove the substring from the line
        line.erase(0, pos + delimiter.length());

        // Get the position of the next occurrence of the delimiter in the line
        pos = line.find(delimiter);
    }

    // Add the remaining substring to the vector of strings
    strings.push_back(line);

    // Return the vector of strings
    return strings;
}
