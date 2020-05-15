#ifndef LIBCHESS_SQUARE_HPP
#define LIBCHESS_SQUARE_HPP

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string>

namespace libchess {

class Square {
   public:
    constexpr Square() : data_{0xff} {
    }

    constexpr Square(const int n) : data_{static_cast<std::uint8_t>(n)} {
        assert(n < 64);
    }

    constexpr Square(const std::string &str) : data_{} {
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
    os << static_cast<char>('a' + sq.file())
       << static_cast<char>('1' + sq.rank());
    return os;
}

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
