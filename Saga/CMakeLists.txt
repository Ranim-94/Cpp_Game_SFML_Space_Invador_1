
# CMake file at Workspace level, for the whole project

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

# Pointing out to the subdirectory containing 
# the source code
add_subdirectory(Saga_Code)

# Adding SFML
# Use the globally installed SFML 3.1 package.
find_package(SFML 3.1 CONFIG REQUIRED COMPONENTS 
Graphics Window System Audio)

# Linking the SFML targets to the project
target_link_libraries(${GAME_EXECUTABLE} PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
	SFML::Audio
)


#[[

# Installing SFML from Github

include(FetchContent)

# create variable name for SFML lib
set(LIB_SFML_NAME SFML)

FetchContent_Declare(
${LIB_SFML_NAME}
GIT_REPOSITORY https://github.com/SFML/SFML
GIT_TAG 3.1.0
GIT_SHALLOW FALSE
)

# 'GIT_SHALLOW FALSE': reproduce all SFML commit history

# Build SFML's own dependencies from source instead of 
# relying on system packages
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
${LIB_SFML_NAME}
)
#]]







