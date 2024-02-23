#include "utils.hpp"  // Include the necessary header file

// Function to split a string into a vector of strings based on a given delimiter
StringVector split(std::string line, std::string delimiter)
{
    StringVector result;  // Create an empty vector to store the split strings

    // If the input string is empty, return an empty vector
    if (line.empty())
        return result;

    size_t i = 0, j = 0;  // Initialize two index variables

    // Loop through the input string
    while (i < line.length() && j < line.length())
    {
        // Find the index of the first occurrence of the delimiter
        // starting from index j
        i = line.find_first_of(delimiter, j);

        // If the delimiter is not found, set i to the end of the string
        if (i == std::string::npos)
            i = line.length();

        // Extract the substring from index j to i and add it to the result vector
        std::string str = line.substr(j, i - j);
        if (!str.empty())
            result.push_back(str);

        // Find the index of the first non-delimiter character
        // starting from index i
        j = line.find_first_not_of(delimiter, i);
    }

    // Return the resulting vector of strings
    return result;
}

