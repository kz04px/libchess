#ifndef LIBCHESS_SIDE_HPP
#define LIBCHESS_SIDE_HPP

namespace libchess {

enum Side : bool
{
    White = 0,
    Black
};

inline constexpr Side operator!(Side s) {
    return static_cast<Side>(!static_cast<bool>(s));
}

}  // namespace libchess

#endif
