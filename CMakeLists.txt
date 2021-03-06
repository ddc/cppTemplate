cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME cppTemplate)
project(${PROJECT_NAME})

set(SOURCE_FILES
        main.cpp
        src/version.rc
        src/constants.hpp
        src/exceptions.hpp
        src/utils.cpp
        src/utils.hpp
        src/log.cpp
        src/log.hpp
        src/databases/sqlite/sqlite.cpp
        src/databases/sqlite/sqlite.hpp
        src/databases/oracle/oracle.cpp
        src/databases/oracle/oracle.hpp)

find_package(Boost 1.77.0 COMPONENTS date_time filesystem system iostreams program_options serialization)
if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} sqlite3)
endif()
