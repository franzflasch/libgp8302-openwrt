cmake_minimum_required(VERSION 3.0)

project(gp8302)

set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-but-set-variable)

add_library(gp8302 SHARED libgp8302.c)
target_include_directories(gp8302 PUBLIC .)
target_link_libraries(gp8302 PUBLIC i2c-user)

install(TARGETS gp8302 RUNTIME DESTINATION /usr/lib)
