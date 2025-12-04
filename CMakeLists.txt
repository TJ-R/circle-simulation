cmake_minimum_required(VERSION 3.20)

# Name of project
project (circle-simulation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This should ensure /usr is first
# place to check since it was finding
# the static library I have for windows
set(CMAKE_PREFIX_PATH "/usr")

find_package(SFML 3 REQUIRED COMPONENTS
  System
  Graphics
  Window
  Audio
  Network
)

# Add your source files
set(SOURCES
  circle-simulation/ball.h
  circle-simulation/ball.cpp
  circle-simulation/main.cpp
)

# Create an executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE circle-simulation)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SFML::System
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::Network
)
