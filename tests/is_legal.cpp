#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

const std::string fens[] = {
    {"startpos"},
};

TEST_CASE("Position::is_legal()") {
    for (const auto &fen : fens) {
        libchess::Position pos{fen};
        const auto moves = pos.legal_moves();
        INFO(fen);
        for (const auto &move : moves) {
            CHECK(pos.is_legal(move));
        }
    }
}
