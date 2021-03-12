#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] bool Position::square_attacked(const Square sq, const Side s) const noexcept {
    return !attackers(sq, s).empty();
}

}  // namespace libchess
