#include <array>
#include <cstdint>
#include <libchess/bitboard.hpp>
#include "catch.hpp"

TEST_CASE("BitboardIterator") {
    const std::array<std::uint64_t, 2> masks = {{
        0x0,
        0x101010101010101,
    }};

    for (const auto &mask : masks) {
        const libchess::Bitboard bb{mask};

        libchess::Bitboard nbb;
        for (const auto &sq : bb) {
            nbb ^= sq;
        }
        REQUIRE(bb == nbb);
    }
}
