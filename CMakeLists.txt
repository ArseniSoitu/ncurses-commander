cmake_minimum_required(VERSION 2.8)
project(commanderWindow)

file(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ncurses)
