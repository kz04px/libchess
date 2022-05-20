#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::is_stalemate() true") {
    const std::array<std::string, 4> tests = {{
        "k7/8/1PP5/3N2K1/5B1P/8/6P1/8 b - - 0 52",
        "8/1R6/8/8/p1N5/P4P2/k5PP/4R1K1 b - - 7 42",
        "5k2/p7/8/5n1K/3n4/8/8/2q3q1 w - - 2 39",
        "4k3/p1p5/8/7p/8/3pq3/8/3K4 w - - 0 42",
    }};

    for (const auto &fen : tests) {
        const auto pos = libchess::Position{fen};
        INFO(pos.get_fen());
        REQUIRE(pos.is_stalemate());
        REQUIRE(pos.is_terminal());
    }
}

TEST_CASE("Position::is_stalemate() false") {
    const std::array<std::string, 4> tests = {{
        "3k4/8/3K4/8/5R2/8/8/8 w - - 0 1",
        "4k3/p1p5/8/7p/3p4/4q3/8/3K4 b - - 3 41",
        "8/7k/2p2bp1/1p5p/pPP1pP1P/P3K1P1/4Q3/q3B3 b - - 1 47",
        "7R/6p1/5pNk/3ppB1p/6P1/2q4P/5P2/6K1 b - - 2 33",
    }};

    for (const auto &fen : tests) {
        const auto pos = libchess::Position{fen};
        INFO(pos.get_fen());
        REQUIRE(!pos.is_stalemate());
        REQUIRE(!pos.is_terminal());
    }
}
