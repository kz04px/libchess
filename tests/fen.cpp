#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

const std::string fens[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
};

TEST_CASE("Position::get_fen()") {
    for (const auto& fen : fens) {
        libchess::Position pos{fen};
        REQUIRE(pos.get_fen() == fen);
    }
}
