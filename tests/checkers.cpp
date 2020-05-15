#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

const std::pair<libchess::Bitboard, std::string> positions[] = {
    {0x0, "startpos"},
    {0x101482000440, "k7/3r4/3Pn3/2p1p3/1r1K3q/8/1Pn5/b5b1 w - - 0 1"},
};

TEST_CASE("Position::checkers()") {
    for (const auto& [ans, fen] : positions) {
        libchess::Position pos{fen};
        INFO(fen);
        CHECK(pos.checkers() == ans);
    }
}
