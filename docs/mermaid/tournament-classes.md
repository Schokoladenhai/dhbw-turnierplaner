```mermaid
classDiagram
    class Tournament {
        -TournamentStatus status
        -unsigned int currentStageIndex
        -string name
        -unordered_map~uuid, unique_ptr~Team~~ teams
        -vector~unique_ptr~Stage~~ stages
        +updateName(string newName)
        +getStatus() TournamentStatus
        +pushStage(unique_ptr~Stage~ stage)
        +popStage()
        +addTeam(unique_ptr~Team~ team)
        +rmvTeam(uuid teamId)
        +getCurrentStage() Stage*
        +start() bool
        +runNextStage(vector~uuid~ teamIds)
        +end(vector~uuid~ teamIds)
        +toJson() json
        +loadFromJson()
    }

    class Stage {
        <<abstract>>
        #unordered_map~uuid, unique_ptr~Match~~ matches
        #FinishedCallback onFinished
        +getMatchById(uuid id) Match*
        +setOnFinished(FinishedCallback cb)
        +canAcceptMatchUpdate(uuid, Score, MatchStatus) bool*
        +matchUpdate(uuid, Score, MatchStatus) bool*
        +isValidMatchResult(Score) bool*
        +generateMatches(int, int)*
        +populateMatches(vector~uuid~)*
        +getAdvancingTeams() vector~uuid~*
        +toJson() json*
    }

    class KoStage {
        -vector~uuid~ matchTree
        +matchUpdate(uuid, Score, MatchStatus) bool
        +isValidMatchResult(Score) bool
        +generateMatches(int, int)
        +populateMatches(vector~uuid~)
        +getAdvancingTeams() vector~uuid~
        +toJson() json
    }

    class Match {
        -uuid id
        -MatchStatus status
        -uuid team1Id
        -uuid team2Id
        -Score score
        +setTeam1(uuid)
        +setTeam2(uuid)
        +getTeam1() uuid
        +getTeam2() uuid
        +setnewScore(Score)
        +advanceStatus() bool
        +skip()
        +getWinner() uuid
        +getId() uuid
        +getStatus() MatchStatus
        +isReady() bool
        +toJson() json
    }

    class Team {
        -uuid id
        -string name
        -vector~string~ players
        +getId() uuid
        +addPlayer(string)
        +rmvPlayer(string)
        +toJson() json
    }

    class Score {
        <<struct>>
        +int points1
        +int points2
    }

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
        MATCH_SKIPED
    }

    %% Relationships
    Stage <|-- KoStage : inheritance
    Tournament "1" *-- "many" Stage : contains (stages)
    Tournament "1" *-- "many" Team : contains (teams)
    Stage "1" *-- "many" Match : contains (matches)
    Match "1" -- "1" Score : uses
    Match "1" -- "1" MatchStatus : uses
    Tournament "1" -- "1" TournamentStatus : uses
   ```
