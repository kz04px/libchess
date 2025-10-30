#include <array>
#include <libchess/position.hpp>
#include <ranges>
#include <string>
#include <vector>
#include "catch.hpp"

TEST_CASE("Hash changes") {
#ifndef NO_HASH
    const std::array<std::string, 2> fens = {{
        "startpos",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
    }};

    for (const auto &fen : fens) {
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
        for (const auto &move : moves) {
            pos.makemove(move);
            REQUIRE(pos.hash() != old_hash);
            pos.undomove();
        }
    }
#endif
}

TEST_CASE("Hash changes2") {
#ifndef NO_HASH
    const std::array<std::string, 12> fens = {{
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
    }};

    std::vector<std::uint64_t> hashes;

    for (const auto &fen : fens) {
        const auto pos = libchess::Position{fen};
        hashes.push_back(pos.hash());
    }

    std::ranges::sort(hashes);
    REQUIRE(std::ranges::adjacent_find(hashes) == hashes.end());
#endif
}

void test(libchess::Position &pos, const int depth) noexcept {
    if (depth == 0) {
        return;
    }

    const auto moves = pos.legal_moves();
    for (const auto &move : moves) {
        const auto prediction = pos.predict_hash(move);
        pos.makemove(move);

        REQUIRE(static_cast<uint64_t>(pos.hash()) == static_cast<uint64_t>(prediction));

        test(pos, depth - 1);
        pos.undomove();
    }
}

TEST_CASE("Predict hash") {
#ifndef NO_HASH
    const std::array<std::string, 7> fens = {{
        "startpos",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
        "4k3/8/K6r/3pP3/8/8/8/8 w - d6 0 1",
        "k7/8/4r3/3pP3/8/8/8/4K3 w - d6 0 1",
        "1k6/8/8/8/4Pp2/8/7B/4K3 b - e3 0 2",
        "4k3/4r3/8/8/8/8/3p4/4K3 w - - 0 2",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
    }};

    for (const auto &fen : fens) {
        auto pos = libchess::Position{fen};
        test(pos, 3);
    }
#endif
}
