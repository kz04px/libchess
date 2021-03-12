#include <iostream>
#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

TEST_CASE("Position::threefold() static") {
    const std::pair<bool, std::string> tests[] = {
        {false, "startpos"},
        {false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1"},
        {false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1"},
        {false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 101 1"},
    };

    for (const auto& [ans, fen] : tests) {
        auto pos = libchess::Position{fen};
        REQUIRE(pos.threefold() == ans);
    }
}

TEST_CASE("Position::threefold() positive sequence") {
    const std::pair<std::string, std::vector<std::string>> tests[] = {
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1", "f6g8"}},
        {"r2q1rk1/pp1bppbp/2np1np1/8/2BNP3/2N1BP2/PPPQ2PP/R3K2R w KQ - 5 10",
         {"c3a4", "c6a5", "a4c3", "a5c6", "c3a4", "c6a5", "a4c3", "a5c6"}},
    };

    for (const auto& [fen, moves] : tests) {
        auto pos = libchess::Position(fen);
        for (const auto& movestr : moves) {
            REQUIRE(!pos.threefold());
            pos.makemove(movestr);
        }
        // Terminal position threefold
        REQUIRE(pos.threefold());
    }
}

TEST_CASE("Position::threefold() negative sequence") {
    const std::pair<std::string, std::vector<std::string>> tests[] = {
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1"}},
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "e2e3", "e7e6", "g1f3", "g8f6", "f3g1", "f6g8"}},
        // Make sure castling permissions are considered
        {"r2q1rk1/pp1bppbp/2np1np1/8/2BNP3/2N1BP2/PPPQ2PP/R3K2R w KQ - 5 10",
         {"c3a4",
          "c6a5",
          "a4c3",
          "a5c6",
          "e1g1",
          "d7e6",
          "g1f2",
          "e6d7",
          "f2e1",
          "d7e6",
          "f1h1",
          "e6d7",
          "c3a4",
          "c6a5",
          "a4c3",
          "a5c6"}},
    };

    for (const auto& [fen, moves] : tests) {
        auto pos = libchess::Position(fen);
        for (const auto& movestr : moves) {
            REQUIRE(!pos.threefold());
            pos.makemove(movestr);
        }
        // Terminal position not threefold
        REQUIRE(!pos.threefold());
    }
}

TEST_CASE("Position::fiftymoves()") {
    const std::pair<bool, std::string> tests[] = {
        {false, "startpos"},
        {false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1"},
        {true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1"},
        {true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 101 1"},
    };

    for (const auto& [ans, fen] : tests) {
        auto pos = libchess::Position{fen};
        REQUIRE(pos.fiftymoves() == ans);
    }
}
