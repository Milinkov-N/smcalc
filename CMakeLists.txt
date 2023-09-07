cmake_minimum_required(VERSION 3.26)
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
