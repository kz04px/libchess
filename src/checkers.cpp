#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] Bitboard Position::checkers() const noexcept {
    return attackers(king_position(turn()), !turn());
}

}  // namespace libchess
