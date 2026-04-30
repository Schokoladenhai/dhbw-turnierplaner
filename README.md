# dhbw-turnierplaner
Tournament Organizer, as a simple softwareproject in Softwareengineering 1.


# 1. Erstelle den Build-Ordner und konfiguriere das Projekt
cmake -S . -B build

# 2. Kompiliere alles
cmake --build build

# 3. Fertige Bins liegen unter ./build/bin
./build/bin/TurnierTiegerServer
./build/bin/TurnierTiegerClient
