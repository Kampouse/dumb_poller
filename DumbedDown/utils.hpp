#pragma once <-- This directive prevents the file from being included multiple times in the same compilation unit

#include <vector> <-- Include the vector library to create a vector of strings
#include <cstring> <-- Include the string library for string functions
#include <iostream> <-- Include the input/output library for console output

typedef std::vector<std::string> StringVector; <-- Create a type alias for a vector of strings

//******************************************************************************
// Function Name: split
//
// Description: This function splits a string into a vector of strings based on a given delimiter
//
// Parameters:
// line - the string to split
// delimiter - the character(s) to use as the delimiter
//
// Returns:
// A vector of strings containing the substrings from the original string
//******************************************************************************
StringVector split(std::string line, std::string delimiter)
{
    StringVector strings; <-- Create an empty vector of strings

    // Get the position of the first occurrence of the delimiter in the line
    size_t pos = line.find(delimiter);


