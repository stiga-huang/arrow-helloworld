cmake_minimum_required(VERSION 2.8.12)
project(arrow-helloworld)

set(CMAKE_MODULE_PATH "~/software/arrow-8.0.0/lib/cmake/arrow/")
set(ARROW_HOME "~/software/arrow-8.0.0")
find_package(Arrow REQUIRED)

add_executable(json_decimal_example json_decimal_example.cc)
target_link_libraries(json_decimal_example PRIVATE arrow_shared)
