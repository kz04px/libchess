#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] bool Position::valid() const noexcept {
#ifdef NO_HASH
    if (hash_ != 0) {
        return false;
    }
#else
    if (hash_ != calculate_hash()) {
        return false;
    }
#endif

    if (ep() != squares::OffSq) {
        if (turn() == Side::White && ep().rank() != 5) {
            return false;
        }
        if (turn() == Side::Black && ep().rank() != 2) {
            return false;
        }
    }

    if (pieces(Side::White, Piece::King).count() != 1) {
        return false;
    }

    if (pieces(Side::Black, Piece::King).count() != 1) {
        return false;
    }

    if (colours_[0] & colours_[1]) {
        return false;
    }

    if (occupancy(Piece::Pawn) & (bitboards::Rank1 | bitboards::Rank8)) {
        return false;
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 6; ++j) {
            if (pieces_[i] & pieces_[j]) {
                return false;
            }
        }
    }

    if ((colours_[0] | colours_[1]) != (pieces_[0] | pieces_[1] | pieces_[2] | pieces_[3] | pieces_[4] | pieces_[5])) {
        return false;
    }

    // Better not be able to capture the king
    if (square_attacked(king_position(!turn()), turn())) {
        return false;
    }

    if (can_castle(Side::White, MoveType::ksc)) {
        if (!(Bitboard(king_position(Side::White)) & bitboards::Rank1)) {
            return false;
        }
        if (piece_on(castle_rooks_from_[0]) != Piece::Rook) {
            return false;
        }
    }

    if (can_castle(Side::White, MoveType::qsc)) {
        if (!(Bitboard(king_position(Side::White)) & bitboards::Rank1)) {
            return false;
        }
        if (piece_on(castle_rooks_from_[1]) != Piece::Rook) {
            return false;
        }
    }

    if (can_castle(Side::Black, MoveType::ksc)) {
        if (!(Bitboard(king_position(Side::Black)) & bitboards::Rank8)) {
            return false;
        }
        if (piece_on(castle_rooks_from_[2]) != Piece::Rook) {
            return false;
        }
    }

    if (can_castle(Side::Black, MoveType::qsc)) {
        if (!(Bitboard(king_position(Side::Black)) & bitboards::Rank8)) {
            return false;
        }
        if (piece_on(castle_rooks_from_[3]) != Piece::Rook) {
            return false;
        }
    }

    return true;
}

}  // namespace libchess
