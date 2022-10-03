#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::vector<Move> Position::legal_moves() const noexcept {
    std::vector<Move> moves;
    moves.reserve(200);
    legal_captures(moves);
    legal_noncaptures(moves);
    return moves;
}

void Position::legal_moves(std::vector<Move> &moves) const noexcept {
    legal_captures(moves);
    legal_noncaptures(moves);
}

}  // namespace libchess
