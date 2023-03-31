cmake_minimum_required(VERSION 3.24)
project(LFA_Tema2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(.idea)

add_executable(LFA_Tema2
        .idea/.gitignore
        .idea/LFA_Tema2.iml
        .idea/modules.xml
        .idea/vcs.xml
        .idea/workspace.xml
        LFA_Tema2.cpp
        README.md)
