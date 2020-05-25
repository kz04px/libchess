#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

const std::string fens[] = {
    "startpos",
    "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8 11",
    "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0 13",
    "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 3",
    "4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2",
    "4k3/8/8/K2pP2r/8/8/8/8 w - d6 0 1",
    "4k3/8/7r/K2pP3/8/8/8/8 w - d6 0 1",
    "5k2/8/8/1K1Pp1r1/8/8/8/8 w - e6 0 2",
};

void valid(libchess::Position &pos, const int depth) noexcept {
    if (depth == 0) {
        return;
    }

    {
        const auto halfmoves = pos.halfmoves();
        const auto fullmoves = pos.fullmoves();
        const auto hash = pos.hash();
        const auto turn = pos.turn();

        pos.makenull();

        REQUIRE(hash != pos.calculate_hash());
        REQUIRE(pos.hash() == pos.calculate_hash());

        valid(pos, depth - 1);
        pos.undonull();

        REQUIRE(halfmoves == pos.halfmoves());
        REQUIRE(fullmoves == pos.fullmoves());
        REQUIRE(hash == pos.hash());
        REQUIRE(turn == pos.turn());
    }

    const auto moves = pos.legal_moves();
    for (const auto &move : moves) {
        const auto halfmoves = pos.halfmoves();
        const auto fullmoves = pos.fullmoves();
        const auto hash = pos.hash();
        const auto turn = pos.turn();
        REQUIRE(pos.hash() == pos.calculate_hash());

        pos.makemove(move);
        valid(pos, depth - 1);
        pos.undomove();

        REQUIRE(halfmoves == pos.halfmoves());
        REQUIRE(fullmoves == pos.fullmoves());
        REQUIRE(hash == pos.hash());
        REQUIRE(turn == pos.turn());
    }
}

TEST_CASE("Consistency") {
    for (const auto &fen : fens) {
        libchess::Position pos{fen};
        valid(pos, 3);
    }
}
