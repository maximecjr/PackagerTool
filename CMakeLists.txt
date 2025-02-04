cmake_minimum_required(VERSION 3.10)
project(PackagerTool)

set(CMAKE_CXX_STANDARD 17)

# Ajout de JsonCpp
find_package(JsonCpp REQUIRED)

# Fichiers sources
add_executable(PackagerTool PackagerTool.cpp)

# Lier la bibliothèque JsonCpp
target_link_libraries(PackagerTool JsonCpp::JsonCpp)