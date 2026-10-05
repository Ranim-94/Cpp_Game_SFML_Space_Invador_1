
# CMake for "Saga_Code" directory
# we add all our .cpp files here
add_executable(${GAME_EXECUTABLE}
${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Application.h
${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Application.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/Entry_Point.h
${CMAKE_CURRENT_SOURCE_DIR}/src/Entry_Point.cpp
)


target_include_directories(${GAME_EXECUTABLE} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include

)