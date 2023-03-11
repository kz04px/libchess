#include <array>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Move strings") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 14> tests = {{
        // Castling
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1g1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1c1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8g8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8c8"},
        // Others
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1d1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1d2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1e2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1f1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1f2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8d8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8d7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8e7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8f8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8f7"},
    }};

    for (const auto &[fen, movestr] : tests) {
        const auto pos = libchess::Position{fen};
        const auto moves = pos.legal_moves();

        auto found = false;
        for (const auto &move : moves) {
            if (pos.move_string(move) == movestr) {
                found = true;
                break;
            }
        }

        REQUIRE(found);
    }
}

TEST_CASE("Move strings DFRC") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 28> tests = {{
        // Castling
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1h1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1a1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8h8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8a8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1h1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1a1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8h8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8a8"},
        // Others
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1d1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1d2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1e2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1f1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1f2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8d8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8d7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8e7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8f8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8f7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1d1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1d2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1e2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1f1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "e1f2"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8d8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8d7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8e7"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8f8"},
        {"r3k2r/8/8/8/8/8/8/R3K2R b HAha - 0 1", "e8f7"},
    }};

    for (const auto &[fen, movestr] : tests) {
        const auto pos = libchess::Position{fen, true};
        const auto moves = pos.legal_moves();

        auto found = false;
        for (const auto &move : moves) {
            const auto str = pos.move_string(move, true);

            REQUIRE(str == static_cast<std::string>(move));

            if (str == movestr) {
                found = true;
            }
        }

        REQUIRE(found);
    }
}
