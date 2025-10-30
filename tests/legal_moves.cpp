#include <algorithm>
#include <libchess/position.hpp>
#include <ranges>
#include <string>
#include <vector>
#include "catch.hpp"

TEST_CASE("Position::legal_captures()") {
    const std::array<std::string, 6> fens = {{
        "startpos",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1",
        "2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23",
        "4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2",
        "8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - c3 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);

        const auto pos = libchess::Position{fen};
        const auto legal_moves = pos.legal_moves();
        const auto legal_captures = pos.legal_captures();
        const auto legal_noncaptures = pos.legal_noncaptures();

        REQUIRE(legal_captures.size() + legal_noncaptures.size() == legal_moves.size());

        const auto num_captures = std::ranges::count_if(legal_moves, [](const auto &move) {
            return move.is_capturing();
        });
        REQUIRE(static_cast<uint64_t>(num_captures) == static_cast<uint64_t>(legal_captures.size()));

        for (const auto &move : legal_captures) {
            REQUIRE(move.is_capturing());
            REQUIRE(std::ranges::find(legal_moves, move) != legal_moves.end());
            REQUIRE(std::ranges::find(legal_noncaptures, move) == legal_noncaptures.end());
        }

        for (const auto &move : legal_noncaptures) {
            REQUIRE(!move.is_capturing());
            REQUIRE(std::ranges::find(legal_moves, move) != legal_moves.end());
            REQUIRE(std::ranges::find(legal_captures, move) == legal_captures.end());
        }
    }
}
