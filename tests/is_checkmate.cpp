#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::is_checkmate() true") {
    const std::array<std::string, 5> tests = {{
        "8/8/8/8/8/3k4/8/3K1r2 w - - 1 2",
        "1k1R4/8/1K6/8/8/8/8/8 b - - 0 1",
        "3k1R2/8/3K4/8/8/8/8/8 b - - 1 1",
        "6k1/r2n1ppp/5n2/6K1/2PQr1q1/6P1/PP5P/RbB4R w - - 13 28",
        "8/7k/2p2bp1/1p5p/pPPqpP1P/P3K1P1/4Q3/4B3 w - - 2 48",
    }};

    for (const auto &fen : tests) {
        const auto pos = libchess::Position{fen};
        INFO(pos.get_fen());
        REQUIRE(pos.is_checkmate());
        REQUIRE(pos.is_terminal());
    }
}

TEST_CASE("Position::is_checkmate() false") {
    const std::array<std::string, 4> tests = {{
        "3k4/8/3K4/8/5R2/8/8/8 w - - 0 1",
        "r3k3/pppq2p1/2nb4/5pr1/3Pp2P/2P5/PP1NPPKP/R2Q3R w q - 0 18",
        "8/7k/2p2bp1/1p5p/pPP1pP1P/P3K1P1/4Q3/q3B3 b - - 1 47",
        "7R/6p1/5pNk/3ppB1p/6P1/2q4P/5P2/6K1 b - - 2 33",
    }};

    for (const auto &fen : tests) {
        const auto pos = libchess::Position{fen};
        INFO(pos.get_fen());
        REQUIRE(!pos.is_checkmate());
        REQUIRE(!pos.is_terminal());
    }
}
