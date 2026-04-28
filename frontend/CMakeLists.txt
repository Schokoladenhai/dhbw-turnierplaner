cmake_minimum_required(VERSION 3.15)
project(DHBW_Turnierplaner_Frontend LANGUAGES CXX)

# --- 1. C++ Standard definieren ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. Qt-Automatisierung aktivieren ---
# Nimmt dir die manuelle Arbeit mit MOC, UIC und RCC ab
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# --- 3. Qt-Pfad erzwingen ---
# Dies umgeht fehlerhafte oder fehlende VS Code Einstellungen
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64" CACHE PATH "Pfad zu Qt" FORCE)

# --- 4. Qt-Bibliotheken suchen ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- 5. Quelldateien sammeln ---
# Sucht automatisch nach .cpp und .ui Dateien im src-Ordner
file(GLOB_RECURSE FRONTEND_SOURCES "src/*.cpp")
file(GLOB_RECURSE FRONTEND_UIS "src/*.ui")

# Sicherheitsüberprüfung, damit add_executable nicht abstürzt
if(NOT FRONTEND_SOURCES)
    message(FATAL_ERROR "Keine C++ Quelldateien im Ordner 'src/' gefunden!")
endif()

# --- 6. Das ausführbare Programm (Target) erstellen ---
add_executable(frontend_app 
    ${FRONTEND_SOURCES}
    ${FRONTEND_UIS}
)

# --- 7. Eigene Header-Verzeichnisse einbinden ---
target_include_directories(frontend_app PRIVATE inc)

# --- 8. Qt mit dem Target verlinken ---
target_link_libraries(frontend_app PRIVATE Qt6::Widgets)

# --- 9. Windows-spezifische Einstellungen ---
# Verhindert, dass sich beim Start der Qt-App ein leeres schwarzes CMD-Fenster öffnet
if(WIN32)
    target_link_options(frontend_app PRIVATE -mwindows)
endif()