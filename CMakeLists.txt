cmake_minimum_required(VERSION 3.16)
project("Otherengine")

add_subdirectory(Source)

install(DIRECTORY "Assets" DESTINATION "Engine")
install(DIRECTORY "Shaders" DESTINATION "Engine")
