cmake_minimum_required(VERSION 3.11.4)

project(libmaestro)

add_library(maestro STATIC libmaestro.h libmaestro.cpp)
target_link_libraries(maestro PRIVATE usb-1.0)
set_target_properties(maestro PROPERTIES CXX_STANDARD 11)
set_target_properties(maestro PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(maestro PROPERTIES PUBLIC_HEADER "libmaestro.h")
set_target_properties(maestro PROPERTIES FOLDER "libmaestro")
target_include_directories(maestro PUBLIC .)

install(TARGETS maestro
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

