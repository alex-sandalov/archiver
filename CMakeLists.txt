cmake_minimum_required(VERSION 3.12)

project(
    ArchiverLabwork6
    VERSION 1.0
    DESCRIPTION "Archiver"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(ParserArguments)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)