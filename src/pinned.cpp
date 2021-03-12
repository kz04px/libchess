#include <iostream>
#include "libchess/movegen.hpp"
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] Bitboard Position::pinned() const noexcept {
    return pinned(turn(), king_position(turn()));
}

[[nodiscard]] Bitboard Position::pinned(const Side s) const noexcept {
    return pinned(s, king_position(s));
}

[[nodiscard]] Bitboard Position::pinned(const Side s, const Square sq) const noexcept {
    Bitboard pinned;

    const Bitboard before = movegen::rook_moves(sq, occupied()) | movegen::bishop_moves(sq, occupied());

    // Bishop stuff
    {
        const auto mask = movegen::bishop_moves(sq, occupied()) & occupancy(s);
        for (const auto &nsq : mask) {
            const auto bb = Bitboard{nsq};
            const auto blockers = occupied() ^ bb;
            const auto discovery = movegen::bishop_moves(sq, blockers);
            const auto diff = blockers & discovery & ~before;
            const auto attackers = diff & (pieces(!turn(), Piece::Bishop) | pieces(!turn(), Piece::Queen));

            if (attackers) {
                pinned |= bb;
            }
        }
    }

    // Rook stuff
    {
        const auto mask = movegen::rook_moves(sq, occupied()) & occupancy(s);
        for (const auto &nsq : mask) {
            const auto bb = Bitboard{nsq};
            const auto blockers = occupied() ^ bb;
            const auto discovery = movegen::rook_moves(sq, blockers);
            const auto diff = blockers & discovery & ~before;
            const auto attackers = diff & (pieces(!turn(), Piece::Rook) | pieces(!turn(), Piece::Queen));

            if (attackers) {
                pinned |= bb;
            }
        }
    }

    assert(pinned.count() <= 8);
    assert((pinned & occupancy(s)) == pinned);

    return pinned;
}

}  // namespace libchess
