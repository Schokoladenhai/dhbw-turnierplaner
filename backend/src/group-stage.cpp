void generateMatches(const int totalTeams, const int totalMatches) {
    // 1. Teams pro Gruppe berechnen
    // Formel: n = (2 * M_total / T_total) + 1
    int teamsPerGroup = (2 * totalMatches / totalTeams) + 1;

    // 2. Anzahl der Gruppen berechnen
    int numGroups = totalTeams / teamsPerGroup;

    // Sicherheitscheck (optional, da du meintest, es geht auf)
    if (totalTeams % teamsPerGroup != 0) {
        // Fehlerbehandlung: Ungültige Konfiguration
        return;
    }

    // Ab hier kannst du wie gewohnt fortfahren:
    // Teams in einen Vector füllen, mischen und auf die Gruppen verteilen.
    std::cout << "Turnier-Struktur: " << numGroups << " Gruppen mit je "
              << teamsPerGroup << " Teams.\n";
}
