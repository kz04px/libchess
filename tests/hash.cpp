#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Hash changes") {
#ifdef NO_HASH
    return;
#else
    const std::string fens[] = {
        "startpos",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
    };

    for (const auto& fen : fens) {
        auto pos = libchess::Position{fen};
        const auto old_hash = pos.hash();

        // Nullmove
        {
            pos.makenull();
            REQUIRE(pos.hash() != old_hash);
            pos.undonull();
        }

        // Legal moves
        const auto moves = pos.legal_moves();
        for (const auto& move : moves) {
            pos.makemove(move);
            REQUIRE(pos.hash() != old_hash);
            pos.undomove();
        }
    }
#endif
}

TEST_CASE("Hash changes2") {
#ifdef NO_HASH
    return;
#else
    const std::string fens[] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQk - 0 1",
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
    };

    std::vector<std::uint64_t> hashes;

    for (const auto& fen : fens) {
        auto pos = libchess::Position{fen};
        hashes.push_back(pos.hash());
    }

    for (std::size_t i = 0; i < hashes.size() - 1; ++i) {
        for (std::size_t j = i + 1; j < hashes.size(); ++j) {
            INFO(fens[i] + " vs " + fens[j]);
            REQUIRE(hashes[i] != hashes[j]);
        }
    }
#endif
}
