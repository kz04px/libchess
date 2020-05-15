#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

void Position::pawn_moves_black(
    std::vector<Move> &moves,
    const Bitboard &pawns,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    // Singles
    const auto singles = empty() & pawns.south() & allowed;
    for (const auto &sq : singles) {
        moves.emplace_back(MoveType::Normal, sq.north(), sq, Piece::Pawn);
    }

    // Doubles
    const auto doubles = empty() & (empty() & pawns.south()).south() &
                         bitboards::Rank5 & allowed;
    for (const auto &sq : doubles) {
        moves.emplace_back(
            MoveType::Double, sq.north().north(), sq, Piece::Pawn);
    }

    // Captures -- Right
    const auto cap_right = pawns.south().east() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_right) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(
            MoveType::Capture, sq.north().west(), sq, Piece::Pawn, cap);
    }

    // Captures -- left
    const auto cap_left = pawns.south().west() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_left) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(
            MoveType::Capture, sq.north().east(), sq, Piece::Pawn, cap);
    }

    // En passant
    if (ep_ != 0xFF) {
        const auto file = bitboards::files[ep_];
        const auto bb = file & bitboards::Rank3;
        const auto ksq = king_position(turn());
        const auto ep_sq = bb.lsbll();

        if (pawns.south().west() & bb & allowed) {
            const auto blockers = occupied() ^ bb.north() ^ bb.north().east();
            const auto mask =
                movegen::rook_moves(ksq, blockers) & bitboards::Rank4;
            const auto asd =
                pieces(!turn(), Piece::Rook) | pieces(!turn(), Piece::Queen);

            // We just enabled a discovered check
            if (mask & asd) {
            } else {
                moves.emplace_back(MoveType::enpassant,
                                   ep_sq.north().east(),
                                   ep_sq,
                                   Piece::Pawn,
                                   Piece::Pawn);
            }
        }
        if (pawns.south().east() & bb & allowed) {
            const auto blockers = occupied() ^ bb.north() ^ bb.north().west();
            const auto mask =
                movegen::rook_moves(ksq, blockers) & bitboards::Rank4;
            const auto asd =
                pieces(!turn(), Piece::Rook) | pieces(!turn(), Piece::Queen);

            // We just enabled a discovered check
            if (mask & asd) {
            } else {
                moves.emplace_back(MoveType::enpassant,
                                   ep_sq.north().west(),
                                   ep_sq,
                                   Piece::Pawn,
                                   Piece::Pawn);
            }
        }
    }
}

}  // namespace libchess
