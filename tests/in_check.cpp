#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::in_check() - true") {
    const std::array<std::string, 16> fens = {{
        "4k3/8/3N4/8/8/8/8/4K3 b - - 0 1",
        "4k3/8/2B5/8/8/8/8/4K3 b - - 0 1",
        "4k3/8/2Q5/8/8/8/8/4K3 b - - 0 1",
        "4k3/8/4Q3/8/8/8/8/4K3 b - - 0 1",
        "4k3/8/4R3/8/8/8/8/4K3 b - - 0 1",
        "1R2k3/8/8/8/8/8/8/4K3 b - - 0 1",
        "4k3/3P4/8/8/8/8/8/4K3 b - - 0 1",
        "4k3/5P2/8/8/8/8/8/4K3 b - - 0 1",
        "4K3/8/3n4/8/8/8/8/4k3 w - - 0 1",
        "4K3/8/2b5/8/8/8/8/4k3 w - - 0 1",
        "4K3/8/4q3/8/8/8/8/4k3 w - - 0 1",
        "4K3/8/2q5/8/8/8/8/4k3 w - - 0 1",
        "2r1K3/8/8/8/8/8/8/4k3 w - - 0 1",
        "4K3/8/4r3/8/8/8/8/4k3 w - - 0 1",
        "8/3p4/4K3/8/8/8/8/4k3 w - - 0 1",
        "8/5p2/4K3/8/8/8/8/4k3 w - - 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen};
        REQUIRE(pos.in_check());
    }
}

TEST_CASE("Position::in_check() - false") {
    const std::array<std::string, 2> fens = {{
        "startpos",
        "1Q2Q2Q/8/3ppp2/1R1pkp1R/3ppp2/8/1B2Q2B/4K3 b - - 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen};
        REQUIRE_FALSE(pos.in_check());
    }
}
