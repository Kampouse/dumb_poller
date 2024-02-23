# Assign the name of the project to the variable NAME
NAME = webserv 

# Define the C++ compiler to be used
CC = g++

# Set various compiler flags for warnings, error reporting, and debugging
FLAGS = -Wall -Wextra -Werror --std=c++98 -g

# List of source files that need to be compiled
SRCS = Exceptions.cpp main.cpp parser.cpp utils.cpp server.cpp server_utils.cpp  response.cpp

# Command to remove files and directories
RM =  rm -rf

# Define the dependency between a .cpp file and its corresponding .o file
.cpp.o:
	@c++ ${FLAGS}  -c $< -o ${<:.cpp=.o}

# List of object files generated from compiling the source files
OBJS = ${SRCS:.cpp=.o}

# Default target to be built
all: ${NAME}

# Linking step to create the executable from the object files
${NAME}:${OBJS} 
		@${CC} ${OBJS} ${FLAGS} -o ${NAME}

# Run the program after building it
run:  all 
	./${NAME} 

# Run the program using valgrind to check for memory leaks
val: all
	valgrind  --leak-check=yes --track-origins=yes -s --trace-children=yes  ./${NAME}

# Remove the object files
clean:
	    ${RM} ${OBJS}

# Compile and run the program for testing
test: all 
	./${NAME} & sleep  1 && http :9991  

# Remove the executable and any other build artifacts
fclean: clean
	@${RM} ${NAME}
	@${RM} ${NAME_TEST}

# Stage the source files and Makefile for commit
git:
		@git add ${SRCS} Makefile Exceptions.hpp config_structs.hpp parser.hpp utils.hpp server.hpp response.hpp

# Alias target to run fclean and all
re: fclean
