#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] Bitboard Position::squares_attacked(const Side s) const noexcept {
    Bitboard mask;

    // Pawns
    if (s == Side::White) {
        const auto pawns = pieces(s, Piece::Pawn);
        mask |= pawns.north().east();
        mask |= pawns.north().west();
    } else {
        const auto pawns = pieces(s, Piece::Pawn);
        mask |= pawns.south().east();
        mask |= pawns.south().west();
    }

    // Knights
    for (const auto &fr : pieces(s, Piece::Knight)) {
        mask |= movegen::knight_moves(fr);
    }

    // Bishops
    for (const auto &fr : pieces(s, Piece::Bishop)) {
        mask |= movegen::bishop_moves(fr, ~empty());
    }

    // Rooks
    for (const auto &fr : pieces(s, Piece::Rook)) {
        mask |= movegen::rook_moves(fr, ~empty());
    }

    // Queens
    for (const auto &fr : pieces(s, Piece::Queen)) {
        mask |= movegen::queen_moves(fr, ~empty());
    }

    // King
    mask |= movegen::king_moves(king_position(s));

    return mask;
}

}  // namespace libchess
