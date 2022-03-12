#include <array>
#include <cstdint>
#include <libchess/position.hpp>
#include <string>
#include "catch.hpp"

TEST_CASE("Position::passed_pawns()") {
    using pair_type = std::pair<std::string, std::uint64_t>;

    const std::array<pair_type, 5> positions = {{
        {"startpos", 0x0},
        {"4k3/1P6/8/2p2pPp/6p1/8/P1PPP3/4K3 w - - 0 1", 0x2004000000100},
        {"4k3/8/4p3/5PP1/1p4pP/1n6/1N6/4K3 b - - 0 1", 0x42000000},
        {"4k3/1P6/8/8/8/8/Pp6/4K3 w - - 0 1", 0x2000000000100},
        {"8/1p2k1p1/8/8/2P4p/7Q/2p5/4K3 b - - 0 1", 0x40000080000400},
    }};

    for (const auto &[fen, passers] : positions) {
        INFO(fen);
        const auto pos = libchess::Position{fen};
        REQUIRE(pos.passed_pawns() == libchess::Bitboard(passers));
    }
}
