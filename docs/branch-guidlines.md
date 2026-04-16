---

# Git Branching Guidelines

Um den `main` Branch stabil zu halten und die Zusammenarbeit zu strukturieren, nutzen wir ein präfix-basiertes Branching-Modell.

## Branch-Namensschema

Struktur: `<typ>/<kurzbeschreibung>` oder `<typ>/<ticket-id>-<kurzbeschreibung>`

### Gängige Typen

| Präfix | Verwendung | Beispiel |
| :--- | :--- | :--- |
| **feature/** | Neue Funktionen oder Erweiterungen. | `feature/user-login` |
| **bugfix/** | Behebung von Fehlern in der Entwicklung. | `bugfix/header-mobile-glitch` |
| **hotfix/** | Kritische Fixes für den Produktions-Code. | `hotfix/security-patch-v1` |
| **docs/** | Reine Dokumentationsänderungen. | `docs/api-endpoints` |
| **refactor/** | Code-Umstrukturierung ohne Logikänderung. | `refactor/clean-up-database-logic` |
| **chore/** | Wartungsarbeiten, Updates, Tooling. | `chore/update-dependencies` |

## Workflow-Regeln

1.  **Keine direkten Commits auf `main`:** Alle Änderungen erfolgen über einen Branch.
2.  **Branch-Start:** Erstelle einen neuen Branch immer vom aktuellen `main`.
    ```bash
    git checkout main
    git pull
    git checkout -b feature/mein-neues-feature
    ```
3.  **Pull Requests:** Sobald die Arbeit fertig ist, wird ein Pull Request (PR) erstellt. Der Branch wird erst nach erfolgreichem Review in den `main` gemerged.
4.  **Cleanup:** Nach dem erfolgreichen Merge sollte der lokale und Remote-Branch gelöscht werden, um das Repo sauber zu halten.
    ```bash
    git branch -d feature/mein-neues-feature
    ```

## Goldene Regel: Atomare Branches
Halte Branches so klein wie möglich. Ändere nicht die Überschrift der Doku im selben Branch, in dem du einen Bug im Login-System fixst. **Ein Branch = Eine Aufgabe.**
