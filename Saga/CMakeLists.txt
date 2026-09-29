
# CMake file at Workspace level

cmake_minimum_required(VERSION 4.4.3)

# Set some basic project attributes
project (Saga
	VERSION 1.0
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

#create a variable
set(GAME_EXECUTABLE Saga_Prog)

# Poiting out to the subdirectory containing 
# the source code
add_subdirectory(Saga_Code)


