#include <gtest/gtest.h>
#include "match.hpp"
#include "team.hpp"

// Prüfen, ob jedes Match wirklich eine eindeutige UUID bekommt
TEST(MatchTest, UniqueIdGeneration) {
    Match m1;
    Match m2;
    EXPECT_FALSE(m1.getId().is_nil());
    EXPECT_FALSE(m2.getId().is_nil());
    EXPECT_NE(m1.getId(), m2.getId());
}

// Äquivalenzklasse: Warten auf Start
TEST(MatchTest, InitialStateAndReadiness) {
    Match m;
    EXPECT_EQ(m.getStatus(), MATCH_WAITING);
    EXPECT_FALSE(m.isReady());

    Team t1("Team 1");
    m.setTeam1(t1.getId());
    EXPECT_FALSE(m.isReady()); // Nur 1 Team reicht nicht

    Team t2("Team 2");
    m.setTeam2(t2.getId());
    EXPECT_TRUE(m.isReady()); // Genau 2 Teams = Ready
}

// Grenzwerte: Zustandsmaschine (State Machine) blockiert illegale Änderungen
TEST(MatchTest, BoundaryStateTransitions) {
    Match m;
    Team t1("T1"); Team t2("T2");
    m.setTeam1(t1.getId()); m.setTeam2(t2.getId());

    EXPECT_TRUE(m.advanceStatus()); // WAITING -> RUNNING

    // Darf nicht überschrieben werden, wenn Match schon läuft!
    Team t3("T3");
    m.setTeam1(t3.getId());
    EXPECT_EQ(m.getTeam1(), t1.getId());

    EXPECT_TRUE(m.advanceStatus()); // RUNNING -> FINISHED
    EXPECT_FALSE(m.advanceStatus()); // MATCH_FINISHED -> blockiert
}

// Äquivalenzklassen: Punkte und Gewinner
TEST(MatchTest, ScoreEquivalenceClasses) {
    Match m;
    Team t1("T1"); Team t2("T2");
    m.setTeam1(t1.getId()); m.setTeam2(t2.getId());
    m.advanceStatus(); // RUNNING
    m.advanceStatus(); // FINISHED

    m.setnewScore({5, 2});
    EXPECT_EQ(m.getWinner(), t1.getId());

    m.setnewScore({1, 3});
    EXPECT_EQ(m.getWinner(), t2.getId());

    // Unentschieden 0:0 liefert nil-UUID
    m.setnewScore({0, 0});
    EXPECT_TRUE(m.getWinner().is_nil());

    // Negative Punkte
    m.setnewScore({-5, -10});
    EXPECT_EQ(m.getWinner(), t1.getId());
}

// Edge Cases: Skip-Logik (Freilose)
TEST(MatchTest, SkipMatchVariations) {
    Team t1("T1"); Team t2("T2");

    // Variation 1: Nur Team 1 vorhanden (Freilos für Team 1)
    Match m1;
    m1.setTeam1(t1.getId());
    m1.skip();
    EXPECT_EQ(m1.getStatus(), MATCH_SKIPED);
    EXPECT_EQ(m1.getWinner(), t1.getId());

    // Variation 2: Nur Team 2 vorhanden (Freilos für Team 2)
    Match m2;
    m2.setTeam2(t2.getId());
    m2.skip();
    EXPECT_EQ(m2.getWinner(), t2.getId());

    // Variation 3: Gar kein Team vorhanden
    Match m3;
    m3.skip();
    EXPECT_TRUE(m3.getWinner().is_nil());
}

// JSON Serialisierung
TEST(MatchTest, JsonSerializationCheck) {
    Match m;
    Team t1("T1");
    m.setTeam1(t1.getId());
    m.setnewScore({10, 0});
    m.skip();

    auto j = m.toJson();
    EXPECT_EQ(j["status"], MATCH_SKIPED);
    EXPECT_EQ(j["team1"], uuids::to_string(t1.getId()));
    EXPECT_EQ(j["score"][uuids::to_string(t1.getId())], 10);
}
