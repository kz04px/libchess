#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::string fen_pieces(const Position &pos) noexcept {
    const char piece_chars[2][6] = {
        {'P', 'N', 'B', 'R', 'Q', 'K'},
        {'p', 'n', 'b', 'r', 'q', 'k'},
    };

    std::string fen;

    for (int y = 7; y >= 0; --y) {
        int num_empty = 0;

        for (int x = 0; x < 8; ++x) {
            const auto sq = Square{x, y};
            const auto bb = Bitboard{sq};
            const auto piece = pos.piece_on(sq);

            if (piece == Piece::None) {
                num_empty++;
            } else {
                // Add the number of empty squares so far
                if (num_empty > 0) {
                    fen += std::to_string(num_empty);
                }
                num_empty = 0;

                const auto gg = static_cast<bool>(pos.occupancy(Side::Black) & bb);
                fen += piece_chars[gg][piece];
            }
        }

        // Add the number of empty squares when we reach the end of the rank
        if (num_empty > 0) {
            fen += std::to_string(num_empty);
        }

        if (y > 0) {
            fen += "/";
        }
    }

    return fen;
}

[[nodiscard]] std::string fen_side(const Position &pos) noexcept {
    return pos.turn() == Side::White ? "w" : "b";
}

[[nodiscard]] std::string fen_castling(const Position &pos) noexcept {
    std::string part;
    if (pos.can_castle(Side::White, MoveType::ksc)) {
        part += "K";
    }
    if (pos.can_castle(Side::White, MoveType::qsc)) {
        part += "Q";
    }
    if (pos.can_castle(Side::Black, MoveType::ksc)) {
        part += "k";
    }
    if (pos.can_castle(Side::Black, MoveType::qsc)) {
        part += "q";
    }
    if (part.empty()) {
        part = "-";
    }
    return part;
}

[[nodiscard]] std::string fen_enpassant(const Position &pos) noexcept {
    if (pos.ep() == squares::OffSq) {
        return "-";
    } else {
        return square_strings[static_cast<int>(pos.ep())];
    }
}

[[nodiscard]] std::string fen_halfmoves(const Position &pos) noexcept {
    return std::to_string(pos.halfmoves());
}

[[nodiscard]] std::string fen_fullmoves(const Position &pos) noexcept {
    return std::to_string(pos.fullmoves());
}

[[nodiscard]] std::string Position::get_fen() const noexcept {
    std::string fen = fen_pieces(*this);
    fen += " " + fen_side(*this);
    fen += " " + fen_castling(*this);
    fen += " " + fen_enpassant(*this);
    fen += " " + fen_halfmoves(*this);
    fen += " " + fen_fullmoves(*this);
    return fen;
}

}  // namespace libchess
