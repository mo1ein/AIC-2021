cmake_minimum_required (VERSION 3.1)

IF(APPLE)
    SET(CMAKE_C_COMPILER clang)
    SET(CMAKE_CXX_COMPILER clang++)
ENDIF()

project (aic21-client-cpp
        LANGUAGES CXX)

add_subdirectory (client)
