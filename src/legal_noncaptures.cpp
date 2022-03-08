#include <cassert>
#include "libchess/bitboard.hpp"
#include "libchess/movegen.hpp"
#include "libchess/position.hpp"
#include "libchess/square.hpp"

namespace libchess {

[[nodiscard]] std::vector<Move> Position::legal_noncaptures() const noexcept {
    std::vector<Move> moves;
    moves.reserve(50);
    legal_noncaptures(moves);
    return moves;
}

void Position::legal_noncaptures(std::vector<Move> &moves) const noexcept {
    [[maybe_unused]] const auto start_size = moves.size();
    const auto us = turn();
    const auto them = !us;
    const auto ch = checkers();
    const auto checked = !ch.empty();
    const auto ksq = king_position(us);
    [[maybe_unused]] const auto kfile = bitboards::files[ksq.file()];
    const auto krank = bitboards::ranks[ksq.rank()];

    // If we're in check multiple times, only the king can move
    if (ch.count() > 1) {
        for (const auto &fr : pieces(us, Piece::King)) {
            const auto mask = movegen::king_moves(fr) & king_allowed();
            for (const auto &to : empty() & mask) {
                moves.emplace_back(MoveType::Normal, fr, to, Piece::King);
            }
        }
        return;
    }

    auto allowed = empty();

    // If we're in check by one piece, we can try block or move the king
    if (ch.count() == 1) {
        const auto sq = ch.lsb();
        assert(piece_on(sq) != Piece::None);
        assert(piece_on(sq) != Piece::King);

        allowed = squares_between(ksq, sq);
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
            const auto attackers = discovery & (pieces(them, Piece::Bishop) | pieces(them, Piece::Queen));

            if (attackers) {
                bishop_pinned |= bb;
                const auto asq = attackers.lsb();
                const auto move_mask = (squares_between(ksq, asq) ^ bb) & allowed;

                if (bb & pieces(us, Piece::Bishop)) {
                    for (const auto &to : move_mask) {
                        moves.emplace_back(MoveType::Normal, sq, to, Piece::Bishop);
                    }
                } else if (bb & pieces(us, Piece::Queen)) {
                    for (const auto &to : move_mask) {
                        moves.emplace_back(MoveType::Normal, sq, to, Piece::Queen);
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
            const auto attackers = discovery & (pieces(them, Piece::Rook) | pieces(them, Piece::Queen));

            if (attackers) {
                rook_pinned |= bb;
                const auto asq = attackers.lsb();
                const auto move_mask = (squares_between(ksq, asq) ^ bb) & allowed;

                if (bb & pieces(us, Piece::Rook)) {
                    for (const auto &to : move_mask) {
                        moves.emplace_back(MoveType::Normal, sq, to, Piece::Rook);
                    }
                } else if (bb & pieces(us, Piece::Queen)) {
                    for (const auto &to : move_mask) {
                        moves.emplace_back(MoveType::Normal, sq, to, Piece::Queen);
                    }
                }
            }
        }
    }

    const Bitboard horizontal_pinned = rook_pinned & krank;
    const Bitboard pinned_pieces = rook_pinned | bishop_pinned;
    const Bitboard nonpinned_pieces = occupancy(us) ^ pinned_pieces;

    assert(pinned_pieces == pinned());
    assert(rook_pinned == (rook_pinned & (kfile | krank)));

    // Pawns
    if (us == Side::White) {
        const auto pawns = pieces(us, Piece::Pawn) & ~(horizontal_pinned | bishop_pinned);
        const auto promo = pawns & bitboards::Rank7;
        const auto nonpromo = pawns & ~bitboards::Rank7;

        // Singles -- Nonpromo
        for (const auto &sq : nonpromo.north() & allowed) {
            moves.emplace_back(MoveType::Normal, sq.south(), sq, Piece::Pawn);
        }

        // Singles -- Promo
        for (const auto &sq : promo.north() & allowed) {
            moves.emplace_back(MoveType::promo, sq.south(), sq, Piece::Pawn, Piece::None, Piece::Queen);
            moves.emplace_back(MoveType::promo, sq.south(), sq, Piece::Pawn, Piece::None, Piece::Rook);
            moves.emplace_back(MoveType::promo, sq.south(), sq, Piece::Pawn, Piece::None, Piece::Bishop);
            moves.emplace_back(MoveType::promo, sq.south(), sq, Piece::Pawn, Piece::None, Piece::Knight);
        }

        // Doubles
        const auto doubles = (empty() & pawns.north()).north() & bitboards::Rank4 & allowed;
        for (const auto &sq : doubles) {
            moves.emplace_back(MoveType::Double, sq.south().south(), sq, Piece::Pawn);
        }
    } else {
        const auto pawns = pieces(us, Piece::Pawn) & ~(horizontal_pinned | bishop_pinned);
        const auto promo = pawns & bitboards::Rank2;
        const auto nonpromo = pawns & ~bitboards::Rank2;

        // Singles -- Nonpromo
        for (const auto &sq : nonpromo.south() & allowed) {
            moves.emplace_back(MoveType::Normal, sq.north(), sq, Piece::Pawn);
        }

        // Singles -- Promo
        for (const auto &sq : promo.south() & allowed) {
            moves.emplace_back(MoveType::promo, sq.north(), sq, Piece::Pawn, Piece::None, Piece::Queen);
            moves.emplace_back(MoveType::promo, sq.north(), sq, Piece::Pawn, Piece::None, Piece::Rook);
            moves.emplace_back(MoveType::promo, sq.north(), sq, Piece::Pawn, Piece::None, Piece::Bishop);
            moves.emplace_back(MoveType::promo, sq.north(), sq, Piece::Pawn, Piece::None, Piece::Knight);
        }

        // Doubles
        const auto doubles = (empty() & pawns.south()).south() & bitboards::Rank5 & allowed;
        for (const auto &sq : doubles) {
            moves.emplace_back(MoveType::Double, sq.north().north(), sq, Piece::Pawn);
        }
    }

    // Knights
    for (const auto &fr : pieces(us, Piece::Knight) & nonpinned_pieces) {
        const auto mask = movegen::knight_moves(fr) & allowed;
        for (const auto &to : mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Knight);
        }
    }

    // Bishops
    for (const auto &fr : pieces(us, Piece::Bishop) & nonpinned_pieces) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Bishop);
        }
    }

    // Rooks
    for (const auto &fr : pieces(us, Piece::Rook) & nonpinned_pieces) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Rook);
        }
    }

    // Queens
    for (const auto &fr : pieces(us, Piece::Queen) & nonpinned_pieces) {
        const auto mask = movegen::queen_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Queen);
        }
    }

    // King
    {
        const auto mask = movegen::king_moves(ksq) & king_allowed() & empty();
        for (const auto &to : mask) {
            moves.emplace_back(MoveType::Normal, ksq, to, Piece::King);
        }
    }

    // Castling
    if (!checked) {
        if (us == Side::White) {
            if (can_castle(Side::White, MoveType::ksc) && piece_on(squares::F1) == Piece::None &&
                piece_on(squares::G1) == Piece::None && !square_attacked(squares::F1, them) &&
                !square_attacked(squares::G1, them)) {
                moves.emplace_back(MoveType::ksc, squares::E1, squares::G1, Piece::King);
            }
            if (can_castle(Side::White, MoveType::qsc) && piece_on(squares::D1) == Piece::None &&
                piece_on(squares::C1) == Piece::None && piece_on(squares::B1) == Piece::None &&
                !square_attacked(squares::D1, them) && !square_attacked(squares::C1, them)) {
                moves.emplace_back(MoveType::qsc, squares::E1, squares::C1, Piece::King);
            }
        } else {
            if (can_castle(Side::Black, MoveType::ksc) && piece_on(squares::F8) == Piece::None &&
                piece_on(squares::G8) == Piece::None && !square_attacked(squares::F8, them) &&
                !square_attacked(squares::G8, them)) {
                moves.emplace_back(MoveType::ksc, squares::E8, squares::G8, Piece::King);
            }
            if (can_castle(Side::Black, MoveType::qsc) && piece_on(squares::D8) == Piece::None &&
                piece_on(squares::C8) == Piece::None && piece_on(squares::B8) == Piece::None &&
                !square_attacked(squares::D8, them) && !square_attacked(squares::C8, them)) {
                moves.emplace_back(MoveType::qsc, squares::E8, squares::C8, Piece::King);
            }
        }
    }

#ifndef NDEBUG
    for (std::size_t i = start_size; i < moves.size(); ++i) {
        assert(!moves[i].is_capturing());
        assert(moves[i].captured() == Piece::None);
    }
#endif
}

}  // namespace libchess
