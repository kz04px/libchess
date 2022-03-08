#ifndef LIBCHESS_SQUARE_HPP
#define LIBCHESS_SQUARE_HPP

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string>

namespace libchess {

class Square {
   public:
    [[nodiscard]] constexpr Square() : data_{0xff} {
    }

    [[nodiscard]] constexpr Square(const int n) : data_{static_cast<std::uint8_t>(n)} {
        assert(n < 64);
    }

    [[nodiscard]] constexpr Square(const int f, const int r) : data_{static_cast<std::uint8_t>(8 * r + f)} {
        assert(data_);
    }

    [[nodiscard]] Square(const std::string &str) : data_{} {
        const int file = str[0] - 'a';
        const int rank = str[1] - '1';
        data_ = static_cast<std::uint8_t>(rank * 8 + file);
    }

    [[nodiscard]] constexpr bool operator==(const Square &rhs) const noexcept {
        return data_ == rhs.data_;
    }

    [[nodiscard]] constexpr bool operator!=(const Square &rhs) const noexcept {
        return data_ != rhs.data_;
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return data_ != 0xFF;
    }

    [[nodiscard]] constexpr explicit operator int() const noexcept {
        return data_;
    }

    [[nodiscard]] explicit operator std::string() const noexcept {
        std::string str;
        str += static_cast<char>('a' + file());
        str += static_cast<char>('1' + rank());
        return str;
    }

    [[nodiscard]] constexpr explicit operator unsigned int() const noexcept {
        return data_;
    }

    [[nodiscard]] constexpr int rank() const noexcept {
        return data_ / 8;
    }

    [[nodiscard]] constexpr int file() const noexcept {
        return data_ % 8;
    }

    [[nodiscard]] constexpr bool light() const noexcept {
        return (rank() + file()) % 2;
    }

    [[nodiscard]] constexpr bool dark() const noexcept {
        return !light();
    }

    [[nodiscard]] constexpr Square flip() const noexcept {
        return Square(data_ ^ 56);
    }

    [[nodiscard]] constexpr Square north() const noexcept {
        return Square(data_ + 8);
    }

    [[nodiscard]] constexpr Square south() const noexcept {
        return Square(data_ - 8);
    }

    [[nodiscard]] constexpr Square east() const noexcept {
        return Square(data_ + 1);
    }

    [[nodiscard]] constexpr Square west() const noexcept {
        return Square(data_ - 1);
    }

   private:
    std::uint8_t data_;
};

inline std::ostream &operator<<(std::ostream &os, const Square &sq) noexcept {
    os << static_cast<std::string>(sq);
    return os;
}

// clang-format off
const std::string square_strings[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};
// clang-format on

namespace squares {

constexpr Square A1 = {0};
constexpr Square B1 = {1};
constexpr Square C1 = {2};
constexpr Square D1 = {3};
constexpr Square E1 = {4};
constexpr Square F1 = {5};
constexpr Square G1 = {6};
constexpr Square H1 = {7};

constexpr Square A2 = {8};
constexpr Square B2 = {9};
constexpr Square C2 = {10};
constexpr Square D2 = {11};

constexpr Square A3 = {16};
constexpr Square B3 = {17};
constexpr Square C3 = {18};
constexpr Square D3 = {19};
constexpr Square E3 = {20};
constexpr Square F3 = {21};
constexpr Square G3 = {22};
constexpr Square H3 = {23};

constexpr Square A4 = {24};
constexpr Square D4 = {27};

constexpr Square A5 = {32};
constexpr Square B5 = {33};
constexpr Square C5 = {34};
constexpr Square D5 = {35};
constexpr Square E5 = {36};
constexpr Square F5 = {37};
constexpr Square G5 = {38};

constexpr Square A6 = {40};
constexpr Square H6 = {47};

constexpr Square A7 = {48};
constexpr Square B7 = {49};
constexpr Square F7 = {53};
constexpr Square G7 = {54};

constexpr Square A8 = {56};
constexpr Square B8 = {57};
constexpr Square C8 = {58};
constexpr Square D8 = {59};
constexpr Square E8 = {60};
constexpr Square F8 = {61};
constexpr Square G8 = {62};
constexpr Square H8 = {63};

static_assert(A1.dark());
static_assert(!A1.light());
static_assert(!H1.dark());
static_assert(H1.light());
static_assert(A1.flip() == A8);
static_assert(A8.flip() == A1);
static_assert(H1.flip() == H8);
static_assert(H8.flip() == H1);
// static_assert(Square("a1") == A1);

}  // namespace squares

}  // namespace libchess

#endif
