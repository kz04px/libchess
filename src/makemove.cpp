#include "libchess/position.hpp"

namespace libchess {

void Position::makemove(const Move &move) noexcept {
    const auto us = turn();
    const auto them = !turn();
    const auto to = move.to();
    const auto from = move.from();
    const auto piece = move.piece();
    const auto captured = move.captured();
    const auto promo = move.promotion();
    const auto ep_old = ep_;

    assert(to != from);
    assert(piece != Piece::None);
    assert(captured != Piece::King);
    assert(promo != Piece::Pawn);
    assert(promo != Piece::King);

    // Remove piece
    colours_[us] ^= move.from();
    pieces_[piece] ^= move.from();

    // Add piece
    colours_[us] ^= move.to();
    pieces_[piece] ^= move.to();

    // Remove ep
    ep_ = 0xFF;

    switch (move.type()) {
        case MoveType::Normal:
            assert(captured == Piece::None);
            assert(promo == Piece::None);
            break;
        case MoveType::Capture:
            assert(captured != Piece::None);
            assert(promo == Piece::None);

            // Remove the captured piece
            pieces_[captured] ^= move.to();
            colours_[them] ^= move.to();
            break;
        case MoveType::Double:
            assert(piece == Piece::Pawn);
            assert(captured == Piece::None);
            assert(promo == Piece::None);
            assert(to.file() == from.file());

            ep_ = to.file();
            break;
        case MoveType::enpassant:
            assert(piece == Piece::Pawn);
            assert(captured == Piece::Pawn);
            assert(promo == Piece::None);
            assert(to.file() == ep_old);

            // Remove the captured pawn
            if (turn() == Side::White) {
                pieces_[Piece::Pawn] ^= move.to().south();
                colours_[Side::Black] ^= move.to().south();
            } else {
                pieces_[Piece::Pawn] ^= move.to().north();
                colours_[Side::White] ^= move.to().north();
            }
            break;
        case MoveType::ksc:
            assert(piece == Piece::King);
            assert(captured == Piece::None);
            assert(promo == Piece::None);

            // Remove the rook
            colours_[us] ^= ksc_rook_fr[us];
            pieces_[Piece::Rook] ^= ksc_rook_fr[us];
            // Add the rook
            colours_[us] ^= ksc_rook_to[us];
            pieces_[Piece::Rook] ^= ksc_rook_to[us];
            break;
        case MoveType::qsc:
            assert(piece == Piece::King);
            assert(captured == Piece::None);
            assert(promo == Piece::None);

            // Remove the rook
            colours_[us] ^= qsc_rook_fr[us];
            pieces_[Piece::Rook] ^= qsc_rook_fr[us];
            // Add the rook
            colours_[us] ^= qsc_rook_to[us];
            pieces_[Piece::Rook] ^= qsc_rook_to[us];
            break;
        case MoveType::promo:
            assert(piece == Piece::Pawn);
            assert(captured == Piece::None);
            assert(promo != Piece::None);

            // Replace pawn with piece
            pieces_[Piece::Pawn] ^= move.to();
            pieces_[promo] ^= move.to();
            break;
        case MoveType::promo_capture:
            assert(piece == Piece::Pawn);
            assert(captured != Piece::None);
            assert(promo != Piece::None);

            // Replace pawn with piece
            pieces_[Piece::Pawn] ^= move.to();
            pieces_[promo] ^= move.to();
            // Remove the captured piece
            pieces_[captured] ^= move.to();
            colours_[them] ^= move.to();
            break;
        default:
            abort();
    }

    // Add to history
    history_.push_back(
        meh{move,
            ep_old,
            {castling_[0], castling_[1], castling_[2], castling_[3]}});

    // Castling permissions
    castling_[usKSC] &=
        !(to == squares::H1 || from == squares::E1 || from == squares::H1);
    castling_[usQSC] &=
        !(to == squares::A1 || from == squares::E1 || from == squares::A1);
    castling_[themKSC] &=
        !(to == squares::H8 || from == squares::E8 || from == squares::H8);
    castling_[themQSC] &=
        !(to == squares::A8 || from == squares::E8 || from == squares::A8);

    // Swap sides
    to_move_ = !to_move_;

    assert(valid());
}

}  // namespace libchess
