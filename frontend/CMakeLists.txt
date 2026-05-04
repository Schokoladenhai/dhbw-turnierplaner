cmake_minimum_required(VERSION 3.15)

# --- 1. Compiler-Pfad erzwingen ---
set(CMAKE_C_COMPILER "C:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")

project(DHBW_Turnierplaner_Frontend LANGUAGES CXX)

# --- 1. C++ Standard definieren ---
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

# --- 3. Qt-Pfad erzwingen ---
# Dies umgeht fehlerhafte oder fehlende VS Code Einstellungen
if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64" CACHE PATH "Pfad zu Qt" FORCE)
endif()

# --- 5. Qt-Bibliotheken suchen ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- 5. Quelldateien sammeln ---
# Sucht automatisch nach .cpp und .ui Dateien im src-Ordner
file(GLOB_RECURSE FRONTEND_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE FRONTEND_UIS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")

# --- 7. Das ausführbare Programm (Target) erstellen ---
add_executable(frontend_app WIN32
    ${FRONTEND_SOURCES}
    ${FRONTEND_HEADERS}
    ${FRONTEND_UIS}
)

# --- 7. Eigene Header-Verzeichnisse einbinden ---
target_include_directories(frontend_app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")

# --- 9. Bibliotheken verlinken ---
# Korrektur: Das Target der Bibliothek heißt 'uuid', nicht 'stduuid::stduuid'
target_link_libraries(frontend_app PRIVATE 
    Qt6::Widgets
    uuid
)

# --- 9. Windows-spezifische Einstellungen ---
# Verhindert, dass sich beim Start der Qt-App ein leeres schwarzes CMD-Fenster öffnet
if(WIN32)
    target_link_options(frontend_app PRIVATE -mwindows)
endif()
