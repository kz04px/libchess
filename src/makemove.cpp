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
    const auto hash_old = hash_;
    const auto ep_old = ep_;
    const auto halfmove_clock_old = halfmove_clock_;

    assert(to != from);
    assert(piece != Piece::None);
    assert(captured != Piece::King);
    assert(promo != Piece::Pawn);
    assert(promo != Piece::King);
    assert(piece_on(move.from()) == piece);

    // Remove piece
    colours_[us] ^= move.from();
    pieces_[piece] ^= move.from();

    // Add piece
    colours_[us] ^= move.to();
    pieces_[piece] ^= move.to();

    // Fullmoves
    fullmove_clock_ += us == Side::Black;

#ifndef NO_HASH
    hash_ ^= zobrist::turn_key();
    hash_ ^= zobrist::piece_key(piece, us, move.from());
    hash_ ^= zobrist::piece_key(piece, us, move.to());
    if (ep_ != squares::OffSq) {
        hash_ ^= zobrist::ep_key(ep_);
    }
#endif

    // Remove ep
    ep_ = squares::OffSq;

    // Increment halfmove clock
    halfmove_clock_++;

    switch (move.type()) {
        case MoveType::Normal:
            assert(captured == Piece::None);
            assert(promo == Piece::None);

            if (piece == Piece::Pawn) {
                halfmove_clock_ = 0;
            }
            break;
        case MoveType::Capture:
            assert(captured != Piece::None);
            assert(promo == Piece::None);

            halfmove_clock_ = 0;

#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(captured, them, move.to());
#endif

            // Remove the captured piece
            pieces_[captured] ^= move.to();
            colours_[them] ^= move.to();
            break;
        case MoveType::Double:
            assert(piece == Piece::Pawn);
            assert(captured == Piece::None);
            assert(promo == Piece::None);
            assert(to.file() == from.file());
            assert((us == Side::White && move.to().rank() == 3) || (us == Side::Black && move.to().rank() == 4));
            assert((us == Side::White && move.from().rank() == 1) || (us == Side::Black && move.from().rank() == 6));

            halfmove_clock_ = 0;
            ep_ = us == Side::White ? to.south() : to.north();

#ifndef NO_HASH
            hash_ ^= zobrist::ep_key(ep_);
#endif
            break;
        case MoveType::enpassant:
            assert(piece == Piece::Pawn);
            assert(captured == Piece::Pawn);
            assert(promo == Piece::None);
            assert(to.file() == ep_old.file());
            assert((us == Side::White && move.to().rank() == 5) || (us == Side::Black && move.to().rank() == 2));
            assert((us == Side::White && move.from().rank() == 4) || (us == Side::Black && move.from().rank() == 3));
            assert(to.file() - from.file() == 1 || from.file() - to.file() == 1);

            halfmove_clock_ = 0;

            // Remove the captured pawn
            if (turn() == Side::White) {
                pieces_[Piece::Pawn] ^= move.to().south();
                colours_[Side::Black] ^= move.to().south();
#ifndef NO_HASH
                hash_ ^= zobrist::piece_key(Piece::Pawn, them, move.to().south());
#endif
            } else {
                pieces_[Piece::Pawn] ^= move.to().north();
                colours_[Side::White] ^= move.to().north();
#ifndef NO_HASH
                hash_ ^= zobrist::piece_key(Piece::Pawn, them, move.to().north());
#endif
            }
            break;
        case MoveType::ksc:
            assert(piece == Piece::King);
            assert(captured == Piece::None);
            assert(promo == Piece::None);
            assert(can_castle(us, MoveType::ksc));
            assert(us == Side::White ? move.from() == squares::E1 : move.from() == squares::E8);
            assert(us == Side::White ? move.to() == squares::G1 : move.to() == squares::G8);
            assert(us == Side::White ? piece_on(squares::E1) == Piece::None : piece_on(squares::E8) == Piece::None);
            assert(us == Side::White ? piece_on(squares::F1) == Piece::None : piece_on(squares::F8) == Piece::None);
            assert(us == Side::White ? piece_on(squares::G1) == Piece::King : piece_on(squares::G8) == Piece::King);
            assert(us == Side::White ? piece_on(squares::H1) == Piece::Rook : piece_on(squares::H8) == Piece::Rook);
            assert(us == Side::White ? !square_attacked(squares::E1, them) : !square_attacked(squares::E8, them));
            assert(us == Side::White ? !square_attacked(squares::F1, them) : !square_attacked(squares::F8, them));
            assert(us == Side::White ? !square_attacked(squares::G1, them) : !square_attacked(squares::G8, them));

#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_fr[us]);
            hash_ ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_to[us]);
