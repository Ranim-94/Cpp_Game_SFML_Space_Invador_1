
# CMake for "Saga_Game" directory
# we add all our .cpp files here
add_executable(${GAME_EXECUTABLE}
${CMAKE_CURRENT_SOURCE_DIR}/game_framework/Game_Application.h
${CMAKE_CURRENT_SOURCE_DIR}/game_framework/Game_Application.cpp
)


target_include_directories(${GAME_EXECUTABLE} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/game_framework
)

# linking the game to the engine
target_link_libraries(${GAME_EXECUTABLE} PUBLIC ${ENGINE_EXECUTABLE})