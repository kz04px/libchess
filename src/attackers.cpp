#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] Bitboard Position::attackers(const Square sq, const Side s) const noexcept {
    Bitboard mask;
    const auto bb = Bitboard{sq};

    if (s == Side::White) {
        const auto pawns = pieces(s, Piece::Pawn);
        mask |= pawns & bb.south().east();
        mask |= pawns & bb.south().west();
    } else {
        const auto pawns = pieces(s, Piece::Pawn);
        mask |= pawns & bb.north().east();
        mask |= pawns & bb.north().west();
    }

    mask |= movegen::knight_moves(sq) & pieces(s, Piece::Knight);

    mask |= movegen::bishop_moves(sq, ~empty()) & (pieces(s, Piece::Bishop) | pieces(s, Piece::Queen));

    mask |= movegen::rook_moves(sq, ~empty()) & (pieces(s, Piece::Rook) | pieces(s, Piece::Queen));

    mask |= movegen::king_moves(sq) & pieces(s, Piece::King);

    return mask;
}

}  // namespace libchess
