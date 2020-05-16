#ifndef LIBCHESS_POSITION_HPP
#define LIBCHESS_POSITION_HPP

#include <ostream>
#include <string>
#include <vector>
#include "bitboard.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "side.hpp"

namespace libchess {

namespace {

enum Castling : int
{
    usKSC,
    usQSC,
    themKSC,
    themQSC
};

constexpr const Square ksc_rook_fr[] = {squares::H1, squares::H8};
constexpr const Square qsc_rook_fr[] = {squares::A1, squares::A8};
constexpr const Square ksc_rook_to[] = {squares::F1, squares::F8};
constexpr const Square qsc_rook_to[] = {squares::D1, squares::D8};

}  // namespace

class Position {
   public:
    Position()
        : ep_{},
          colours_{},
          pieces_{},
          halfmove_clock_{0},
          fullmove_clock_{0},
          to_move_{Side::White},
          history_{} {
    }

    Position(const std::string &fen)
        : ep_{},
          colours_{},
          pieces_{},
          halfmove_clock_{0},
          fullmove_clock_{0},
          to_move_{Side::White},
          history_{} {
        set_fen(fen);
    }

    [[nodiscard]] constexpr Side turn() const noexcept {
        return to_move_;
    }

    [[nodiscard]] constexpr Bitboard occupancy(const Side s) const noexcept {
        return colours_[s];
    }

    [[nodiscard]] constexpr Bitboard occupancy(const Piece p) const noexcept {
        return pieces_[p];
    }

    [[nodiscard]] constexpr Bitboard pieces(const Side s, const Piece p) const
        noexcept {
        return occupancy(s) & occupancy(p);
    }

    [[nodiscard]] constexpr Bitboard occupied() const noexcept {
        return occupancy(Side::White) | occupancy(Side::Black);
    }

    [[nodiscard]] constexpr Bitboard empty() const noexcept {
        return ~occupied();
    }

    [[nodiscard]] constexpr std::uint64_t hash() const noexcept {
        return hash_;
    }

    void set_fen(const std::string &fen) noexcept;

    [[nodiscard]] std::string get_fen() const noexcept;

    [[nodiscard]] bool is_legal(const Move &m) const noexcept;

    [[nodiscard]] constexpr bool threefold() const noexcept {
        return true;
    }

    [[nodiscard]] constexpr bool fiftymoves() const noexcept {
        return halfmove_clock_ >= 100;
    }

    [[nodiscard]] constexpr Square king_position(const Side s) const noexcept {
        return pieces(s, Piece::King).lsbll();
    }

    [[nodiscard]] bool square_attacked(const Square sq, const Side s) const
        noexcept;

    [[nodiscard]] Bitboard squares_attacked(const Side s) const noexcept;

    [[nodiscard]] Bitboard checkers() const noexcept;

    [[nodiscard]] Bitboard attackers(const Square sq, const Side s) const
        noexcept;

    [[nodiscard]] constexpr bool in_check() const noexcept {
        return square_attacked(king_position(turn()), !turn());
    }

    [[nodiscard]] Bitboard king_allowed() const noexcept;

    [[nodiscard]] Bitboard king_allowed(const Side s) const noexcept;

    [[nodiscard]] Bitboard pinned() const noexcept;

    [[nodiscard]] Bitboard pinned(const Side s) const noexcept;

    [[nodiscard]] Bitboard pinned(const Side s, const Square sq) const noexcept;

    [[nodiscard]] std::vector<Move> check_evasions() const noexcept;

    [[nodiscard]] std::vector<Move> legal_moves() const noexcept;

    void pinned_moves(std::vector<Move> &moves,
                      const Bitboard &pieces,
                      const Bitboard &allowed) const noexcept;

    [[nodiscard]] std::vector<Move> legal_captures() const noexcept {
        return {};
    }

    void pawn_moves_white(std::vector<Move> &moves,
                          const Bitboard &piece_mask,
                          const Bitboard &allowed) const noexcept;

    void promotions_white(std::vector<Move> &moves,
                          const Bitboard &piece_mask,
                          const Bitboard &allowed) const noexcept;

    void pawn_moves_black(std::vector<Move> &moves,
                          const Bitboard &piece_mask,
                          const Bitboard &allowed) const noexcept;

    void promotions_black(std::vector<Move> &moves,
                          const Bitboard &piece_mask,
                          const Bitboard &allowed) const noexcept;

    void knight_moves(std::vector<Move> &moves,
                      const Bitboard &piece_mask,
                      const Bitboard &allowed) const noexcept;

    void bishop_moves(std::vector<Move> &moves,
                      const Bitboard &piece_mask,
                      const Bitboard &allowed) const noexcept;

    void rook_moves(std::vector<Move> &moves,
                    const Bitboard &piece_mask,
                    const Bitboard &allowed) const noexcept;

    void queen_moves(std::vector<Move> &moves,
                     const Bitboard &piece_mask,
                     const Bitboard &allowed) const noexcept;

    void king_moves(std::vector<Move> &moves,
                    const Bitboard &piece_mask,
                    const Bitboard &allowed) const noexcept;

    [[nodiscard]] std::size_t count_moves() const noexcept;