#endif

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
            assert(can_castle(us, MoveType::qsc));
            assert(us == Side::White ? move.from() == squares::E1 : move.from() == squares::E8);
            assert(us == Side::White ? move.to() == squares::C1 : move.to() == squares::C8);
            assert(us == Side::White ? piece_on(squares::E1) == Piece::None : piece_on(squares::E8) == Piece::None);
            assert(us == Side::White ? piece_on(squares::D1) == Piece::None : piece_on(squares::D8) == Piece::None);
            assert(us == Side::White ? piece_on(squares::C1) == Piece::King : piece_on(squares::C8) == Piece::King);
            assert(us == Side::White ? piece_on(squares::B1) == Piece::None : piece_on(squares::B8) == Piece::None);
            assert(us == Side::White ? piece_on(squares::A1) == Piece::Rook : piece_on(squares::A8) == Piece::Rook);
            assert(us == Side::White ? !square_attacked(squares::E1, them) : !square_attacked(squares::E8, them));
            assert(us == Side::White ? !square_attacked(squares::D1, them) : !square_attacked(squares::D8, them));
            assert(us == Side::White ? !square_attacked(squares::C1, them) : !square_attacked(squares::C8, them));

#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_fr[us]);
            hash_ ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_to[us]);
#endif

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
            assert(move.to().file() == move.from().file());
            assert((us == Side::White && move.to().rank() == 7) || (us == Side::Black && move.to().rank() == 0));
            assert((us == Side::White && move.from().rank() == 6) || (us == Side::Black && move.from().rank() == 1));

            halfmove_clock_ = 0;

#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(Piece::Pawn, us, move.to());
            hash_ ^= zobrist::piece_key(promo, us, move.to());
#endif

            // Replace pawn with piece
            pieces_[Piece::Pawn] ^= move.to();
            pieces_[promo] ^= move.to();
            break;
        case MoveType::promo_capture:
            assert(piece == Piece::Pawn);
            assert(captured != Piece::None);
            assert(promo != Piece::None);
            assert(move.to().file() != move.from().file());
            assert((us == Side::White && move.to().rank() == 7) || (us == Side::Black && move.to().rank() == 0));
            assert((us == Side::White && move.from().rank() == 6) || (us == Side::Black && move.from().rank() == 1));

            halfmove_clock_ = 0;

#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(captured, them, move.to());
            hash_ ^= zobrist::piece_key(Piece::Pawn, us, move.to());
            hash_ ^= zobrist::piece_key(promo, us, move.to());
#endif

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

#ifndef NO_HASH
    const bool castling_old[] = {
        castling_[0],
        castling_[1],
        castling_[2],
        castling_[3],
    };
#endif

    // Add to history
    history_.push_back(
        meh{hash_old, move, ep_old, halfmove_clock_old, {castling_[0], castling_[1], castling_[2], castling_[3]}});

    // Castling permissions
    castling_[usKSC] &= !(to == squares::H1 || from == squares::E1 || from == squares::H1);
    castling_[usQSC] &= !(to == squares::A1 || from == squares::E1 || from == squares::A1);
    castling_[themKSC] &= !(to == squares::H8 || from == squares::E8 || from == squares::H8);
    castling_[themQSC] &= !(to == squares::A8 || from == squares::E8 || from == squares::A8);

#ifndef NO_HASH
    if (castling_[usKSC] != castling_old[usKSC]) {
        hash_ ^= zobrist::castling_key(usKSC);
    }
    if (castling_[usQSC] != castling_old[usQSC]) {
        hash_ ^= zobrist::castling_key(usQSC);
    }
    if (castling_[themKSC] != castling_old[themKSC]) {
        hash_ ^= zobrist::castling_key(themKSC);
    }
    if (castling_[themQSC] != castling_old[themQSC]) {
        hash_ ^= zobrist::castling_key(themQSC);
    }
#endif

    // Swap sides
    to_move_ = !to_move_;

    assert(valid());
}

}  // namespace libchess
