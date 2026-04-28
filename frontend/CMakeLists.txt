cmake_minimum_required(VERSION 3.15)

# --- 1. Compiler-Pfad erzwingen ---
# Muss VOR project() stehen, um Strawberry Perl definitiv zu überschreiben
set(CMAKE_C_COMPILER "C:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")

project(DHBW_Turnierplaner_Frontend LANGUAGES CXX)

# --- 2. C++ Standard definieren ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3. Qt-Automatisierung & Pfade ---
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Pfad zu den Qt-Bibliotheken
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64" CACHE PATH "Pfad zu Qt" FORCE)

# --- 4. Qt-Bibliotheken suchen ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- 5. Quelldateien sammeln ---
# Wir sammeln jetzt auch die Header (.hpp), damit der MOC sie zuverlässig findet
file(GLOB_RECURSE FRONTEND_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE FRONTEND_HEADERS CONFIGURE_DEPENDS "inc/*.hpp")
file(GLOB_RECURSE FRONTEND_UIS CONFIGURE_DEPENDS "src/*.ui")

# --- 6. Das ausführbare Programm (Target) erstellen ---
# WICHTIG: Die Header müssen hier mit aufgeführt werden, 
# sonst generiert Qt keinen Code für Signale/Slots (vtable-Fehler)
add_executable(frontend_app WIN32
    ${FRONTEND_SOURCES}
    ${FRONTEND_HEADERS}
    ${FRONTEND_UIS}
)

# --- 7. Verzeichnisse einbinden ---
target_include_directories(frontend_app PRIVATE 
    inc 
    ${CMAKE_CURRENT_BINARY_DIR}
)

# --- 8. Qt mit dem Target verlinken ---
target_link_libraries(frontend_app PRIVATE Qt6::Widgets)

# --- 9. Debug-Ausgabe ---
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Gefundene Header für MOC: ${FRONTEND_HEADERS}")