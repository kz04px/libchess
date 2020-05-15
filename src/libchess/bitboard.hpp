#ifndef LIBCHESS_BITBOARD_HPP
#define LIBCHESS_BITBOARD_HPP

#include <cassert>
#include <cstdint>
#include <ostream>
#include "square.hpp"

namespace libchess {

class BitboardIterator {
   public:
    constexpr BitboardIterator(const std::uint64_t &data) : data_{data} {
    }

    [[nodiscard]] constexpr Square operator*() const noexcept {
        return static_cast<Square>(__builtin_ctzll(data_));
    }

    constexpr BitboardIterator operator++() noexcept {
        data_ &= data_ - 1;
        return *this;
    }

    [[nodiscard]] constexpr bool operator!=(const BitboardIterator &rhs) const
        noexcept {
        return data_ != rhs.data_;
    }

   private:
    std::uint64_t data_;
};

class Bitboard {
   public:
    constexpr Bitboard() : mask_{0} {
    }

    constexpr Bitboard(const std::uint64_t mask) : mask_{mask} {
    }

    constexpr Bitboard(const Square sq) : mask_{1ULL << static_cast<int>(sq)} {
    }

    [[nodiscard]] constexpr bool get(const Square sq) const noexcept {
        return (mask_ >> static_cast<int>(sq)) & 1;
    }

    constexpr void set(const Square sq) noexcept {
        mask_ |= 1ULL << static_cast<int>(sq);
    }

    [[nodiscard]] constexpr int count() const noexcept {
        return __builtin_popcountll(mask_);
    }

    [[nodiscard]] constexpr Bitboard adjacent() const noexcept {
        return north() | south();
    }

    constexpr void clear() noexcept {
        mask_ = 0;
    }

    [[nodiscard]] constexpr BitboardIterator begin() const noexcept {
        return BitboardIterator{mask_};
    }

    [[nodiscard]] constexpr BitboardIterator end() const noexcept {
        return BitboardIterator{0};
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return mask_;
    }

    [[nodiscard]] constexpr bool operator==(const Bitboard &rhs) const
        noexcept {
        return mask_ == rhs.mask_;
    }

    [[nodiscard]] constexpr bool operator!=(const Bitboard &rhs) const
        noexcept {
        return mask_ != rhs.mask_;
    }

    [[nodiscard]] constexpr Bitboard operator&(const Bitboard &rhs) const
        noexcept {
        return mask_ & rhs.mask_;
    }

    [[nodiscard]] constexpr Bitboard operator|(const Bitboard &rhs) const
        noexcept {
        return mask_ | rhs.mask_;
    }

    [[nodiscard]] constexpr Bitboard operator^(const Bitboard &rhs) const
        noexcept {
        return mask_ ^ rhs.mask_;
    }

    [[nodiscard]] constexpr Bitboard operator~() const noexcept {
        return ~mask_;
    }

    Bitboard constexpr operator&=(const Bitboard &rhs) noexcept {
        mask_ &= rhs.mask_;
        return *this;
    }

    Bitboard constexpr operator|=(const Bitboard &rhs) noexcept {
        mask_ |= rhs.mask_;
        return *this;
    }

    Bitboard constexpr operator^=(const Bitboard &rhs) noexcept {
        mask_ ^= rhs.mask_;
        return *this;
    }

    [[nodiscard]] constexpr Bitboard operator<<(const int n) const noexcept {
        return mask_ << n;
    }

    [[nodiscard]] constexpr Bitboard operator>>(const int n) const noexcept {
        return mask_ >> n;
    }

    [[nodiscard]] constexpr Square lsbll() const noexcept {
        assert(mask_);
        return __builtin_ctzll(mask_);
    }

    [[nodiscard]] constexpr std::uint64_t value() const noexcept {
        return mask_;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return mask_ == 0;
    }

    [[nodiscard]] constexpr Bitboard north() const noexcept {
        return mask_ << 8;
    }

    [[nodiscard]] constexpr Bitboard south() const noexcept {
        return mask_ >> 8;
    }

    [[nodiscard]] constexpr Bitboard east() const noexcept {
        return (mask_ << 1) & ~0x0101010101010101ULL;
    }

    [[nodiscard]] constexpr Bitboard west() const noexcept {
        return (mask_ >> 1) & ~0x8080808080808080ULL;
    }

    /*
    [[nodiscard]] constexpr Bitboard east() const noexcept {
        return (mask_ << 1) & (~constants::FileA);
    }

    [[nodiscard]] constexpr Bitboard west() const noexcept {
        return (mask_ >> 1) & (~constants::FileH);
    }
    */

