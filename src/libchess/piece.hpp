#ifndef LIBCHESS_PIECE_HPP
#define LIBCHESS_PIECE_HPP

namespace libchess {

enum Piece : int
{
    Pawn = 0,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None,
};

}  // namespace libchess

#endif
