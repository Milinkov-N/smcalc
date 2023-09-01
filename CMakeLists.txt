cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

project(smcalc)


aux_source_directory(src SOURCE)
add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
