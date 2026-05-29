cmake_minimum_required(VERSION 3.14) # 3.14+ ist besser für FetchContent

project(TurnierTigerServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header Dateien
set(HEADER_FILES
    inc/uuid.h
    inc/uuid-gen.hpp
    inc/tournament.hpp
    inc/team.hpp
    inc/stage.hpp
    inc/match.hpp
    inc/ko-stage.hpp
    inc/group-stage.hpp
    inc/json.hpp
    inc/tournament-builder.hpp
)

# Core Sources (OHNE main.cpp!)
set(CORE_SOURCES
    src/uuid-gen.cpp
    src/tournament.cpp
    src/team.cpp
    src/stage.cpp
    src/match.cpp
    src/ko-stage.cpp
    src/group-stage.cpp
    src/tournament-builder.cpp
)

# 1. Core-Bibliothek erstellen (für Server UND Tests nutzbar)
add_library(TurnierTigerCore STATIC ${CORE_SOURCES} ${HEADER_FILES})
target_include_directories(TurnierTigerCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc")

# 2. Das Hauptprogramm (Executable)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE TurnierTigerCore)

# ==========================================
# GoogleTest Setup
# ==========================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 3. Test Executable definieren
add_executable(RunTests
    tests/test_team.cpp
    tests/test_match.cpp
    tests/test_ko_stage.cpp
)

# Gegen GTest und unsere eigene Bibliothek linken
target_link_libraries(RunTests
    PRIVATE
    TurnierTigerCore
    GTest::gtest_main
)

# Test-Erkennung für CTest aktivieren
include(GoogleTest)
gtest_discover_tests(RunTests)
