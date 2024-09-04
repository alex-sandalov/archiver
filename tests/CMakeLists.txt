include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCES
	ArchiverTests.cpp
)

add_executable(ArchiverTests ${SOURCES})

target_link_libraries(
    ArchiverTests
    Archiver
    ParserArgumentsLibray
    GTest::gtest_main
)



target_include_directories(ArchiverTests PRIVATE "${PROJECT_SOURCE_DIR}/include")

include(GoogleTest)

gtest_discover_tests(ArchiverTests)