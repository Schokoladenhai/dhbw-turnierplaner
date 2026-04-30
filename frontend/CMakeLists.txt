cmake_minimum_required(VERSION 3.15)

# --- 1. Compiler-Pfad erzwingen ---
set(CMAKE_C_COMPILER "C:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")

project(DHBW_Turnierplaner_Frontend LANGUAGES CXX)

# --- 2. C++ Standard auf 20 erhöhen ---
# Dies aktiviert std::span und entfernt die Abhängigkeit zur GSL
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3. Externe Bibliotheken laden (stduuid) ---
include(FetchContent)
FetchContent_Declare(
    stduuid
    GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
    GIT_TAG        v1.2.3
)
# MakeAvailable lädt die Quelldateien und fügt das Target 'uuid' hinzu
FetchContent_MakeAvailable(stduuid)

# --- 4. Qt-Automatisierung & Pfade ---
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64" CACHE PATH "Pfad zu Qt" FORCE)

# --- 5. Qt-Bibliotheken suchen ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- 6. Quelldateien sammeln ---
file(GLOB_RECURSE FRONTEND_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE FRONTEND_HEADERS CONFIGURE_DEPENDS "inc/*.hpp")
file(GLOB_RECURSE FRONTEND_UIS CONFIGURE_DEPENDS "src/*.ui")

# --- 7. Das ausführbare Programm (Target) erstellen ---
add_executable(frontend_app WIN32
    ${FRONTEND_SOURCES}
    ${FRONTEND_HEADERS}
    ${FRONTEND_UIS}
)

# --- 8. Verzeichnisse einbinden ---
target_include_directories(frontend_app PRIVATE 
    inc 
    ${CMAKE_CURRENT_BINARY_DIR}
)

# --- 9. Bibliotheken verlinken ---
# Korrektur: Das Target der Bibliothek heißt 'uuid', nicht 'stduuid::stduuid'
target_link_libraries(frontend_app PRIVATE 
    Qt6::Widgets
    uuid
)

# --- 10. Debug-Ausgabe ---
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Standard: C++${CMAKE_CXX_STANDARD}")