#include <cassert>
#include <iostream>
#include "libchess/bitboard.hpp"
#include "libchess/movegen.hpp"
#include "libchess/position.hpp"
#include "libchess/square.hpp"

namespace libchess {

void Position::pinned_moves(std::vector<Move> &moves,
                            const Bitboard &p,
                            const Bitboard &allowed) const noexcept {
    const auto ksq = king_position(turn());
    const auto kbb = Bitboard{ksq};
    const Bitboard rook_pinned = p & movegen::rook_moves(ksq, occupied());
    const Bitboard bishop_pinned = p & movegen::bishop_moves(ksq, occupied());
    const Bitboard weow = movegen::rook_moves(ksq, occupied()) |
                          movegen::bishop_moves(ksq, occupied());

    assert((rook_pinned & p) == rook_pinned);
    assert((bishop_pinned & p) == bishop_pinned);

    // Bishop pinned
    for (const auto &sq : bishop_pinned) {
        const auto bb = Bitboard{sq};
        const auto piece = piece_on(sq);
        const auto blockers = occupied() ^ bb ^ kbb;
        const auto discovery = movegen::bishop_moves(ksq, blockers);
        const auto diff = blockers & discovery & ~weow;

        assert(diff.count() == 1);
        const auto asq = diff.lsbll();
        assert(piece_on(asq) == Piece::Bishop || piece_on(asq) == Piece::Queen);
        assert(Bitboard{asq} & occupancy(!turn()));
        const auto move_mask =
            ((squares_between(ksq, asq) | diff) ^ bb) & allowed;

        if (piece == Piece::Pawn) {
            if (turn() == Side::White) {
                pawn_moves_white(moves, bb & ~bitboards::Rank7, move_mask);
                promotions_white(moves, bb & bitboards::Rank7, move_mask);
            } else {
                pawn_moves_black(moves, bb & ~bitboards::Rank2, move_mask);
                promotions_black(moves, bb & bitboards::Rank2, move_mask);
            }
        } else if (piece == Piece::Bishop) {
            bishop_moves(moves, bb, move_mask);
        } else if (piece == Piece::Rook) {
            rook_moves(moves, bb, move_mask);
        } else if (piece == Piece::Queen) {
            queen_moves(moves, bb, move_mask);
        }
    }

    // Rook pinned
    for (const auto &sq : rook_pinned) {
        const auto bb = Bitboard{sq};
        const auto piece = piece_on(sq);
        const auto blockers = occupied() ^ bb ^ kbb;
        const auto discovery = movegen::rook_moves(ksq, blockers);
        const auto diff = blockers & discovery & ~weow;
        assert(diff.count() == 1);
        const auto asq = diff.lsbll();
        assert(piece_on(asq) == Piece::Rook || piece_on(asq) == Piece::Queen);
        assert(Bitboard{asq} & occupancy(!turn()));
        const auto move_mask =
            ((squares_between(ksq, asq) | diff) ^ bb) & allowed;

        if (piece == Piece::Pawn) {
            if (turn() == Side::White) {
                pawn_moves_white(moves, bb & ~bitboards::Rank7, move_mask);
                promotions_white(moves, bb & bitboards::Rank7, move_mask);
            } else {
                pawn_moves_black(moves, bb & ~bitboards::Rank2, move_mask);
                promotions_black(moves, bb & bitboards::Rank2, move_mask);
            }
        } else if (piece == Piece::Bishop) {
            bishop_moves(moves, bb, move_mask);
        } else if (piece == Piece::Rook) {
            rook_moves(moves, bb, move_mask);
        } else if (piece == Piece::Queen) {
            queen_moves(moves, bb, move_mask);
        }
    }
}

void Position::knight_moves(
    std::vector<Move> &moves,
    const Bitboard &pieces,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    for (const auto &fr : pieces) {
        const auto mask = movegen::knight_moves(fr) & allowed;
        // Captures
        for (const auto &to : occupancy(!turn()) & mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Knight, cap);
        }
        // Non-captures
        for (const auto &to : empty() & mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Knight);
        }
    }
}

void Position::bishop_moves(
    std::vector<Move> &moves,
    const Bitboard &pieces,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    for (const auto &fr : pieces) {
        const auto mask = movegen::bishop_moves(fr, ~empty()) & allowed;
        // Captures
        for (const auto &to : occupancy(!turn()) & mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Bishop, cap);
        }
        // Non-captures
        for (const auto &to : empty() & mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Bishop);
        }
    }
}

void Position::rook_moves(std::vector<Move> &moves,
                          const Bitboard &pieces,
                          const Bitboard &allowed = bitboards::AllSquares) const
    noexcept {
    for (const auto &fr : pieces) {
        const auto mask = movegen::rook_moves(fr, ~empty()) & allowed;
        // Captures
        for (const auto &to : occupancy(!turn()) & mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Rook, cap);
        }
        // Non-captures
        for (const auto &to : empty() & mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Rook);
        }
    }
}

