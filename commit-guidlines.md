# Git Commit Guide: Conventional Commits

Dieser Standard hilft dabei, die Projekthistorie sauber und automatisiert auswertbar zu halten.

## Die gängigsten Typen

| Präfix | Beschreibung |
| :--- | :--- |
| **feat** | Eine neue Funktion für den Anwender. |
| **fix** | Eine Fehlerbehebung. |
| **docs** | Änderungen an der Dokumentation (README, Kommentare). |
| **style** | Formatierungen, fehlende Semikolons etc. (keine Logikänderung). |
| **refactor** | Code-Verbesserung, die weder ein Bugfix noch ein Feature ist. |
| **perf** | Änderungen, die die Performance verbessern. |
| **test** | Hinzufügen oder Korrigieren von Tests. |
| **build** | Änderungen am Build-System oder externen Abhängigkeiten (npm, Maven). |
| **ci** | Änderungen an der CI-Konfiguration (GitHub Actions, Jenkins). |
| **chore** | Sonstige Aufgaben, die nicht den Source-Code oder Tests betreffen. |

## Struktur eines Commits

```text
<typ>(<bereich>): <beschreibung>

Beispiel:
feat(frontend): Neues irgendwas
