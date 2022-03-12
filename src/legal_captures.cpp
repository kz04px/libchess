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

    const auto kfile = bitboards::files[ksq.file()];
    const auto krank = bitboards::ranks[ksq.rank()];
    const auto pinned = this->pinned();
    const auto pinned_horizontal = pinned & krank;
    const auto pinned_vertical = pinned & kfile;
    const auto pinned_rook = pinned_horizontal | pinned_vertical;
    const auto pinned_bishop = pinned ^ pinned_rook;

    // Pawns
    if (us == Side::White) {
        const auto pawns = pieces(us, Piece::Pawn) & ~pinned_rook;
        const auto promo = pawns & bitboards::Rank7;
        const auto nonpromo = pawns & ~bitboards::Rank7;

        // Captures -- Right
        for (const auto &sq : nonpromo.north().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.south().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- left
        for (const auto &sq : nonpromo.north().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.south().east(), sq, Piece::Pawn, cap);
        }

        // Promo Captures -- Right
        for (const auto &sq : promo.north().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.south().west(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // Promo Captures -- left
        for (const auto &sq : promo.north().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.south().east(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // En passant
        if (ep_ != squares::OffSq) {
            if (pawns.north().west() & ep_bb) {
                moves.emplace_back(MoveType::enpassant, ep_.south().east(), ep_, Piece::Pawn, Piece::Pawn);
            }
            if (pawns.north().east() & ep_bb) {
                moves.emplace_back(MoveType::enpassant, ep_.south().west(), ep_, Piece::Pawn, Piece::Pawn);
            }
        }
    } else {
        const auto pawns = pieces(us, Piece::Pawn) & ~pinned_rook;
        const auto promo = pawns & bitboards::Rank2;
        const auto nonpromo = pawns & ~bitboards::Rank2;

        // Captures -- Right
        for (const auto &sq : nonpromo.south().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.north().west(), sq, Piece::Pawn, cap);
        }

        // Captures -- left
        for (const auto &sq : nonpromo.south().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, sq.north().east(), sq, Piece::Pawn, cap);
        }

        // Promo Captures -- Right
        for (const auto &sq : promo.south().east() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.north().west(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // Promo Captures -- left
        for (const auto &sq : promo.south().west() & allowed) {
            const auto cap = piece_on(sq);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Queen);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Rook);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Bishop);
            moves.emplace_back(MoveType::promo_capture, sq.north().east(), sq, Piece::Pawn, cap, Piece::Knight);
        }

        // En passant
        if (ep_ != squares::OffSq) {
            if (pawns.south().west() & ep_bb) {
                moves.emplace_back(MoveType::enpassant, ep_.north().east(), ep_, Piece::Pawn, Piece::Pawn);
            }
            if (pawns.south().east() & ep_bb) {
                moves.emplace_back(MoveType::enpassant, ep_.north().west(), ep_, Piece::Pawn, Piece::Pawn);
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

    // Bishops
    for (const auto &fr : pieces(us, Piece::Bishop) & ~pinned_rook) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Bishop, cap);
        }
    }

    // Rooks
    for (const auto &fr : pieces(us, Piece::Rook) & ~pinned_bishop) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed;
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Rook, cap);
        }
    }

    // Queens
    for (const auto &fr : pieces(us, Piece::Queen)) {
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
        const auto mask = movegen::king_moves(ksq) & king_allowed() & occupancy(them);
        for (const auto &to : mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, ksq, to, Piece::King, cap);
        }
    }

    // Filter pseudolegal moves
    const auto pawn_attackers = pieces(them, Piece::Pawn);
    const auto knight_attackers = pieces(them, Piece::Knight);
    const auto bishop_attackers = pieces(them, Piece::Bishop) | pieces(them, Piece::Queen);
    const auto rook_attackers = pieces(them, Piece::Rook) | pieces(them, Piece::Queen);

    const auto trash = us == Side::White ? ep_bb.south() : ep_bb.north();
    // const auto pinned = in_check ? bitboards::AllSquares : this->pinned();

    std::size_t back = start_size;
    for (std::size_t i = start_size; i < moves.size(); ++i) {
        const auto bb_from = Bitboard{moves[i].from()};
        auto legal = true;

        if (moves[i].piece() != Piece::Knight && ((bb_from & pinned) || moves[i].type() == MoveType::enpassant)) {
            const auto nksq = moves[i].piece() == Piece::King ? moves[i].to() : ksq;
            auto blockers = (Bitboard{moves[i].from()} ^ occupied()) | Bitboard{moves[i].to()};
            auto new_pawns = pawn_attackers & ~Bitboard{moves[i].to()};

            if (moves[i].type() == MoveType::enpassant) {
                blockers ^= trash;
                new_pawns ^= trash;
            }

            const auto pawn_attacked =
                Bitboard{nksq} & (us == Side::White ? new_pawns.south().east() | new_pawns.south().west()
                                                    : new_pawns.north().east() | new_pawns.north().west());

            if (pawn_attacked) {
                legal = false;
            } else if (movegen::knight_moves(nksq) & knight_attackers & ~Bitboard{moves[i].to()}) {
                legal = false;
            } else if (movegen::bishop_moves(nksq, blockers) & bishop_attackers & ~Bitboard{moves[i].to()}) {
                legal = false;
            } else if (movegen::rook_moves(nksq, blockers) & rook_attackers & ~Bitboard{moves[i].to()}) {
                legal = false;
            }
        }

        if (legal) {
            if (i > back) {
                moves[back] = moves[i];
            }
            back++;
        }
    }

    moves.resize(back);

#ifndef NDEBUG
    for (std::size_t i = start_size; i < moves.size(); ++i) {
        assert(moves[i].is_capturing());
        assert(moves[i].captured() != Piece::None);
        assert(moves[i].captured() != Piece::King);
    }
#endif
}

}  // namespace libchess
