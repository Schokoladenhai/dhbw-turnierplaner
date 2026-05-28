cmake_minimum_required(VERSION 3.16)
project(frontend_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt 6 Core und Widgets laden
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# Dynamische Inhaltssuche in Ihren neuen Verzeichnissen
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*.h" "inc/*.hpp")
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS "src/*.ui")

add_executable(frontend_app
    )

target_sources(frontend_app PRIVATE
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)

include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Am Ende deines CMake-Files verknüpfst du es mit deiner App:
target_link_libraries(frontend_app PRIVATE nlohmann_json::nlohmann_json)

# Definiert "inc/" als Basis für Include-Pfade (wichtig für #include "windows/...")
target_include_directories(frontend_app PRIVATE inc)
target_link_libraries(frontend_app PRIVATE Qt6::Widgets)