#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::string Position::get_fen() const noexcept {
    std::string fen;

    // Part 1 -- Piece locations
    int empty = 0;
    for (int i = static_cast<int>(squares::A8); i >= 0; ++i) {
        const auto sq = Square{i};
        const auto bb = Bitboard{sq};

        if (pieces_[Piece::Pawn] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "P";
            } else {
                fen += "p";
            }
        } else if (pieces_[Piece::Knight] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "N";
            } else {
                fen += "n";
            }
        } else if (pieces_[Piece::Bishop] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "B";
            } else {
                fen += "b";
            }
        } else if (pieces_[Piece::Rook] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "R";
            } else {
                fen += "r";
            }
        } else if (pieces_[Piece::Queen] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "Q";
            } else {
                fen += "q";
            }
        } else if (pieces_[Piece::King] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (colours_[Side::White] & bb) {
                fen += "K";
            } else {
                fen += "k";
            }
        } else {
            empty++;
        }

        if (sq.file() == 7) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (i > static_cast<int>(squares::H1)) {
                fen += "/";
            }
            i -= 16;
        }
    }

    // Part 2 -- Side to move
    if (turn() == Side::White) {
        fen += " w";
    } else {
        fen += " b";
    }

    // Part 3 -- Castling permissions
    std::string part;
    if (castling_[usKSC] == true) {
        part += "K";
    }
    if (castling_[usQSC] == true) {
        part += "Q";
    }
    if (castling_[themKSC] == true) {
        part += "k";
    }
    if (castling_[themQSC] == true) {
        part += "q";
    }
    if (part.empty()) {
        part = "-";
    }
    fen += " " + part;

    // Part 4 -- En passant square
    if (ep_ == 0xFF) {
        fen += " -";
    } else {
        if (turn() == Side::White) {
            const auto bb = bitboards::Rank6 & bitboards::files[ep_];
            const auto sq = bb.lsb();
            fen += " " + square_strings[static_cast<int>(sq)];
        } else {
            const auto bb = bitboards::Rank3 & bitboards::files[ep_];
            const auto sq = bb.lsb();
            fen += " " + square_strings[static_cast<int>(sq)];
        }
    }

    // Part 5 -- Halfmove clock
    fen += " " + std::to_string(halfmove_clock_);

    // Part 6 -- Fullmove number
    fen += " " + std::to_string(fullmove_clock_);

    return fen;
}

}  // namespace libchess
