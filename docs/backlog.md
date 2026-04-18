# Projekt Backlog

Hier werden alle Anforderungen und Features gesammelt. Sobald ein Item in einen Sprint gezogen wird, wird es entsprechend markiert.

## Legende
- **B** Backlog (Noch nicht geplant)
- **S** In Arbeit (Einem Sprint zugeordnet)
- **E** Erledigt

---

## Features / Tasks

- [x] E **Grundgerüst:** Repository aufsetzen und README erstellen
- [ ] B **SSE** Server Site Events für das Backend erstellen. Wichtig Resync funktion.
- [ ] B **Speicherformat der Tabelle** Eine Struktur die alles managed rund um Turnierplan speichern und bearbeiten. Konzeptionel und als blueprint unter docs speichern.
- [ ] B **Globale Konfigfile für Backend und Frontend** Konfiguriert URl für die Backend Verbindung. Und weiteres Nötige.
- [ ] B **Server Konfigfile** Erstellen einer Konfigfile für wichtige Server Parameter (Threads, Refreshrate ...).
- [ ] B **Turnierplan Speicherformat Implementieren Backend** Das Konzept der Speicherung und bearbeitung auf dem Server bauen.
- [ ] B **Turnierplan Speicherformat Implementieren Frontend** Das Konzept der Speicherung und bearbeitung in dem Clientcode bauen.
- [ ] B **Clients soll Updates zum Turnierverlauf senden** Implementierung, sodass der Client Änderungen (punkte, Gewinner etc) an den Server schicken kann.
- [ ] B **Server soll Updates von Clients bearbeiten** Anehmen, Überprüfen, Speichern und Update senden, per SSE.
- [ ] B **Client Webinterface gestallten** Die WEB-UI gestallten konzeptionel.
- [ ] B **Client Webinterface implementieren** Die WEB-UI nach dem Konzept bauen.
- [ ] B **Team-Management Logik** CRUD-Operationen für Teams (bevor das Turnier startet).
- [ ] B **Turnier-Generator (K.O.-System, Gruppenphase)** Die Logik, die aus n Teams die erste Runde berechnet (inkl. Freilose/Byes).
- [ ] B **Spielfeld-Tracker** Ein Modul, das verwaltet, welches Spiel gerade auf welchem Feld stattfindet.
- [ ] B **Rollen/Berechtigungen** Darf dieser Client gerade dieses Ergebnis eintragen? (Simpler Token oder Passwort pro Turnier).
- [ ] B **Daten-Persistenz** Eine Datenbank (SQLite/PostgreSQL).

---
Neue Feature als `- [ ] B Name` hinzufügen.*
