#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::uint64_t Position::predict_hash(const Move &move) const noexcept {
#ifdef NO_HASH
    return 0;
#else
    auto new_hash = hash();

    const auto us = turn();
    const auto them = !turn();
    const auto to = move.to();
    const auto from = move.from();
    const auto piece = move.piece();
    const auto captured = move.captured();
    const auto promo = move.promotion();

    new_hash ^= zobrist::turn_key();
    if (ep_ != squares::OffSq) {
        new_hash ^= zobrist::ep_key(ep_);
    }

    switch (move.type()) {
        case MoveType::Normal:
            new_hash ^= zobrist::piece_key(piece, us, move.from());
            new_hash ^= zobrist::piece_key(piece, us, move.to());
            break;
        case MoveType::Capture:
            new_hash ^= zobrist::piece_key(piece, us, move.from());
            new_hash ^= zobrist::piece_key(piece, us, move.to());
            new_hash ^= zobrist::piece_key(captured, them, move.to());
            break;
        case MoveType::Double:
            new_hash ^= zobrist::piece_key(piece, us, move.from());
            new_hash ^= zobrist::piece_key(piece, us, move.to());
            new_hash ^= zobrist::ep_key(to);
            break;
        case MoveType::enpassant:
            new_hash ^= zobrist::piece_key(piece, us, move.from());
            new_hash ^= zobrist::piece_key(piece, us, move.to());
            if (turn() == Side::White) {
                new_hash ^= zobrist::piece_key(Piece::Pawn, them, move.to().south());
            } else {
                new_hash ^= zobrist::piece_key(Piece::Pawn, them, move.to().north());
            }
            break;
        case MoveType::ksc:
            new_hash ^= zobrist::piece_key(Piece::King, us, move.from());
            new_hash ^= zobrist::piece_key(Piece::King, us, move.to());
            new_hash ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_fr[us]);
            new_hash ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_to[us]);
            break;
        case MoveType::qsc:
            new_hash ^= zobrist::piece_key(Piece::King, us, move.from());
            new_hash ^= zobrist::piece_key(Piece::King, us, move.to());
            new_hash ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_fr[us]);
            new_hash ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_to[us]);
            break;
        case MoveType::promo:
            new_hash ^= zobrist::piece_key(Piece::Pawn, us, move.from());
            new_hash ^= zobrist::piece_key(promo, us, move.to());
            break;
        case MoveType::promo_capture:
            new_hash ^= zobrist::piece_key(Piece::Pawn, us, move.from());
            new_hash ^= zobrist::piece_key(promo, us, move.to());
            new_hash ^= zobrist::piece_key(captured, them, move.to());
            break;
        default:
            abort();
    }

    if (can_castle(Side::White, MoveType::ksc) && (to == squares::H1 || from == squares::E1 || from == squares::H1)) {
        new_hash ^= zobrist::castling_key(usKSC);
    }

    if (can_castle(Side::White, MoveType::qsc) && (to == squares::A1 || from == squares::E1 || from == squares::A1)) {
        new_hash ^= zobrist::castling_key(usQSC);
    }

    if (can_castle(Side::Black, MoveType::ksc) && (to == squares::H8 || from == squares::E8 || from == squares::H8)) {
        new_hash ^= zobrist::castling_key(themKSC);
    }

    if (can_castle(Side::Black, MoveType::qsc) && (to == squares::A8 || from == squares::E8 || from == squares::A8)) {
        new_hash ^= zobrist::castling_key(themQSC);
    }

    return new_hash;
#endif
}

}  // namespace libchess
