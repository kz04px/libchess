#include <cstdint>
#include <libchess/bitboard.hpp>
#include "catch.hpp"

const std::uint64_t masks[] = {
    0x0,
    0x101010101010101,
};

TEST_CASE("BitboardIterator") {
    for (const auto& mask : masks) {
        libchess::Bitboard bb{mask};
        libchess::Bitboard nbb;

        for (const auto& sq : bb) {
            nbb ^= sq;
        }

        REQUIRE(bb == nbb);
    }
}
