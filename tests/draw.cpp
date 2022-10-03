#include <array>
#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

TEST_CASE("Position::threefold() static") {
    using pair_type = std::pair<std::string, bool>;

    const std::array<pair_type, 4> tests = {{
        {"startpos", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 101 1", false},
    }};

    for (const auto &[fen, threefold] : tests) {
        const auto pos = libchess::Position{fen};
        REQUIRE(pos.threefold() == threefold);
    }
}

TEST_CASE("Position::threefold() positive sequence") {
    using pair_type = std::pair<std::string, std::vector<std::string>>;

    const std::array<pair_type, 2> tests = {{
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1", "f6g8"}},
        {"r2q1rk1/pp1bppbp/2np1np1/8/2BNP3/2N1BP2/PPPQ2PP/R3K2R w KQ - 5 10",
         {"c3a4", "c6a5", "a4c3", "a5c6", "c3a4", "c6a5", "a4c3", "a5c6"}},
    }};

    for (const auto &[fen, moves] : tests) {
        auto pos = libchess::Position(fen);
        for (const auto &movestr : moves) {
            REQUIRE(!pos.threefold());
            pos.makemove(movestr);
        }
        // Terminal position threefold
        REQUIRE(pos.threefold());
    }
}

TEST_CASE("Position::threefold() negative sequence") {
    using pair_type = std::pair<std::string, std::vector<std::string>>;

    const std::array<pair_type, 3> tests = {{
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
    }};

    for (const auto &[fen, moves] : tests) {
        auto pos = libchess::Position(fen);
        for (const auto &movestr : moves) {
            REQUIRE(!pos.threefold());
            pos.makemove(movestr);
        }
        // Terminal position not threefold
        REQUIRE(!pos.threefold());
    }
}

TEST_CASE("Position::fiftymoves()") {
    using pair_type = std::pair<std::string, bool>;

    const std::array<pair_type, 4> tests = {{
        {"startpos", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1", true},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 101 1", true},
    }};

    for (const auto &[fen, ans] : tests) {
        auto pos = libchess::Position{fen};
        REQUIRE(pos.fiftymoves() == ans);
    }
}

TEST_CASE("Position::is_draw() fiftymoves") {
    const std::array<std::pair<std::string, bool>, 5> tests = {{
        {"startpos", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1", false},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1", true},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 101 1", true},
        // More than fiftymoves according to fen, but checkmate
        {"3k1R2/8/3K4/8/8/8/8/8 b - - 110 1", false},
    }};

    for (const auto &[fen, ans] : tests) {
        auto pos = libchess::Position{fen};
        REQUIRE(pos.is_draw() == ans);
    }
}

TEST_CASE("Position::is_draw() threefold") {
    using test_type = std::tuple<std::string, std::vector<std::string>, bool>;

    const std::array<test_type, 5> tests = {{
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1", "f6g8"}, true},
        {"r2q1rk1/pp1bppbp/2np1np1/8/2BNP3/2N1BP2/PPPQ2PP/R3K2R w KQ - 5 10",
         {"c3a4", "c6a5", "a4c3", "a5c6", "c3a4", "c6a5", "a4c3", "a5c6"},
         true},
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1"}, false},
        {"startpos", {"g1f3", "g8f6", "f3g1", "f6g8", "e2e3", "e7e6", "g1f3", "g8f6", "f3g1", "f6g8"}, false},
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
          "a5c6"},
         false},
    }};

    for (const auto &[fen, moves, ans] : tests) {
        auto pos = libchess::Position(fen);
        for (const auto &movestr : moves) {
            REQUIRE(!pos.is_draw());
            pos.makemove(movestr);
        }
        // Terminal position threefold
        REQUIRE(pos.is_draw() == ans);
        REQUIRE(pos.is_terminal() == ans);
    }
}
