# Coding Guidelines

Dieses Dokument definiert die Programmierstandards für unser Projekt, um eine konsistente und wartbare Codebasis zu gewährleisten.

## 1. Benennungskonventionen (Naming Conventions)

| Element | Stil | Beispiel |
| :--- | :--- | :--- |
| **Klassen** | PascalCase (UpperCaps) | `UserRepository`, `TaskController` |
| **Variablen** | camelCase (empfohlen) | `userCount`, `isValid` |
| **Funktionen** | camelCase | `calculateTotal()`, `fetchData()` |
| **Konstanten** | UPPER_SNAKE_CASE | `MAX_RETRY_LIMIT`, `API_URL` |
| **Dateinamen** | kebab-case | `user-service.js`, `auth-provider.ts` |

---

## 2. Allgemeine Prinzipien

### 2.1 Aussagekräftige Namen
Vermeide Abkürzungen oder einzelne Buchstaben (außer in kurzen Schleifen wie `i`).
* ❌ `let d = new Date();`
* ✅ `let currentDate = new Date();`

## 2.2 Keine Umlaute:
In Code (Variablen, Klassen, Kommentaren) und Dateinamen sind `ä`, `ö`, `ü` und `ß` strengstens untersagt.
* Nutzen Sie stattdessen: `ae`, `oe`, `ue`, `ss`.

### 2.3 Funktionen
* Funktionen sollten **eine** Sache tun (Single Responsibility).
* Namen sollten Verben enthalten: `getUser()` statt `user()`.

---

## 3. Formatierung

* **Einrückung:** 2 oder 4 Leerzeichen (keine Tabs verwenden). In den meisten Editoren kann man Tabs zu Spacces machen.
* **Klammern:** Öffnende geschweifte Klammern `{` bleiben in der Regel in der gleichen Zeile wie die Anweisung.