void Position::queen_moves(
    std::vector<Move> &moves,
    const Bitboard &pieces,
    const Bitboard &allowed = bitboards::AllSquares) const noexcept {
    for (const auto &fr : pieces) {
        const auto mask = movegen::queen_moves(fr, ~empty()) & allowed;
        // Captures
        for (const auto &to : occupancy(!turn()) & mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::Queen, cap);
        }
        // Non-captures
        for (const auto &to : empty() & mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::Queen);
        }
    }
}

void Position::king_moves(std::vector<Move> &moves,
                          const Bitboard &pieces,
                          const Bitboard &allowed = bitboards::AllSquares) const
    noexcept {
    for (const auto &fr : pieces) {
        const auto mask = movegen::king_moves(fr) & allowed;
        // Captures
        for (const auto &to : occupancy(!turn()) & mask) {
            const auto cap = piece_on(to);
            assert(cap != Piece::None);
            assert(cap != Piece::King);
            moves.emplace_back(MoveType::Capture, fr, to, Piece::King, cap);
        }
        // Non-captures
        for (const auto &to : empty() & mask) {
            moves.emplace_back(MoveType::Normal, fr, to, Piece::King);
        }
    }
}

[[nodiscard]] std::vector<Move> Position::legal_moves() const noexcept {
    const auto ch = checkers();
    const int num_checkers = ch.count();

    // If we're in check from more than one piece, we can only play an evading
    // king move
    if (num_checkers > 1) {
        return check_evasions();
    }

    Bitboard allowed = bitboards::AllSquares;

    // If we're in check by one piece, we can try capture it or block
    if (num_checkers == 1) {
        const auto ksq = king_position(turn());
        const auto sq = ch.lsbll();
        assert(piece_on(sq) != Piece::None);
        assert(piece_on(sq) != Piece::King);

        // We can capture the attacker
        allowed = ch;

        // We can block the attack
        allowed |= squares_between(ksq, sq);
    }

    const auto them = !turn();
    std::vector<Move> moves;
    moves.reserve(50);

    const auto pinboys = pinned();
    const auto pawns = pieces(turn(), Piece::Pawn);
    const auto knights = pieces(turn(), Piece::Knight);
    const auto bishops = pieces(turn(), Piece::Bishop);
    const auto rooks = pieces(turn(), Piece::Rook);
    const auto queens = pieces(turn(), Piece::Queen);
    const auto kings = pieces(turn(), Piece::King);

    pinned_moves(moves, pinboys, allowed);

    if (turn() == Side::White) {
        const auto no_promo = pawns & ~pinboys & ~bitboards::Rank7;
        const auto promo = pawns & ~pinboys & bitboards::Rank7;
        pawn_moves_white(moves, no_promo, allowed);
        promotions_white(moves, promo, allowed);
    } else {
        const auto no_promo = pawns & ~pinboys & ~bitboards::Rank2;
        const auto promo = pawns & ~pinboys & bitboards::Rank2;
        pawn_moves_black(moves, no_promo, allowed);
        promotions_black(moves, promo, allowed);
    }
    knight_moves(moves, knights & ~pinboys, allowed);
    bishop_moves(moves, bishops & ~pinboys, allowed);
    rook_moves(moves, rooks & ~pinboys, allowed);
    queen_moves(moves, queens & ~pinboys, allowed);
    king_moves(moves, kings, king_allowed());

    // Castling
    if (turn() == Side::White) {
        if (can_castle(Side::White, MoveType::ksc) &&
            piece_on(squares::F1) == Piece::None &&
            piece_on(squares::G1) == Piece::None &&
            !square_attacked(squares::E1, them) &&
            !square_attacked(squares::F1, them) &&
            !square_attacked(squares::G1, them)) {
            moves.emplace_back(
                MoveType::ksc, squares::E1, squares::G1, Piece::King);
        }
        if (can_castle(Side::White, MoveType::qsc) &&
            piece_on(squares::D1) == Piece::None &&
            piece_on(squares::C1) == Piece::None &&
            piece_on(squares::B1) == Piece::None &&
            !square_attacked(squares::E1, them) &&
            !square_attacked(squares::D1, them) &&
            !square_attacked(squares::C1, them)) {
            moves.emplace_back(
                MoveType::qsc, squares::E1, squares::C1, Piece::King);
        }
    } else {
        if (can_castle(Side::Black, MoveType::ksc) &&
            piece_on(squares::F8) == Piece::None &&
            piece_on(squares::G8) == Piece::None &&
            !square_attacked(squares::E8, them) &&
            !square_attacked(squares::F8, them) &&
            !square_attacked(squares::G8, them)) {
            moves.emplace_back(
                MoveType::ksc, squares::E8, squares::G8, Piece::King);
        }
        if (can_castle(Side::Black, MoveType::qsc) &&
            piece_on(squares::D8) == Piece::None &&
            piece_on(squares::C8) == Piece::None &&
            piece_on(squares::B8) == Piece::None &&
            !square_attacked(squares::E8, them) &&
            !square_attacked(squares::D8, them) &&
            !square_attacked(squares::C8, them)) {
            moves.emplace_back(
                MoveType::qsc, squares::E8, squares::C8, Piece::King);
        }
    }

    return moves;
}

}  // namespace libchess
