// Exceptions.h
// This header file defines a custom exceptions class for a specific application.
// The class hierarchy includes a base class Exceptions, with several derived
// exception classes for different types of errors that may occur within the
// application.

#pragma once

#include <iostream> // Include the iostream library for standard input/output operations
#include <exception> // Include the exception library for standard exception handling

class Exceptions {
	// The base class for all custom exceptions in this application
public:
	Exceptions(); // Default constructor
	virtual ~Exceptions(); // Destructor

	// Derived exception classes
	class FileOpeningError : public std::exception {
	private:
		std::string arg; // A string argument for this specific exception
	public:
		FileOpeningError(std::string _arg); // Constructor with argument
		virtual ~FileOpeningError() throw(); // Destructor
		virtual const char * what() const throw(); // Function to return a C-string describing the error
	};

	class ConfigError : public std::exception {
	public:
		virtual const char * what() const throw(); // Function to return a C-string describing the error
	};

	class NestedServerError : public std::exception {
	public:
		virtual const char * what() const throw(); // Function to return a C-string describing the error
	};

	class SemicolonError : public std::exception {
	public:
		virtual const char * what() const throw(); // Function to return a C-string describing the error
	};

	class InvalidFieldError : public std::exception {
	private:
		std::string arg; // A string argument for this specific exception
	public:
		InvalidFieldError(std::string _arg); // Constructor with argument
		virtual ~InvalidFieldError() throw(); // Destructor
		virtual const char * what() const throw(); // Function to return a C-string describing the error
	};

	class UnknownFieldError : public std::exception {
	public:
		virtual const char
