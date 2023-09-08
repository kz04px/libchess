#include <cassert>
#include "libchess/bitboard.hpp"
#include "libchess/movegen.hpp"
#include "libchess/position.hpp"
#include "libchess/square.hpp"

namespace libchess {

[[nodiscard]] std::vector<Move> Position::legal_captures() const noexcept {
    std::vector<Move> moves;
    moves.reserve(50);
    legal_captures(moves);
    return moves;
}

void Position::legal_captures(std::vector<Move> &moves) const noexcept {
    [[maybe_unused]] const auto start_size = moves.size();
    const auto us = turn();
    const auto them = !us;
    const auto ksq = king_position(us);
    const auto checkers = this->checkers();
    const auto ep_bb = ep_ == squares::OffSq ? Bitboard{} : Bitboard{ep_};
    auto allowed = occupancy(them);

    if (checkers.count() > 1) {
        const auto mask = movegen::king_moves(ksq) & king_allowed() & occupancy(them);
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, ksq, to, Piece::King, cap);
        }
        return;
    } else if (checkers.count() == 1) {
        allowed = Bitboard{checkers.lsb()};
    }

    const auto ray_north_east = [](const Square sq, const Bitboard blockers) {
        auto bb = Bitboard(sq).north().east();
        bb |= (bb & ~blockers).north().east();
        bb |= (bb & ~blockers).north().east();
        bb |= (bb & ~blockers).north().east();
        bb |= (bb & ~blockers).north().east();
        bb |= (bb & ~blockers).north().east();
        bb |= (bb & ~blockers).north().east();
        return bb;
    };

    const auto ray_south_west = [](const Square sq, const Bitboard blockers) {
        auto bb = Bitboard(sq).south().west();
        bb |= (bb & ~blockers).south().west();
        bb |= (bb & ~blockers).south().west();
        bb |= (bb & ~blockers).south().west();
        bb |= (bb & ~blockers).south().west();
        bb |= (bb & ~blockers).south().west();
        bb |= (bb & ~blockers).south().west();
        return bb;
    };

    const auto ray_east = [](const Square sq, const Bitboard blockers) {
        auto bb = Bitboard(sq).east();
        bb |= (bb & ~blockers).east();
        bb |= (bb & ~blockers).east();
        bb |= (bb & ~blockers).east();
        bb |= (bb & ~blockers).east();
        bb |= (bb & ~blockers).east();
        bb |= (bb & ~blockers).east();
        return bb;
    };

    const auto ray_west = [](const Square sq, const Bitboard blockers) {
        auto bb = Bitboard(sq).west();
        bb |= (bb & ~blockers).west();
        bb |= (bb & ~blockers).west();
        bb |= (bb & ~blockers).west();
        bb |= (bb & ~blockers).west();
        bb |= (bb & ~blockers).west();
        bb |= (bb & ~blockers).west();
        return bb;
    };

    const auto kfile = bitboards::files[ksq.file()];
    const auto krank = bitboards::ranks[ksq.rank()];
    const auto pinned = this->pinned();
    const auto pinned_horizontal = pinned & krank;
    const auto pinned_vertical = pinned & kfile;
    const auto pinned_rook = pinned_horizontal | pinned_vertical;
    const auto pinned_bishop = pinned ^ pinned_rook;
    const auto pinned_ne_sw = pinned_bishop & (ray_north_east(ksq, occupied()) | ray_south_west(ksq, occupied()));
    const auto pinned_nw_se = pinned_bishop ^ pinned_ne_sw;

    const auto bishop_xrays = movegen::bishop_moves(ksq, occupied() ^ pinned_bishop);
    const auto rook_xrays = movegen::rook_moves(ksq, occupied() ^ pinned_rook);

    assert((pinned_ne_sw | pinned_nw_se) == pinned_bishop);
    assert((pinned_vertical | pinned_horizontal) == pinned_rook);
    assert((pinned_bishop | pinned_rook) == pinned);

    // Pawns
    if (us == Side::White) {
        const auto pawns_ne = pieces(us, Piece::Pawn) & ~pinned_rook & ~pinned_nw_se;
        const auto pawns_nw = pieces(us, Piece::Pawn) & ~pinned_rook & ~pinned_ne_sw;
        const auto promo_ne = pawns_ne & bitboards::Rank7;
        const auto promo_nw = pawns_nw & bitboards::Rank7;
        const auto nonpromo_ne = pawns_ne & ~bitboards::Rank7;
        const auto nonpromo_nw = pawns_nw & ~bitboards::Rank7;

        // Captures -- North east
        for (const auto &sq : nonpromo_ne.north().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.south().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- North west
        for (const auto &sq : nonpromo_nw.north().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.south().east(), sq, Piece::Pawn, cap);
        }

        // Promo Captures -- North east
        for (const auto &sq : promo_ne.north().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // Promo Captures -- North west
        for (const auto &sq : promo_nw.north().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // En passant
        if (ep_bb) {
            const auto rq = pieces(Side::Black, Piece::Rook) | pieces(Side::Black, Piece::Queen);

            // North west
            if (pawns_nw & ep_bb.south().east()) {
                const auto blockers = occupied() ^ ep_bb ^ ep_bb.south() ^ ep_bb.south().east();
                const auto east = ray_east(ksq, blockers);
                const auto west = ray_west(ksq, blockers);
                if (east & rq || west & rq) {
                } else {
                    moves.emplace_back(MoveType::enpassant, ep_.south().east(), ep_, Piece::Pawn, Piece::Pawn);
                }
            }
            // North east
            if (pawns_ne & ep_bb.south().west()) {
                const auto blockers = occupied() ^ ep_bb ^ ep_bb.south() ^ ep_bb.south().west();
                const auto east = ray_east(ksq, blockers);
                const auto west = ray_west(ksq, blockers);
                if (east & rq || west & rq) {
                } else {
                    moves.emplace_back(MoveType::enpassant, ep_.south().west(), ep_, Piece::Pawn, Piece::Pawn);
                }
            }
        }
    } else {
        const auto pawns_se = pieces(us, Piece::Pawn) & ~pinned_rook & ~pinned_ne_sw;
        const auto pawns_sw = pieces(us, Piece::Pawn) & ~pinned_rook & ~pinned_nw_se;
        const auto promo_se = pawns_se & bitboards::Rank2;
        const auto promo_sw = pawns_sw & bitboards::Rank2;
        const auto nonpromo_se = pawns_se & ~bitboards::Rank2;
        const auto nonpromo_sw = pawns_sw & ~bitboards::Rank2;

        // Captures -- South east
        for (const auto &sq : nonpromo_se.south().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.north().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- South west
        for (const auto &sq : nonpromo_sw.south().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.north().east(), sq, Piece::Pawn, cap);
        }

        // Promo Captures -- South east
        for (const auto &sq : promo_se.south().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // Promo Captures -- South west
        for (const auto &sq : promo_sw.south().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // En passant
        if (ep_bb) {
            const auto rq = pieces(Side::White, Piece::Rook) | pieces(Side::White, Piece::Queen);

            // South west
            if (nonpromo_sw & ep_bb.north().east() & ~pinned_nw_se) {
                const auto blockers = occupied() ^ ep_bb.north() ^ ep_bb.north().east();
                const auto east = ray_east(ksq, blockers);
                const auto west = ray_west(ksq, blockers);
                if (east & rq || west & rq) {
                } else {
                    moves.emplace_back(MoveType::enpassant, ep_.north().east(), ep_, Piece::Pawn, Piece::Pawn);
                }
            }
            // South east
            if (nonpromo_se & ep_bb.north().west() & ~pinned_ne_sw) {
                const auto blockers = occupied() ^ ep_bb.north() ^ ep_bb.north().west();
                const auto east = ray_east(ksq, blockers);
                const auto west = ray_west(ksq, blockers);
                if (east & rq || west & rq) {
                } else {
                    moves.emplace_back(MoveType::enpassant, ep_.north().west(), ep_, Piece::Pawn, Piece::Pawn);
                }
            }
        }
    }

    // Knights
    for (const auto &fr : pieces(us, Piece::Knight) & ~pinned) {
        const auto mask = movegen::knight_moves(fr) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Knight, cap);
        }
    }

    // Bishops -- nonpinned
    for (const auto &fr : pieces(us, Piece::Bishop) & ~pinned) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Bishop, cap);
        }
    }
    // Bishops -- pinned
    for (const auto &fr : pieces(us, Piece::Bishop) & pinned_bishop) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed & bishop_xrays;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Bishop, cap);
        }
    }

    // Rooks -- nonpinned
    for (const auto &fr : pieces(us, Piece::Rook) & ~pinned) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Rook, cap);
        }
    }
    // Rooks -- pinned
    for (const auto &fr : pieces(us, Piece::Rook) & pinned_rook) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed & rook_xrays;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Rook, cap);
        }
    }

    // Queens -- queen moves -- nonpinned
    for (const auto &fr : pieces(us, Piece::Queen) & ~pinned) {
        const auto mask = movegen::queen_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Queen, cap);
        }
    }
    // Queens -- bishop moves -- bishop pinned
    for (const auto &fr : pieces(us, Piece::Queen) & pinned_bishop) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed & bishop_xrays;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Queen, cap);
        }
    }
    // Queens -- rook moves -- rook pinned
    for (const auto &fr : pieces(us, Piece::Queen) & pinned_rook) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed & rook_xrays;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Queen, cap);
        }
    }

    // King
    {
        const auto mask = movegen::king_moves(ksq) & king_allowed() & occupancy(them);
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, ksq, to, Piece::King, cap);
        }
    }

#ifndef NDEBUG
    for (std::size_t i = start_size; i < moves.size(); ++i) {
        assert(moves[i].is_capturing());
        assert(moves[i].captured() != Piece::None);
        assert(moves[i].captured() != Piece::King);
    }
#endif
}

}  // namespace libchess
