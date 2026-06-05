cmake_minimum_required(VERSION 3.22)
project(CubicDiscord VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

include(cmake/CPM.cmake)

option(CUBIC_LOCAL_BUILD "Enable local testing mode" OFF)
if(CUBIC_LOCAL_BUILD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE CUBIC_LOCAL_BUILD=1)
endif()

find_package(dpp CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    re2::re2
    dpp::dpp
    OpenSSL::SSL OpenSSL::Crypto
)

CPMAddPackage("gh:dankmeme01/asp2#main")

target_link_libraries(${PROJECT_NAME} PRIVATE
    asp
    fmt
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
