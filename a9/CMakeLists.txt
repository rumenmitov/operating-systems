cmake_minimum_required(VERSION 3.10)

project(quiz)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")


add_executable(quiz src/quiz.c src/fetch.c src/parse.c src/play.c src/errors.c)
target_link_libraries(quiz jansson)
