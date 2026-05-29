```mermaid
classDiagram
    class QMainWindow {
        <<Framework>>
    }
    

    class MainWindow {
        -Ui::MainWindow* ui
        +updateTournamentUi()
        .. Signals ..
        +requestMatchUpdate(QString matchId, int p1, int p2, int status)
    }

    class KoMatch {
        -Ui::KoMatch* ui
        -Match m_match
        -QString m_state
        +KoMatch(const Match& match)
        -on_pushButton_clicked()
        .. Signals ..
        +matchResultChanged(QString matchId, int p1, int p2, int status)
    }

    class NetworkController {
        -QNetworkAccessManager* networkManager
        -QNetworkReply* sseReply
        +NetworkController(QObject* parent)
        +startSseStream(const QString& hostUrl)
        +sendMatchUpdate(const QString& matchId, int points1, int points2, int statusEnum)
        +triggerInitialBroadcast()
        -handleSseReadyRead()
        -handleSseFinished()
        .. Signals ..
        +tournamentUpdated(const std::string& rawJson)
    }

    class Match {
        +uuids::uuid id
        +Team team1
        +Team team2
        +int[2] score
        +std::string status
        +setScore(int s1, int s2)
    }

    class Team {
        -uuids::uuid id
        -std::string name
        +getName() std::string
        +getId() uuids::uuid
    }

    class JsonInterpreter {
        <<Utility>>
        +jsontoclasses(std::string& rawJson)
    }

    QMainWindow <|-- MainWindow

    MainWindow "1" *-- "many" KoMatch : dynamische Instanziierung im QGridLayout
    KoMatch "1" o-- "1" Match : kapselt lokales Datenmodell
    Match "1" *-- "2" Team : referenziert Kontrahenten
    
    MainWindow ..> NetworkController : Event-Kopplung via Qt Signal/Slot
    NetworkController ..> JsonInterpreter : delegiert textuellen Inbound
    JsonInterpreter ..> Match : bearbeitet globale Zustandsvektoren
    JsonInterpreter ..> Team : bearbeitet globale Zustandsvektoren
  ```