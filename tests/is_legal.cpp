#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::is_legal()") {
    const std::array<std::string, 7> fens = {{
        "startpos",
        "2rqr1k1/pp1bppb1/3p1npB/4n2p/3NP2P/1BN2P2/PPPQ2P1/1K1R3R b - - 1 14",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1",
        "2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23",
        "4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2",
        "8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - c3 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen};
        const auto moves = pos.legal_moves();
        for (const auto &move : moves) {
            REQUIRE(pos.is_legal(move));
        }
    }
}
