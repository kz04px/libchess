#include <cstdint>
#include <libchess/bitboard.hpp>
#include <libchess/movegen.hpp>
#include <libchess/square.hpp>
#include <tuple>
#include "catch.hpp"

TEST_CASE("Movegen knights") {
    using pair_type = std::pair<libchess::Square, std::uint64_t>;

    const std::array<pair_type, 3> positions = {{
        {libchess::squares::A1, 0x20400},
        {libchess::squares::D4, 0x142200221400},
        {libchess::squares::F8, 0x88500000000000},
    }};

    for (const auto &[sq, moves] : positions) {
        INFO(sq);
        REQUIRE(libchess::movegen::knight_moves(sq) == libchess::Bitboard(moves));
    }
}

TEST_CASE("Movegen bishops") {
    using tuple_type = std::tuple<libchess::Square, std::uint64_t, std::uint64_t>;

    const std::array<tuple_type, 3> positions = {{
        {libchess::squares::A1, 0x0, 0x8040201008040200},
        {libchess::squares::D4, 0x0, 0x8041221400142241},
        {libchess::squares::D4, 0x40000400040000, 0x40201400142040},
    }};

    for (const auto &[sq, blockers, moves] : positions) {
        INFO(sq);
        REQUIRE(libchess::movegen::bishop_moves(sq, libchess::Bitboard(blockers)) == libchess::Bitboard(moves));
    }
}

TEST_CASE("Movegen rooks") {
    using tuple_type = std::tuple<libchess::Square, std::uint64_t, std::uint64_t>;

    const std::array<tuple_type, 3> positions = {{
        {libchess::squares::A1, 0x0, 0x1010101010101fe},
        {libchess::squares::D4, 0x0, 0x8080808f7080808},
        {libchess::squares::D4, 0x840000800, 0x877080800},
    }};

    for (const auto &[sq, blockers, moves] : positions) {
        INFO(sq);
        REQUIRE(libchess::movegen::rook_moves(sq, libchess::Bitboard(blockers)) == libchess::Bitboard(moves));
    }
}

TEST_CASE("Movegen queens") {
    using pair_type = std::pair<libchess::Square, std::uint64_t>;

    const std::array<pair_type, 2> positions = {{
        {libchess::squares::A1, 0x0},
        {libchess::squares::D8, 0x8a00024008140000},
    }};

    for (const auto &[sq, blockers] : positions) {
        INFO(sq);
        const auto bishop = libchess::movegen::bishop_moves(sq, libchess::Bitboard(blockers));
        const auto rook = libchess::movegen::rook_moves(sq, libchess::Bitboard(blockers));
        const auto queen = libchess::movegen::queen_moves(sq, libchess::Bitboard(blockers));
        REQUIRE((bishop | rook) == queen);
    }
}

TEST_CASE("Movegen king") {
    using pair_type = std::pair<libchess::Square, std::uint64_t>;

    const std::array<pair_type, 3> positions = {{
        {libchess::squares::A1, 0x302},
        {libchess::squares::D4, 0x1c141c0000},
        {libchess::squares::D8, 0x141c000000000000},
    }};

    for (const auto &[sq, moves] : positions) {
        INFO(sq);
        REQUIRE(libchess::movegen::king_moves(sq) == libchess::Bitboard(moves));
    }
}
