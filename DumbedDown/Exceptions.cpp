#include "Exceptions.hpp"

// Constructor for the FileOpeningError class
Exceptions::FileOpeningError::FileOpeningError(std::string _arg) : arg("Error opening file: " + _arg + "\n") {}

// Destructor for the FileOpeningError class
Exceptions::FileOpeningError::~FileOpeningError() throw() {}

// Returns the error message for the FileOpeningError class
const char * Exceptions::FileOpeningError::what() const throw() { return (arg.c_str()); }

// Returns the error message for the ConfigError class
const char * Exceptions::ConfigError::what() const throw() { return ("Error in Configuration file\n"); }

// Returns the error message for the NestedServerError class
const char * Exceptions::NestedServerError::what() const throw() { return ("Nested servers are not allowed in this project\n"); }

// Returns the error message for the SemicolonError class
const char * Exceptions::SemicolonError::what() const throw() { return ("Missing ; in Configuration file\n"); }

// Returns the error message for the UnknownFieldError class
const char * Exceptions::UnknownFieldError::what() const throw() { return ("Unknown field in Configuration file\n"); }

// Constructor for the InvalidFieldError class
Exceptions::InvalidFieldError::InvalidFieldError(std::string _arg) : arg(_arg + " field has an invalid argument\n") {}

// Destructor for the InvalidFieldError class
Exceptions::InvalidFieldError::~InvalidFieldError() throw() {}

// Returns the error message for the InvalidFieldError class
const char * Exceptions::InvalidFieldError::what() const throw() { return (arg.c_str()); }
