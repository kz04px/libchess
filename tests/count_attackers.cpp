#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

const std::pair<int, std::string> positions[] = {
    {0, "startpos"},
    {7, "k7/3r4/3Pn3/2p1p3/1r1K3q/8/1Pn5/b5b1 w - - 0 1"},
};

TEST_CASE("Position::count_attackers()") {
    for (const auto& [ans, fen] : positions) {
        libchess::Position pos{fen};
        const auto ksq = pos.king_position(pos.turn());
        INFO(fen);
        CHECK(pos.count_attackers(ksq, !pos.turn()) == ans);
    }
}
