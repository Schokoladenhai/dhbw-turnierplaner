```mermaid
classDiagram
    direction TB

    %% Enums and Structs
    class TournamentStatus {
        <<enumeration>>
        TOURNAMENT_SETUP
        TOURNAMENT_RUNNING
        TOURNAMENT_FINISHED
    }

    class MatchStatus {
        <<enumeration>>
        MATCH_WAITING
        MATCH_RUNNING
        MATCH_FINISHED
    }

    class Score {
        <<struct>>
        +int points1
        +int points2
    }

    class MatchNode {
        <<struct>>
        +Match* match
        +uint16_t Index
    }

    %% Main Classes
    class Tournament {
        -TournamentStatus status
        -unsigned int currentStageIndex
        -string name
        -unordered_map<uuid, unique_ptr<Team>> teams
        -vector<unique_ptr<Stage>> stages
        -runNextStage(vector<Team*> teams)
        +Tournament()
        +updateName(string newName)
        +addStage(unique_ptr<Stage> stage)
        +rmvStage(Stage*)
        +addTeam(unique_ptr<Team> team)
        +rmvTeam(uuid teamId)
        +start()
        +end()
        +saveToJson()
        +loadFromJson()
    }

    class Stage {
        <<abstract>>
        #unordered_map<uuid, unique_ptr<Match>> matches
        #FinishedCallback onFinished
        +virtual ~Stage()
        +getMatches() unordered_map
        +setOnFinished(FinishedCallback cb)
        +virtual generateMatches(vector<uuid> participants)*
        +virtual getAdvancingTeams()* vector<uuid>
    }

    class KoStage {
        -unordered_map<uuid, MatchNode> matchTree
        +generateMatches(vector<uuid> participants)
        +getAdvancingTeams() vector<uuid>
    }

    class Match {
        -uuid id
        -MatchStatus status
        -uuid team1Id
        -uuid team2Id
        -Score score
        +Match()
        +setResults(int points1, int points2)
        +getWinner() uuid
        +getId() uuid
        +getStatus() MatchStatus
        +isReady() bool
    }

    class Team {
        -uuid id
        -string name
        -vector<string> players
        +Team()
        +getId() uuid
        +addPlayer(string name)
        +rmvPlayer(string name)
    }

    %% Relationships
    Tournament "1" *-- "many" Stage : owns (unique_ptr)
    Tournament "1" *-- "many" Team : owns (unique_ptr)
    Tournament ..> TournamentStatus : uses

    Stage "1" *-- "many" Match : owns (unique_ptr)
    Stage <|-- KoStage : inheritance

    KoStage "1" o-- "many" MatchNode : contains
    MatchNode --> Match : references

    Match "1" *-- "1" Score : contains
    Match ..> MatchStatus : uses
   ```
