# CMakeList.txt : CMake project for SolarSystemMVP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
# Add source to this project's executable.
add_executable (main "main.cpp" )
target_link_libraries(main PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/gamedata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# TODO: Add tests and install targets if needed.
