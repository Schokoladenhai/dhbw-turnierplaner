cmake_minimum_required(VERSION 3.16)
project(frontend_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt 6 Core, Widgets und Network für die API-/SSE-Anbindung laden
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

# Dynamische Inhaltssuche in den Projektverzeichnissen
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*.h" "inc/*.hpp")
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS "src/*.ui")

# Ausführbare Datei deklarieren
add_executable(frontend_app)

# Quellcodedateien dem Target zuweisen
target_sources(frontend_app PRIVATE
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)

# JSON-Bibliothek von nlohmann über FetchContent einbinden
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Basisverzeichnis "inc/" für Header-Include-Pfade definieren
target_include_directories(frontend_app PRIVATE inc)

# Externe Bibliotheken und Qt-Module mit der App verknüpfen
target_link_libraries(frontend_app PRIVATE 
    Qt6::Widgets 
    Qt6::Network 
    nlohmann_json::nlohmann_json
)

if(WIN32)
    get_target_property(Qt6_CORELIB_RELEASE_LOCATION Qt6::Core LOCATION)
    get_filename_component(QT_BIN_DIR "${Qt6_CORELIB_RELEASE_LOCATION}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET frontend_app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env PATH="${QT_BIN_DIR}"
            ${WINDEPLOYQT_EXECUTABLE} --compiler-runtime "$<TARGET_FILE:frontend_app>"
            COMMENT "Führe automatischen windeployqt-Aufruf für das Frontend aus..."
        )
    endif()
endif()