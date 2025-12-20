cmake_minimum_required(VERSION 3.16)
project(Temppura)

set(CXX_STANDARD 20)

set(WINDOWS_BUILD TRUE)

if (WINDOWS_BUILD)
    add_link_options(-static -static-libgcc -static-libstdc++)
endif()

add_executable(
    ${PROJECT_NAME}
    source/main.cpp
    source/compiler.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    source
)