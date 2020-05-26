#include <libchess/position.hpp>
#include <string>
#include <vector>
#include "catch.hpp"

const std::string fens[] = {
    {"startpos"},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"},
    {"r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1"},
    {"2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23"},
    {"4k3/8/4r3/3pP3/8/8/8/4K3 w - d6 0 2"},
    {"8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - c3 0 1"},
};

TEST_CASE("Position::legal_captures()") {
    for (const auto &fen : fens) {
        INFO(fen);

        auto pos = libchess::Position{fen};
        const auto legal_moves = pos.legal_moves();
        const auto legal_captures = pos.legal_captures();
        const auto legal_noncaptures = pos.legal_noncaptures();

        // Count
        int count_capturing = 0;
        int count_noncapturing = 0;
        for (const auto &move : legal_moves) {
            if (move.is_capturing()) {
                count_capturing++;
            } else {
                count_noncapturing++;
            }
        }

        REQUIRE(count_capturing == legal_captures.size());
        REQUIRE(count_noncapturing == legal_noncaptures.size());
        REQUIRE(count_capturing + count_noncapturing == legal_moves.size());

        for (const auto &move : legal_captures) {
            // is_capturing()
            REQUIRE(move.is_capturing());

            // Find in legal moves
            bool found = false;
            for (const auto &asd : legal_moves) {
                if (asd == move) {
                    found = true;
                    break;
                }
            }
            REQUIRE(found);

            // Do not find in legal_noncaptures
            found = false;
            for (const auto &asd : legal_noncaptures) {
                if (asd == move) {
                    found = true;
                    break;
                }
            }
            REQUIRE(!found);
        }

        for (const auto &move : legal_noncaptures) {
            // is_capturing()
            REQUIRE(!move.is_capturing());

            // Find in legal moves
            bool found = false;
            for (const auto &asd : legal_moves) {
                if (asd == move) {
                    found = true;
                    break;
                }
            }
            REQUIRE(found);

            // Do not find in legal_captures
            found = false;
            for (const auto &asd : legal_captures) {
                if (asd == move) {
                    found = true;
                    break;
                }
            }
            REQUIRE(!found);
        }
    }
}
