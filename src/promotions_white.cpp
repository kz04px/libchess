#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

void Position::promotions_white(
    std::vector<Move> &moves,
    const Bitboard &pieces,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    assert((pieces & bitboards::Rank7) == pieces);

    // Singles
    const auto singles = empty() & pieces.north() & allowed;
    for (const auto &sq : singles) {
        moves.emplace_back(MoveType::promo,
                           sq.south(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo,
                           sq.south(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo,
                           sq.south(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo,
                           sq.south(),
                           sq,
                           Piece::Pawn,
                           Piece::None,
                           Piece::Knight);
    }

    // Captures -- Right
    const auto cap_right = pieces.north().east() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_right) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().west(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Knight);
    }

    // Captures -- left
    const auto cap_left = pieces.north().west() & occupancy(!turn()) & allowed;
    for (const auto &sq : cap_left) {
        const auto cap = piece_on(sq);
        assert(cap != Piece::None);
        assert(cap != Piece::King);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Queen);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Rook);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Bishop);
        moves.emplace_back(MoveType::promo_capture,
                           sq.south().east(),
                           sq,
                           Piece::Pawn,
                           cap,
                           Piece::Knight);
    }
}

}  // namespace libchess
