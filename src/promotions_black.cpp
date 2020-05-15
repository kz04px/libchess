#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

void Position::promotions_black(
    std::vector<Move> &moves,
    const Bitboard &pieces,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    assert((pieces & bitboards::Rank2) == pieces);

    // Singles
    for (const auto &sq : empty() & pieces.south() & allowed) {
        moves.emplace_back(MoveType::promo,
                           sq.north(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo,
                           sq.north(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo,
                           sq.north(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo,
                           sq.north(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Knight);
    }

    // Captures -- Right
    for (const auto &sq :
         pieces.south().east() & occupancy(!turn()) & allowed) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Knight);
    }

    // Captures -- left
    for (const auto &sq :
         pieces.south().west() & occupancy(!turn()) & allowed) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo_capture,
                           sq.north().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Knight);
    }
}

}  // namespace libchess
