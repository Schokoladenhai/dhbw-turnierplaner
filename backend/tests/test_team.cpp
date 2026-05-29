#include <gtest/gtest.h>
#include "team.hpp"
#include "uuid-gen.hpp"
#include <string>

// Standard-Initialisierung und generierte UUID
TEST(TeamTest, Initialization) {
    Team t("Die wilden Tiger");
    EXPECT_FALSE(t.getId().is_nil());
    EXPECT_EQ(t.toJson()["name"], "Die wilden Tiger");
}

// Grenzwert: Team mit eigener (injizierter) UUID erstellen
TEST(TeamTest, InitializationWithCustomUUID) {
    uuids::uuid customId = generateUuid();
    Team t("Custom Team", customId);
    EXPECT_EQ(t.getId(), customId);
    EXPECT_EQ(t.toJson()["id"], uuids::to_string(customId));
}

// Grenzwert: Arbeiten mit 0 Spielern
TEST(TeamTest, BoundaryEmptyTeam) {
    Team t("Leeres Team");
    EXPECT_NO_THROW(t.rmvPlayer("Geist"));
    EXPECT_TRUE(t.toJson()["players"].empty());
}

// Äquivalenzklasse & Grenzwert: Hinzufügen und Entfernen
TEST(TeamTest, AddAndRemovePlayersBoundaries) {
    Team t("Test Team");
    t.addPlayer("Alice");
    t.addPlayer("Bob");

    // Ungültige Eingabe: Spieler existiert nicht
    t.rmvPlayer("Charlie");
    EXPECT_EQ(t.toJson()["players"].size(), 2);

    // Grenzwert: Den letzten verbleibenden Spieler entfernen
    t.rmvPlayer("Bob");
    t.rmvPlayer("Alice");
    EXPECT_TRUE(t.toJson()["players"].empty());
}

// Edge Case: Gleichen Spieler mehrfach hinzufügen
TEST(TeamTest, DuplicatePlayersHandling) {
    Team t("Klonkrieger");
    t.addPlayer("Jango");
    t.addPlayer("Jango");
    t.addPlayer("Boba");

    auto j = t.toJson();
    EXPECT_EQ(j["players"].size(), 3);

    // std::erase in C++20 entfernt ALLE Vorkommen dieses Wertes
    t.rmvPlayer("Jango");
    j = t.toJson();
    EXPECT_EQ(j["players"].size(), 1);
    EXPECT_EQ(j["players"][0], "Boba");
}

// Edge Case: Leerer String als Name
TEST(TeamTest, EmptyPlayerName) {
    Team t("Team Ghost");
    t.addPlayer("");
    EXPECT_EQ(t.toJson()["players"].size(), 1);
    EXPECT_EQ(t.toJson()["players"][0], "");
}