   private:
    std::uint64_t mask_;
};

inline std::ostream &operator<<(std::ostream &os, const Bitboard &bb) noexcept {
    int i = 56;
    while (i >= 0) {
        const auto sq = Square(i);
        if (bb & Bitboard{sq}) {
            os << '1';
        } else {
            os << '0';
        }

        if (i % 8 == 7) {
            os << '\n';
            i -= 16;
        }

        i++;
    }

    return os;
}

[[nodiscard]] constexpr Bitboard squares_between(Square sq1, const Square sq2) {
    const auto dx = (sq2.file() - sq1.file());
    const auto dy = (sq2.rank() - sq1.rank());
    const auto adx = dx > 0 ? dx : -dx;
    const auto ady = dy > 0 ? dy : -dy;

    if (dx == 0 || dy == 0 || adx == ady) {
        Bitboard mask;
        while (sq1 != sq2) {
            if (dx > 0) {
                sq1 = sq1.east();
            } else if (dx < 0) {
                sq1 = sq1.west();
            }
            if (dy > 0) {
                sq1 = sq1.north();
            } else if (dy < 0) {
                sq1 = sq1.south();
            }
            mask |= Bitboard{sq1};
        }
        return mask & ~Bitboard{sq2};
    }

    return {};
}

namespace bitboards {

constexpr Bitboard FileA = {0x0101010101010101};
constexpr Bitboard FileB = {0x0202020202020202};
constexpr Bitboard FileC = {0x0404040404040404};
constexpr Bitboard FileD = {0x0808080808080808};
constexpr Bitboard FileE = {0x1010101010101010};
constexpr Bitboard FileF = {0x2020202020202020};
constexpr Bitboard FileG = {0x4040404040404040};
constexpr Bitboard FileH = {0x8080808080808080};

constexpr Bitboard Rank1 = {0x00000000000000ff};
constexpr Bitboard Rank2 = {0x000000000000ff00};
constexpr Bitboard Rank3 = {0x0000000000ff0000};
constexpr Bitboard Rank4 = {0x00000000ff000000};
constexpr Bitboard Rank5 = {0x000000ff00000000};
constexpr Bitboard Rank6 = {0x0000ff0000000000};
constexpr Bitboard Rank7 = {0x00ff000000000000};
constexpr Bitboard Rank8 = {0xff00000000000000};

constexpr Bitboard files[] =
    {FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH};
constexpr Bitboard ranks[] =
    {Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8};
constexpr Bitboard adjacent_files[] = {FileB,
                                       FileA | FileC,
                                       FileB | FileD,
                                       FileC | FileE,
                                       FileD | FileF,
                                       FileE | FileG,
                                       FileF | FileH,
                                       FileG};

constexpr Bitboard LightSquares = {0x55aa55aa55aa55aa};
constexpr Bitboard DarkSquares = {0xaa55aa55aa55aa55};
constexpr Bitboard Empty = {0x0000000000000000};
constexpr Bitboard AllSquares = {0xffffffffffffffff};

static_assert(Empty.count() == 0);
static_assert(AllSquares.count() == 64);
static_assert(LightSquares.count() == 32);
static_assert(DarkSquares.count() == 32);
static_assert((~Empty) == AllSquares);
static_assert(~AllSquares == Empty);
static_assert((LightSquares & DarkSquares) == Empty);
static_assert((LightSquares | DarkSquares) == AllSquares);

}  // namespace bitboards

static_assert(sizeof(Bitboard) == sizeof(std::uint64_t));
// Same file
static_assert(squares_between(squares::A1, squares::A4) == 0x10100);
static_assert(squares_between(squares::A4, squares::A1) == 0x10100);
// Same Rank
static_assert(squares_between(squares::A1, squares::D1) == 0x6);
static_assert(squares_between(squares::D1, squares::A1) == 0x6);
// Same diagonal a1-h8
static_assert(squares_between(squares::A1, squares::D4) == 0x40200);
static_assert(squares_between(squares::D4, squares::A1) == 0x40200);
// Same diagonal a8-h1
static_assert(squares_between(squares::A4, squares::D1) == 0x20400);
static_assert(squares_between(squares::D1, squares::A4) == 0x20400);
// Hmm
static_assert(squares_between(squares::A1, squares::A1) == 0x0);
static_assert(squares_between(squares::A1, squares::B1) == 0x0);
static_assert(squares_between(squares::A1, squares::B2) == 0x0);
static_assert(squares_between(squares::A1, squares::A2) == 0x0);
static_assert(squares_between(squares::A1, squares::B3) == 0x0);
static_assert(squares_between(squares::A1, squares::C2) == 0x0);
static_assert(squares_between(squares::B3, squares::A1) == 0x0);
static_assert(squares_between(squares::B3, squares::A1) == 0x0);

}  // namespace libchess

#endif
