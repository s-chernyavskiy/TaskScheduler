include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        lib_tests
        any_tests.cpp
        scheduler_tests.cpp
)

target_link_libraries(
        lib_tests
        scheduler
        any
        GTest::gtest_main
)

target_include_directories(lib_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(lib_tests)
