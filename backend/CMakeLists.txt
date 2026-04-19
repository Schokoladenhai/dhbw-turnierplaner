cmake_minimum_required(VERSION 3.10)

# 1. Projektname und Sprache definieren
project(MeinProjekt VERSION 1.0 LANGUAGES CXX)

# 2. WICHTIG: Erzeugt die compile_commands.json für Zed/clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3. C++ Standard festlegen (z.B. C++17 oder C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Alle Quelldateien aus src/ sammeln
# (Ersetze 'main.cpp', falls deine Datei anders heißt)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 5. Das ausführbare Programm (Executable) definieren
add_executable(${PROJECT_NAME} ${SOURCES})

# 6. Den Header-Ordner 'inc/' bekannt machen
# PUBLIC sorgt dafür, dass clangd den Pfad sofort findet
target_include_directories(${PROJECT_NAME} PUBLIC inc)
