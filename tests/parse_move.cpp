#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::parse_move()") {
    const std::array<std::string, 10> fens = {{
        "startpos",
        "4k3/1P6/8/2p2pPp/6p1/8/P1PPP3/4K3 w - - 0 1",
        "4k3/8/4p3/5PP1/1p4pP/1n6/1N6/4K3 b - - 0 1",
        "4k3/1P6/8/8/8/8/Pp6/4K3 w - - 0 1",
        "8/1p2k1p1/8/8/2P4p/7Q/2p5/4K3 b - - 0 1",
        "6B1/8/8/8/1Pp5/8/k7/4K3 b - b3 0 2",
        "1B6/8/8/8/5pP1/8/7k/4K3 b - g3 0 2",
        "4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1",
        "4k3/8/8/8/2pPp3/8/8/4K3 b - d3 0 1",
        "8/8/8/4k3/5Pp1/8/8/3K4 b - f3 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const auto pos = libchess::Position{fen};
        const auto moves = pos.legal_moves();
        for (const auto &move : moves) {
            const auto str = static_cast<std::string>(move);
            const auto nmove = pos.parse_move(str);
            REQUIRE(nmove == move);
        }
    }
}

TEST_CASE("Position::parse_move() success") {
    const std::array<std::string, 6> strings = {{
        "a2a3",
        "a2a4",
        "b2b3",
        "c4c5",
        "d4d5",
        "g1f3",
    }};

    const auto pos = libchess::Position{"rnbqk2r/pppp1ppp/4pn2/8/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4"};
    for (const auto &movestring : strings) {
        REQUIRE_NOTHROW(pos.parse_move(movestring));
    }
}

TEST_CASE("Position::parse_move() exceptions") {
    const std::array<std::string, 7> strings = {{
        "",
        "0000",
        "longlonglong",
        "c3d5",
        "b5c4",
        "h1g1",
        "a1a1",
    }};

    const auto pos = libchess::Position{"rnbqk2r/pppp1ppp/4pn2/8/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4"};
    for (const auto &movestring : strings) {
        REQUIRE_THROWS(pos.parse_move(movestring));
    }
}
