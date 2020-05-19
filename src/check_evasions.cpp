#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::vector<Move> Position::check_evasions() const noexcept {
    std::vector<Move> moves;
    moves.reserve(8);

    const auto ksq = king_position(turn());
    const auto safe = king_allowed(turn());
    const auto mask = movegen::king_moves(ksq) & safe;

    // Captures
    for (const auto &to : occupancy(!turn()) & mask) {
        const auto cap = piece_on(to);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(MoveType::Capture, ksq, to, Piece::King, cap);
    }

    // Non-captures
    for (const auto &to : empty() & mask) {
        moves.emplace_back(MoveType::Normal, ksq, to, Piece::King);
    }

    assert(moves.size() <= 8);

    return moves;
}

}  // namespace libchess
