cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME cppTemplate)
project(${PROJECT_NAME})

set(SOURCE_FILES
        main.cpp
        src/constants.hpp
        src/log.cpp
        src/log.hpp
        src/utils.cpp
        src/utils.hpp
        src/version.rc
        )

find_package(Boost 1.77.0 COMPONENTS date_time filesystem system iostreams program_options serialization)
if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()
