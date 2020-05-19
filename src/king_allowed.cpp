#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] Bitboard Position::king_allowed() const noexcept {
    return king_allowed(turn());
}

[[nodiscard]] Bitboard Position::king_allowed(const Side s) const noexcept {
    const Bitboard blockers = ~empty() ^ king_position(s);
    Bitboard mask;

    // Pawns
    if (s == Side::White) {
        const auto pawns = pieces(!s, Piece::Pawn);
        mask |= pawns.south().east();
        mask |= pawns.south().west();
    } else {
        const auto pawns = pieces(!s, Piece::Pawn);
        mask |= pawns.north().east();
        mask |= pawns.north().west();
    }

    // Knights
    for (const auto &fr : pieces(!s, Piece::Knight)) {
        mask |= movegen::knight_moves(fr);
    }

    // Bishops
    for (const auto &fr : pieces(!s, Piece::Bishop)) {
        mask |= movegen::bishop_moves(fr, blockers);
    }

    // Rooks
    for (const auto &fr : pieces(!s, Piece::Rook)) {
        mask |= movegen::rook_moves(fr, blockers);
    }

    // Queens
    for (const auto &fr : pieces(!s, Piece::Queen)) {
        mask |= movegen::queen_moves(fr, blockers);
    }

    // King
    mask |= movegen::king_moves(king_position(!s));

    // Let's remove friendly pieces
    mask |= occupancy(s);

    // Let's remove enemy king square
    mask |= king_position(!s);

    return ~mask;
}

}  // namespace libchess
