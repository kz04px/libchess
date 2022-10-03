#include <cassert>
#include <sstream>
#include "libchess/position.hpp"

namespace libchess {

void Position::set_fen(const std::string &fen) noexcept {
    if (fen == "startpos") {
        set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        return;
    }

    clear();

    std::stringstream ss{fen};
    std::string word;

    ss >> word;
    int i = 56;
    for (const auto &c : word) {
        switch (c) {
            case 'P':
                set(Square(i), Side::White, Piece::Pawn);
                i++;
                break;
            case 'p':
                set(Square(i), Side::Black, Piece::Pawn);
                i++;
                break;
            case 'N':
                set(Square(i), Side::White, Piece::Knight);
                i++;
                break;
            case 'n':
                set(Square(i), Side::Black, Piece::Knight);
                i++;
                break;
            case 'B':
                set(Square(i), Side::White, Piece::Bishop);
                i++;
                break;
            case 'b':
                set(Square(i), Side::Black, Piece::Bishop);
                i++;
                break;
            case 'R':
                set(Square(i), Side::White, Piece::Rook);
                i++;
                break;
            case 'r':
                set(Square(i), Side::Black, Piece::Rook);
                i++;
                break;
            case 'Q':
                set(Square(i), Side::White, Piece::Queen);
                i++;
                break;
            case 'q':
                set(Square(i), Side::Black, Piece::Queen);
                i++;
                break;
            case 'K':
                set(Square(i), Side::White, Piece::King);
                i++;
                break;
            case 'k':
                set(Square(i), Side::Black, Piece::King);
                i++;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                i += c - '1' + 1;
                break;
            case '/':
                i -= 16;
                break;
            default:
                break;
        }
    }

    // Side to move
    ss >> word;
    if (word == "w") {
        to_move_ = Side::White;
    } else {
        to_move_ = Side::Black;
    }

    // Castling perms
    ss >> word;
    {
        auto get_square = [](const char &c) noexcept -> Square {
            if (c == 'K') {
                return squares::H1;
            } else if (c == 'Q') {
                return squares::A1;
            } else if (c == 'k') {
                return squares::H8;
            } else if (c == 'q') {
                return squares::A8;
            } else if ('A' <= c && c <= 'H') {
                return Square(c - 'A');
            } else if ('a' <= c && c <= 'h') {
                return Square(56 + c - 'a');
            } else {
                return squares::OffSq;
            }
        };

        auto is_uppercase = [](const char &c) noexcept -> bool {
            return 'A' <= c && c <= 'Z';
        };
        const auto wksq = king_position(Side::White);
        const auto bksq = king_position(Side::Black);
        for (const auto &c : word) {
            const auto perm_square = get_square(c);
            if (perm_square == squares::OffSq) {
                break;
            }

            const auto is_white = is_uppercase(c);
            const auto rooks =
                pieces(is_white ? Side::White : Side::Black, libchess::Rook) & bitboards::ranks[perm_square.rank()];
            const auto ksq = is_white ? wksq : bksq;
            const auto is_ksc = perm_square.file() > ksq.file();
            const auto rook_found = rooks.get(perm_square);
            const auto rook_east = rook_found ? perm_square : rooks.hsb();
            const auto rook_west = rook_found ? perm_square : rooks.lsb();

            if (is_white) {
                if (is_ksc) {
                    castling_[0] = true;
                    castle_rooks_from_[0] = rook_east;
                } else {
                    castling_[1] = true;
                    castle_rooks_from_[1] = rook_west;
                }
            } else {
                if (is_ksc) {
                    castling_[2] = true;
                    castle_rooks_from_[2] = rook_east;
                } else {
                    castling_[3] = true;
                    castle_rooks_from_[3] = rook_west;
                }
            }
        }
    }

    // En passant
    ss >> word;
    if (word != "-") {
        ep_ = Square(word);
    }

    // Halfmove clock
    ss >> halfmove_clock_;

    // Fullmove clock
    ss >> fullmove_clock_;

    // Calculate hash
#ifdef NO_HASH
    hash_ = 0;
#else
    hash_ = calculate_hash();
#endif

    assert(valid());
}

}  // namespace libchess
