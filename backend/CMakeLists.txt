cmake_minimum_required(VERSION 3.10)

project(TurnierTiegerServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
uuid.h
uuid-gen.hpp
tournament.hpp
team.hpp
stage.hpp
match.hpp
ko-stage.hpp
group-stage.hpp
json.hpp
)

set(SOURCE_FILES
main.cpp
uuid-gen.cpp
tournament.cpp
team.cpp
stage.cpp
match.cpp
ko-stage.cpp
group-stage.cpp
)

list(TRANSFORM HEADER_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/inc/")
list(TRANSFORM SOURCE_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")


# 5. Das ausführbare Programm (Executable) definieren
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc")
