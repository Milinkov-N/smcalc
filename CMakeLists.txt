cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(smcalc VERSION 0.1)

### Uncomment to see how much more memory getting used
### without utilizing string_view
# add_compile_definitions(_UNOPTIMIZED)

aux_source_directory(src SOURCE)
add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
set(TEST_TARGET smcalc_tests)
add_executable(
  ${TEST_TARGET}
  tests/tests_main.cc
  src/lexer.cc
  src/rpn.cc
  src/calc.cc
)
target_include_directories(${TEST_TARGET} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
  ${TEST_TARGET}
  GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(${TEST_TARGET})
