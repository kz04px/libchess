#include <cstdint>
#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

const std::pair<std::string, std::vector<std::uint64_t>> positions[] = {
    {"startpos", {1, 20, 400, 8902}},
    {"8/4k3/8/8/8/8/4K3/8 w - - 0 1", {1, 8}},
    {"7k/8/8/8/8/4B3/8/7K w - - 0 1", {1, 14}},
    {"7k/8/8/8/8/4N3/8/7K w - - 0 1", {1, 11}},
    {"4k3/4r3/8/8/8/3p4/4P3/4K3 w - - 0 1", {1, 6}},
    // En passant is scary
    {"4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2", {1, 5, 75, 526}},
    {"4k3/8/8/K2pP2r/8/8/8/8 w - d6 0 1", {1, 6, 94, 640}},
    {"4k3/8/7r/K2pP3/8/8/8/8 w - d6 0 1", {1, 5, 89, 538}},
    {"5k2/8/8/1K1Pp1r1/8/8/8/8 w - e6 0 2", {1, 9, 134, 948}},
    {"8/8/8/8/1k1Pp2R/8/8/4K3 b - d3 0 1", {1, 8, 125, 869}},
    {"8/8/8/8/1k1PpN1R/8/8/4K3 b - d3 0 1", {1, 9, 193, 1322}},
    {"8/8/8/8/1k1Ppn1R/8/8/4K3 b - d3 0 1", {1, 17, 220, 3001}},
    {"8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - c3 0 1", {1, 21, 206, 4135}},
    {"4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1", {1, 9, 47, 376}},
    {"4k3/8/8/8/2pPp3/8/8/4K3 b - d3 0 1", {1, 9, 47, 376}},
    {"2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23",
     {1, 46, 1356, 56661}},
};

TEST_CASE("Position::perft()") {
    for (const auto& [fen, nodes] : positions) {
        libchess::Position pos{fen};
        INFO(fen);
        for (std::size_t i = 0; i < nodes.size(); ++i) {
            REQUIRE(pos.perft(i) == nodes[i]);
        }
    }
}
