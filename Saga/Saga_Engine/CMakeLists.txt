
# CMake for "Saga_Engine" directory
# we add all our .cpp files here
add_library(${ENGINE_EXECUTABLE} STATIC
${CMAKE_CURRENT_SOURCE_DIR}/framework/Application.h
${CMAKE_CURRENT_SOURCE_DIR}/framework/Application.cpp
${CMAKE_CURRENT_SOURCE_DIR}/framework/Core.h
${CMAKE_CURRENT_SOURCE_DIR}/framework/Core.cpp
${CMAKE_CURRENT_SOURCE_DIR}/framework/World.h
${CMAKE_CURRENT_SOURCE_DIR}/framework/World.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Entry_Point.h
${CMAKE_CURRENT_SOURCE_DIR}/Entry_Point.cpp
)


target_include_directories(
${ENGINE_EXECUTABLE} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)

# linking SFML to the Engine
target_link_libraries(
${ENGINE_EXECUTABLE} PUBLIC 
SFML::Graphics 
SFML::Window 
SFML::System 
SFML::Audio
)