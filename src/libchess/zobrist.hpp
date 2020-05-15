#ifndef LIBCHESS_ZOBRIST_HPP
#define LIBCHESS_ZOBRIST_HPP

#include <cstdint>
#include "piece.hpp"
#include "square.hpp"

namespace libchess::zobrist {

namespace {

constexpr const std::uint64_t keys[6][64] = {};

}  // namespace

std::uint64_t key(const Piece p, const Square sq) {
    return keys[static_cast<int>(p)][static_cast<int>(sq)];
}

}  // namespace libchess::zobrist

#endif
