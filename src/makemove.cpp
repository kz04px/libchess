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
    const auto type = move.type();
    const auto hash_old = hash_;
    const auto ep_old = ep_;
    const auto halfmove_clock_old = halfmove_clock_;

    assert(to != from);
    assert(piece != Piece::None);
    assert(captured != Piece::King);
    assert(promo != Piece::Pawn);
    assert(promo != Piece::King);
    assert(piece_on(move.from()) == piece);

    // Fullmoves
    fullmove_clock_ += us == Side::Black;

#ifndef NO_HASH
    hash_ ^= zobrist::turn_key();
    if (ep_ != squares::OffSq) {
        hash_ ^= zobrist::ep_key(ep_);
    }
#endif

    // Remove ep
    ep_ = squares::OffSq;

    // Increment halfmove clock
    halfmove_clock_++;

    switch (type) {
        case MoveType::Normal:
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(move.to());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
            assert(captured == Piece::None);
            assert(promo == Piece::None);

            if (piece == Piece::Pawn) {
                halfmove_clock_ = 0;
            }
            break;
        case MoveType::Capture:
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(move.to());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
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
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(move.to());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
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
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(move.to());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
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
                assert(piece_on(move.from()) == Piece::King);
                assert(piece_on(move.to()) == XFEN ? Piece::Rook : Piece::None);
                colours_[us] ^= Bitboard(move.from()) ^ Bitboard(castle_king_to[us * 2]);
                pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(castle_king_to[us * 2]);

#ifndef NO_HASH
                hash_ ^= zobrist::piece_key(Piece::King, us, move.from());
                hash_ ^= zobrist::piece_key(Piece::King, us, castle_king_to[us * 2]);
                hash_ ^= zobrist::piece_key(Piece::Rook, us, castle_rooks_from_[us * 2]);
                hash_ ^= zobrist::piece_key(Piece::Rook, us, castle_rook_to[us * 2]);
#endif

                // Remove the rook
                colours_[us] ^= castle_rooks_from_[us * 2];
                pieces_[Piece::Rook] ^= castle_rooks_from_[us * 2];
                // Add the rook
                colours_[us] ^= castle_rook_to[us * 2];
                pieces_[Piece::Rook] ^= castle_rook_to[us * 2];

                assert(piece == Piece::King);
                assert(captured == Piece::None);
                assert(promo == Piece::None);
                assert(can_castle(us, MoveType::ksc));
                 assert(move.to() == (XFEN ? castle_rooks_from_[us * 2] : castle_king_to[us * 2]));

                // No overlap between any pieces and the path of the king, exclude the castling rook
                assert(!(occupied() & squares_between(from, castle_king_to[us * 2]) & ~Bitboard(castle_rook_to[us * 2])));
                // No overlap between any pieces and the path of the rook, exclude the castled king
                assert(
                    !(occupied() & squares_between(castle_rooks_from_[us * 2], castle_rook_to[us * 2]) & ~occupancy(Piece::King)));

                // Check if rook is at destination
                assert(piece_on(castle_rook_to[us * 2]) == Piece::Rook);
                // Check that king is on its destination square
                assert(piece_on(castle_king_to[us * 2]) == Piece::King);

                // Start square of king is either empty, its own, or the rook's target square
                assert(piece_on(from) == Piece::None || from == castle_rook_to[us * 2] || from == castle_king_to[us * 2]);
                // Start square of rook is either empty, its own, or the king's target square
                assert(piece_on(castle_rooks_from_[us * 2]) == Piece::None ||
                       castle_rooks_from_[us * 2] == castle_rook_to[us * 2] ||
                       castle_rooks_from_[us * 2] == castle_king_to[us * 2]);

                // Check if all squares touched by king are not attacked
                assert(!(squares_attacked(them) &
                         (squares_between(from, castle_king_to[us * 2]) | from | pieces(us, Piece::King))));
            break;
        case MoveType::qsc:
                assert(piece_on(move.from()) == Piece::King);
                assert(piece_on(move.to()) == XFEN ? Piece::Rook : Piece::None);
                colours_[us] ^= Bitboard(move.from()) ^ Bitboard(castle_king_to[us * 2 + 1]);
                pieces_[piece] ^= Bitboard(move.from()) ^ Bitboard(castle_king_to[us * 2 + 1]);

#ifndef NO_HASH
                hash_ ^= zobrist::piece_key(Piece::King, us, move.from());
                hash_ ^= zobrist::piece_key(Piece::King, us, castle_king_to[us * 2 + 1]);
                hash_ ^= zobrist::piece_key(Piece::Rook, us, castle_rooks_from_[us * 2 + 1]);
                hash_ ^= zobrist::piece_key(Piece::Rook, us, castle_rook_to[us * 2 + 1]);
#endif

                // Remove the rook
                colours_[us] ^= castle_rooks_from_[us * 2 + 1];
                pieces_[Piece::Rook] ^= castle_rooks_from_[us * 2 + 1];
                // Add the rook
                colours_[us] ^= castle_rook_to[us * 2 + 1];
                pieces_[Piece::Rook] ^= castle_rook_to[us * 2 + 1];

                assert(piece == Piece::King);
                assert(captured == Piece::None);
                assert(promo == Piece::None);
                assert(can_castle(us, MoveType::qsc));
                assert(move.to() == (XFEN ? castle_rooks_from_[us * 2 + 1] : castle_king_to[us * 2 + 1]));

                // No overlap between any pieces and the path of the king, exclude the castling rook
                assert(!(occupied() & squares_between(from, castle_king_to[us * 2 + 1]) & ~Bitboard(castle_rook_to[us * 2 + 1])));
                // No overlap between any pieces and the path of the rook, exclude the castled king
                assert(!(occupied() & squares_between(castle_rooks_from_[us * 2 + 1], castle_rook_to[us * 2 + 1]) &
                         ~occupancy(Piece::King)));

                // Check if rook is at destination
                assert(piece_on(castle_rook_to[us * 2 + 1]) == Piece::Rook);
                // Check that king is on its destination square
                assert(piece_on(castle_king_to[us * 2 + 1]) == Piece::King);

                assert(castle_king_to[us * 2 + 1] == pieces(us, Piece::King).hsb());

                // Start square of rook is either empty, its own, or the king's target square
                assert(piece_on(castle_rooks_from_[us * 2 + 1]) == Piece::None ||
                    castle_rooks_from_[us * 2 + 1] == castle_rook_to[us * 2 + 1] ||
                    castle_rooks_from_[us * 2 + 1] == castle_king_to[us * 2 + 1]);

                // Start square of king is either empty, its own, or the rook's target square
                assert(piece_on(from) == Piece::None || from == castle_rook_to[us * 2 + 1] || from == castle_king_to[us * 2 + 1]);

                // Check if all squares touched by king are not attacked
                assert(!(squares_attacked(them) &
                         (squares_between(from, castle_king_to[us * 2 + 1]) | from | pieces(us, Piece::King))));
            break;
        case MoveType::promo:
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[Piece::Pawn] ^= Bitboard(move.from());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
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
            pieces_[promo] ^= move.to();
            break;
        case MoveType::promo_capture:
            colours_[us] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            pieces_[Piece::Pawn] ^= Bitboard(move.from());
#ifndef NO_HASH
            hash_ ^= zobrist::piece_key(piece, us, move.from());
            hash_ ^= zobrist::piece_key(piece, us, move.to());
#endif
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
    castling_[usKSC] &=
        !((piece == Piece::King && us == Side::White) || from == castle_rooks_from_[0] || to == castle_rooks_from_[0]);
    castling_[usQSC] &=
        !((piece == Piece::King && us == Side::White) || from == castle_rooks_from_[1] || to == castle_rooks_from_[1]);
    castling_[themKSC] &=
        !((piece == Piece::King && us == Side::Black) || from == castle_rooks_from_[2] || to == castle_rooks_from_[2]);
    castling_[themQSC] &=
        !((piece == Piece::King && us == Side::Black) || from == castle_rooks_from_[3] || to == castle_rooks_from_[3]);

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
