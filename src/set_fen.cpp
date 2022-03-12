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
    for (const auto &c : word) {
        if (c == 'K') {
            castling_[0] = true;
        } else if (c == 'Q') {
            castling_[1] = true;
        } else if (c == 'k') {
            castling_[2] = true;
        } else if (c == 'q') {
            castling_[3] = true;
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
