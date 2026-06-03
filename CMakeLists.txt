cmake_minimum_required(VERSION 3.22)
project(CubicDiscord VERSION 1.0.0 LANGUAGES CXX)

include(cmake/CPM.cmake)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(dpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
CPMAddPackage("gh:dankmeme01/asp2#main")

target_link_libraries(${PROJECT_NAME} PRIVATE
    dpp::dpp
    OpenSSL::SSL
    OpenSSL::Crypto
    asp
    fmt
)

option(CUBIC_LOCAL_BUILD "Enable local testing mode" OFF)
if(CUBIC_LOCAL_BUILD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE CUBIC_LOCAL_BUILD=1)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
