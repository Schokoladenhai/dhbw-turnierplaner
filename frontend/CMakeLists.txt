cmake_minimum_required(VERSION 3.15)
project(DHBW_Turnierplaner_Frontend LANGUAGES CXX)

# Standards definieren
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Includeverzeichnis definieren (relativ zum Frontend-Ordner)
include_directories(inc)

# Alle Quelldateien rekursiv sammeln oder explizit angeben
file(GLOB_RECURSE FRONTEND_SOURCES "src/*.cpp" "src/*.c")

# Das Target definieren
add_executable(frontend_app src/main.cpp ${FRONTEND_SOURCES})

# Falls das Frontend Bibliotheken benötigt (z.B. Qt, SFML, etc.)
# find_package(PackageName REQUIRED)
# target_link_libraries(frontend_app PRIVATE PackageName::Target)