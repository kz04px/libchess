#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

const std::pair<libchess::Bitboard, std::string> positions[] = {
    {0x0, "startpos"},
    {0x1c141c000000, "b2r2b1/8/2PPP3/q1PKP1r1/2PPP3/8/b2r2q1/7k w - - 0 1"},
    {0x0, "B2R4/8/2PPP2k/Q1PKP3/2PPP3/8/3R4/8 w - - 0 1"},
    {0x0, "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"},
    {0x30000000, "7k/8/4r2b/8/4NN2/4K3/8/8 w - - 0 1"},
    {0x0, "7k/8/4r2b/4N1N1/4NN2/4K3/8/8 w - - 0 1"},
    {0x0, "7k/8/4r2b/4n1n1/4NN2/4K3/8/8 w - - 0 1"},
    {0x0, "R3k2r/8/8/8/8/8/8/4K2R b Kk - 0 1"},
    {0x0, "1k6/8/8/1B6/4Q3/3PP3/1R1PK3/8 w - - 0 1"},
    {0x1000000000, "4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2"},
};

TEST_CASE("Position::pinned()") {
    for (const auto& [ans, fen] : positions) {
        libchess::Position pos{fen};
        INFO(fen);
        CHECK(pos.pinned(pos.turn()) == ans);
    }
}
