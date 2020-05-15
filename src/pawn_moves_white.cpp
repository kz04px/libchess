#include <iostream>
#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

void Position::pawn_moves_white(
    std::vector<Move> &moves,
    const Bitboard &pawns,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    // Singles
    const auto singles = empty() & pawns.north() & allowed;
    for (const auto &sq : singles) {
        moves.emplace_back(MoveType::Normal, sq.south(), sq, Piece::Pawn);
    }

    // Doubles
    const auto doubles = empty() & (empty() & pawns.north()).north() &
                         bitboards::Rank4 & allowed;
    for (const auto &sq : doubles) {
        moves.emplace_back(
            MoveType::Double, sq.south().south(), sq, Piece::Pawn);
    }

    // Captures -- Right
    const auto cap_right = pawns.north().east() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_right) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(
            MoveType::Capture, sq.south().west(), sq, Piece::Pawn, cap);
    }

    // Captures -- left
    const auto cap_left = pawns.north().west() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_left) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(
            MoveType::Capture, sq.south().east(), sq, Piece::Pawn, cap);
    }

    // En passant
    if (ep_ != 0xFF) {
        const auto file = bitboards::files[ep_];
        const auto bb = file & bitboards::Rank6;
        const auto ksq = king_position(turn());
        const auto ep_sq = bb.lsbll();

        if (pawns.north().west() & bb & allowed) {
            const auto blockers = occupied() ^ bb.south() ^ bb.south().east();
            const auto mask =
                movegen::rook_moves(ksq, blockers) & bitboards::Rank5;
            const auto asd =
                pieces(!turn(), Piece::Rook) | pieces(!turn(), Piece::Queen);

            // We just enabled a discovered check
            if (mask & asd) {
            } else {
                moves.emplace_back(MoveType::enpassant,
                                   ep_sq.south().east(),
                                   ep_sq,
                                   Piece::Pawn,
                                   Piece::Pawn);
            }
        }
        if (pawns.north().east() & bb & allowed) {
            const auto blockers = occupied() ^ bb.south() ^ bb.south().west();
            const auto mask =
                movegen::rook_moves(ksq, blockers) & bitboards::Rank5;
            const auto asd =
                pieces(!turn(), Piece::Rook) | pieces(!turn(), Piece::Queen);

            // We just enabled a discovered check
            if (mask & asd) {
            } else {
                moves.emplace_back(MoveType::enpassant,
                                   ep_sq.south().west(),
                                   ep_sq,
                                   Piece::Pawn,
                                   Piece::Pawn);
            }
        }
    }
}

}  // namespace libchess