    [[nodiscard]] std::uint64_t perft(const int depth) noexcept;

    [[nodiscard]] constexpr bool can_castle(const Side s,
                                            const MoveType mt) const noexcept {
        if (s == Side::White) {
            if (mt == MoveType::ksc) {
                return castling_[0];
            } else {
                return castling_[1];
            }
        } else {
            if (mt == MoveType::ksc) {
                return castling_[2];
            } else {
                return castling_[3];
            }
        }
        return true;
    }

    void makemove(const Move &move) noexcept;

    void undomove() noexcept;

    void makenull() noexcept {
        history_.push_back(meh{});
        to_move_ = !to_move_;
    }

    void clear() noexcept {
        colours_[0].clear();
        colours_[1].clear();
        pieces_[0].clear();
        pieces_[1].clear();
        pieces_[2].clear();
        pieces_[3].clear();
        pieces_[4].clear();
        pieces_[5].clear();
        halfmove_clock_ = 0;
        fullmove_clock_ = 0;
        ep_ = 0xFF;
        hash_ = 0x0;
        castling_[0] = false;
        castling_[1] = false;
        castling_[2] = false;
        castling_[3] = false;
        to_move_ = Side::White;
        history_.clear();
    }

    int ep_;

   private:
    void set(const Square sq, const Side s, const Piece p) noexcept {
        colours_[s] |= sq;
        pieces_[p] |= sq;
    }

    [[nodiscard]] constexpr Piece piece_on(const Square sq) const noexcept {
        for (int i = 0; i < 6; ++i) {
            if (pieces_[i] & Bitboard{sq}) {
                return Piece(i);
            }
        }
        return Piece::None;
    }

    [[nodiscard]] constexpr bool valid() const noexcept {
        if (ep_ != 0xFF && ep_ > 7) {
            return false;
        }

        if (pieces(Side::White, Piece::King).count() != 1) {
            return false;
        }

        if (pieces(Side::Black, Piece::King).count() != 1) {
            return false;
        }

        if (colours_[0] & colours_[1]) {
            return false;
        }

        if (occupancy(Piece::Pawn) & (bitboards::Rank1 | bitboards::Rank8)) {
            return false;
        }

        for (int i = 0; i < 5; ++i) {
            for (int j = i + 1; j < 6; ++j) {
                if (pieces_[i] & pieces_[j]) {
                    return false;
                }
            }
        }

        if ((colours_[0] | colours_[1]) !=
            (pieces_[0] | pieces_[1] | pieces_[2] | pieces_[3] | pieces_[4] |
             pieces_[5])) {
            return false;
        }

        // Better not be able to capture the king
        if (square_attacked(king_position(!turn()), turn())) {
            return false;
        }

        return true;
    }

    struct meh {
        Move move;
        int ep;
        bool castling[4];
    };

    Bitboard colours_[2];
    Bitboard pieces_[6];
    int halfmove_clock_;
    int fullmove_clock_;
    std::uint64_t hash_;
    bool castling_[4];
    Side to_move_;
    std::vector<meh> history_;
};

inline std::ostream &operator<<(std::ostream &os,
                                const Position &pos) noexcept {
    int i = 56;
    while (i >= 0) {
        const auto sq = Square(i);
        const auto bb = Bitboard{sq};
        if (pos.pieces(Side::White, Piece::Pawn) & bb) {
            os << 'P';
        } else if (pos.pieces(Side::White, Piece::Knight) & bb) {
            os << 'N';
        } else if (pos.pieces(Side::White, Piece::Bishop) & bb) {
            os << 'B';
        } else if (pos.pieces(Side::White, Piece::Rook) & bb) {
            os << 'R';
        } else if (pos.pieces(Side::White, Piece::Queen) & bb) {
            os << 'Q';
        } else if (pos.pieces(Side::White, Piece::King) & bb) {
            os << 'K';
        } else if (pos.pieces(Side::Black, Piece::Pawn) & bb) {
            os << 'p';
        } else if (pos.pieces(Side::Black, Piece::Knight) & bb) {
            os << 'n';
        } else if (pos.pieces(Side::Black, Piece::Bishop) & bb) {
            os << 'b';
        } else if (pos.pieces(Side::Black, Piece::Rook) & bb) {
            os << 'r';
        } else if (pos.pieces(Side::Black, Piece::Queen) & bb) {
            os << 'q';
        } else if (pos.pieces(Side::Black, Piece::King) & bb) {
            os << 'k';
        } else {
            os << '-';
        }

        if (i % 8 == 7) {
            os << '\n';
            i -= 16;
        }

        i++;
    }

    os << "Castling: ";
    os << (pos.can_castle(Side::White, MoveType::ksc) ? "K" : "");
    os << (pos.can_castle(Side::White, MoveType::qsc) ? "Q" : "");
    os << (pos.can_castle(Side::Black, MoveType::ksc) ? "k" : "");
    os << (pos.can_castle(Side::Black, MoveType::qsc) ? "q" : "");
    os << '\n';
    os << "EP: " << pos.ep_ << '\n';
    os << "Turn: " << (pos.turn() == Side::White ? 'w' : 'b');

    return os;
}

}  // namespace libchess

#endif
