#include "libchess/position.hpp"

namespace libchess {

void Position::undomove() noexcept {
    // Swap sides
    to_move_ = !to_move_;

    const auto &move = history_.back().move;
    const auto us = turn();
    const auto them = !turn();
    const auto piece = move.piece();
    const auto captured = move.captured();
    const auto promo = move.promotion();

    // En passant
    ep_ = history_.back().ep;

    // Halfmoves
    halfmove_clock_ = history_.back().halfmove_clock;

    // Fullmoves
    fullmove_clock_ -= us == Side::Black;

    // Castling
    castling_[0] = history_.back().castling[0];
    castling_[1] = history_.back().castling[1];
    castling_[2] = history_.back().castling[2];
    castling_[3] = history_.back().castling[3];

#ifndef NO_HASH
    hash_ = history_.back().hash;
#endif

    // Remove piece
    colours_[us] ^= move.to();
    pieces_[piece] ^= move.to();

    // Add piece
    colours_[us] ^= move.from();
    pieces_[piece] ^= move.from();

    switch (move.type()) {
        case MoveType::Normal:
            break;
        case MoveType::Double:
            break;
        case MoveType::Capture:
            colours_[them] ^= move.to();
            pieces_[captured] ^= move.to();
            break;
        case MoveType::enpassant:
            // Replace the captured pawn
            if (turn() == Side::White) {
                pieces_[Piece::Pawn] ^= move.to().south();
                colours_[Side::Black] ^= move.to().south();
            } else {
                pieces_[Piece::Pawn] ^= move.to().north();
                colours_[Side::White] ^= move.to().north();
            }
            break;
        case MoveType::ksc:
            // Remove the rook
            colours_[us] ^= ksc_rook_fr[us];
            pieces_[Piece::Rook] ^= ksc_rook_fr[us];
            // Add the rook
            colours_[us] ^= ksc_rook_to[us];
            pieces_[Piece::Rook] ^= ksc_rook_to[us];
            break;
        case MoveType::qsc:
            // Remove the rook
            colours_[us] ^= qsc_rook_fr[us];
            pieces_[Piece::Rook] ^= qsc_rook_fr[us];
            // Add the rook
            colours_[us] ^= qsc_rook_to[us];
            pieces_[Piece::Rook] ^= qsc_rook_to[us];
            break;
        case MoveType::promo:
            // Replace piece with pawn
            pieces_[Piece::Pawn] ^= move.to();
            pieces_[promo] ^= move.to();
            break;
        case MoveType::promo_capture:
            // Replace pawn with piece
            pieces_[Piece::Pawn] ^= move.to();
            pieces_[promo] ^= move.to();
            // Replace the captured piece
            pieces_[captured] ^= move.to();
            colours_[them] ^= move.to();
            break;
        default:
            break;
    }

    // Remove from history
    history_.pop_back();

    assert(valid());
}

}  // namespace libchess
