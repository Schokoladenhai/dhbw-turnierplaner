cmake_minimum_required(VERSION 3.10)
project(MeinGesamtProjekt VERSION 1.0)

# 1. Globale Build-Einstellungen
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# 2. Output-Verzeichnis festlegen (Die Exen landen in build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 3. Unterprojekte einbinden
add_subdirectory(backend)
# add_subdirectory(frontend)
