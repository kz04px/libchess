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
    const auto ch = checkers();
    const auto ksq = king_position(us);
    [[maybe_unused]] const auto kfile = bitboards::files[ksq.file()];
    [[maybe_unused]] const auto krank = bitboards::ranks[ksq.rank()];
    const auto ep_bb = ep_ == 0xFF ? Bitboard{}
                                   : bitboards::files[ep_] &
                                         (us == Side::White ? bitboards::Rank6
                                                            : bitboards::Rank3);

    // If we're in check multiple times, only the king can move
    if (ch.count() > 1) {
        for (const auto &fr : pieces(us, Piece::King)) {
            const auto mask = movegen::king_moves(fr) & king_allowed();
            for (const auto &to : occupancy(them) & mask) {
                const auto cap = piece_on(to);
                assert(cap != Piece::None);
                assert(cap != Piece::King);
                moves.emplace_back(MoveType::Capture, fr, to, Piece::King, cap);
            }
        }
        return;
    }

    auto allowed = occupancy(them);

    // If we're in check by one piece, we can try capture it
    if (ch.count() == 1) {
        const auto sq = ch.lsbll();
        assert(piece_on(sq) != Piece::None);
        assert(piece_on(sq) != Piece::King);

        allowed = Bitboard{sq};
    }

    const Bitboard bishop_rays = movegen::bishop_moves(ksq, occupied());
    const Bitboard rook_rays = movegen::rook_moves(ksq, occupied());

    Bitboard bishop_pinned;
    Bitboard rook_pinned;

    // Bishop pinned
    {
        for (const auto &sq : occupancy(us) & bishop_rays) {
            const auto bb = Bitboard{sq};
            const auto blockers = occupied() ^ bb;
            const auto new_rays = movegen::bishop_moves(ksq, blockers);
            const auto discovery = new_rays ^ bishop_rays;
            const auto attackers = discovery & (pieces(them, Piece::Bishop) |
                                                pieces(them, Piece::Queen));

            if (attackers) {
                bishop_pinned |= bb;
                if (attackers & allowed) {
                    const auto asq = attackers.lsbll();
                    const auto cap = piece_on(asq);
                    assert(cap == Piece::Bishop || cap == Piece::Queen);

                    if (bb & pieces(us, Piece::Pawn)) {
                        if (us == Side::White) {
                            // We can capture the attacker
                            if ((bb.north().east() & attackers) ||
                                (bb.north().west() & attackers)) {
                                // Capture with promotion
                                if (attackers & bitboards::Rank8) {
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Queen);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Rook);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Bishop);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Knight);
                                }
                                // Regular capture
                                else {
                                    moves.emplace_back(MoveType::Capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap);
                                }
                            }
                            // If we can't capture the pinner, en passant might
                            // maintain the pin
                            else if (ep_bb & squares_between(ksq, asq) &
                                     bb.north().west()) {
                                moves.emplace_back(MoveType::enpassant,
                                                   sq,
                                                   sq.north().west(),
                                                   Piece::Pawn,
                                                   Piece::Pawn);
                            }
                            // If we can't capture the pinner, en passant might
                            // maintain the pin
                            else if (ep_bb & squares_between(ksq, asq) &
                                     bb.north().east()) {
                                moves.emplace_back(MoveType::enpassant,
                                                   sq,
                                                   sq.north().east(),
                                                   Piece::Pawn,
                                                   Piece::Pawn);
                            }
                        } else {
                            if ((bb.south().east() & attackers) ||
                                (bb.south().west() & attackers)) {
                                if (attackers & bitboards::Rank1) {
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Queen);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Rook);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Bishop);
                                    moves.emplace_back(MoveType::promo_capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap,
                                                       Piece::Knight);
                                } else {
                                    moves.emplace_back(MoveType::Capture,
                                                       sq,
                                                       asq,
                                                       Piece::Pawn,
                                                       cap);
                                }
                            }
                            // If we can't capture the pinner, en passant might
                            // maintain the pin
                            else if (ep_bb & squares_between(ksq, asq) &
                                     bb.south().west()) {
                                moves.emplace_back(MoveType::enpassant,
                                                   sq,
                                                   sq.south().west(),
                                                   Piece::Pawn,
                                                   Piece::Pawn);
                            }
                            // If we can't capture the pinner, en passant might
                            // maintain the pin
                            else if (ep_bb & squares_between(ksq, asq) &
                                     bb.south().east()) {
                                moves.emplace_back(MoveType::enpassant,
                                                   sq,
                                                   sq.south().east(),
                                                   Piece::Pawn,
                                                   Piece::Pawn);
                            }
                        }
                    } else if (bb & pieces(us, Piece::Bishop)) {
                        moves.emplace_back(
                            MoveType::Capture, sq, asq, Piece::Bishop, cap);
                    } else if (bb & pieces(us, Piece::Queen)) {
                        moves.emplace_back(
                            MoveType::Capture, sq, asq, Piece::Queen, cap);
                    }
                }
            }
        }
    }

    // Rook pinned
    {
        for (const auto &sq : occupancy(us) & rook_rays) {
            const auto bb = Bitboard{sq};
            const auto blockers = occupied() ^ bb;
            const auto new_rays = movegen::rook_moves(ksq, blockers);
            const auto discovery = new_rays ^ rook_rays;
            const auto attackers = discovery & (pieces(them, Piece::Rook) |
                                                pieces(them, Piece::Queen));

            if (attackers) {
                rook_pinned |= bb;
                if (attackers & allowed) {
                    const auto asq = attackers.lsbll();
                    const auto apiece = piece_on(asq);

                    if (bb & pieces(us, Piece::Rook)) {
                        moves.emplace_back(
                            MoveType::Capture, sq, asq, Piece::Rook, apiece);
                    } else if (bb & pieces(us, Piece::Queen)) {
                        moves.emplace_back(
                            MoveType::Capture, sq, asq, Piece::Queen, apiece);
                    }
                }
            }
        }
    }

    const Bitboard pinned_pieces = rook_pinned | bishop_pinned;
    const Bitboard nonpinned_pieces = occupancy(us) ^ pinned_pieces;

    assert(pinned_pieces == pinned());
    assert(rook_pinned == (rook_pinned & (kfile | krank)));

    // Pawns
    if (us == Side::White) {
        const auto pawns = pieces(us, Piece::Pawn) & nonpinned_pieces;
        const auto promo = pawns & bitboards::Rank7;
        const auto nonpromo = pawns & ~bitboards::Rank7;

        // Captures -- Right
        for (const auto &sq : nonpromo.north().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(
                MoveType::Capture, sq.south().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- left
        for (const auto &sq : nonpromo.north().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(
                MoveType::Capture, sq.south().east(), sq, Piece::Pawn, cap);
        }

        // Promo Captures -- Right
        for (const auto &sq : promo.north().east() & allowed) {
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
        for (const auto &sq : promo.north().west() & allowed) {
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

        // En passant
        if (ep_ != 0xFF) {
            const auto file = bitboards::files[ep_];
            const auto bb = file & bitboards::Rank6;
            const auto ep_sq = bb.lsbll();

            if (pawns.north().west() & bb) {
                const auto blockers =
                    occupied() ^ bb.south() ^ bb.south().east();
                const auto mask =
                    movegen::rook_moves(ksq, blockers) & bitboards::Rank5;
                const auto asd =
                    pieces(them, Piece::Rook) | pieces(them, Piece::Queen);

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
            if (pawns.north().east() & bb) {
                const auto blockers =
                    occupied() ^ bb.south() ^ bb.south().west();
                const auto mask =
                    movegen::rook_moves(ksq, blockers) & bitboards::Rank5;
                const auto asd =
                    pieces(them, Piece::Rook) | pieces(them, Piece::Queen);

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
    } else {
        const auto pawns = pieces(us, Piece::Pawn) & nonpinned_pieces;
        const auto promo = pawns & bitboards::Rank2;
        const auto nonpromo = pawns & ~bitboards::Rank2;

        // Captures -- Right
        for (const auto &sq : nonpromo.south().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(
                MoveType::Capture, sq.north().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- left
        for (const auto &sq : nonpromo.south().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(
                MoveType::Capture, sq.north().east(), sq, Piece::Pawn, cap);
        }

        // Captures -- Right
        for (const auto &sq : promo.south().east() & allowed) {
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
        for (const auto &sq : promo.south().west() & allowed) {
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

        // En passant
        if (ep_ != 0xFF) {
            const auto file = bitboards::files[ep_];
            const auto bb = file & bitboards::Rank3;
            const auto ep_sq = bb.lsbll();

            if (pawns.south().west() & bb) {
                const auto blockers =
                    occupied() ^ bb.north() ^ bb.north().east();
                const auto mask =
                    movegen::rook_moves(ksq, blockers) & bitboards::Rank4;
                const auto asd = pieces(!turn(), Piece::Rook) |
                                 pieces(!turn(), Piece::Queen);

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
            if (pawns.south().east() & bb) {
                const auto blockers =
                    occupied() ^ bb.north() ^ bb.north().west();
                const auto mask =
                    movegen::rook_moves(ksq, blockers) & bitboards::Rank4;
                const auto asd = pieces(!turn(), Piece::Rook) |
                                 pieces(!turn(), Piece::Queen);

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

    // Knights
    for (const auto &fr : pieces(us, Piece::Knight) & nonpinned_pieces) {
        const auto mask = movegen::knight_moves(fr) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Knight, cap);
        }
    }

    // Bishops
    for (const auto &fr : pieces(us, Piece::Bishop) & nonpinned_pieces) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Bishop, cap);
        }
    }

    // Rooks
    for (const auto &fr : pieces(us, Piece::Rook) & nonpinned_pieces) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Rook, cap);
        }
    }

    // Queens
    for (const auto &fr : pieces(us, Piece::Queen) & nonpinned_pieces) {
        const auto mask = movegen::queen_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Queen, cap);
        }
    }

    // King
    {
        const auto mask =
            movegen::king_moves(ksq) & king_allowed() & occupancy(them);
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
