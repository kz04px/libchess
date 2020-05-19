#include "libchess/movegen.hpp"
#include <array>
#include <cassert>
#include <cstdint>

namespace libchess::movegen {

// clang-format off
constexpr std::uint64_t bishop_magic[64] = {
    0x404040404040ULL,   0xa060401007fcULL,  0x401020200000ULL,
    0x806004000000ULL,   0x440200000000ULL,  0x80100800000ULL,
    0x104104004000ULL,   0x20020820080ULL,   0x40100202004ULL,
    0x20080200802ULL,    0x10040080200ULL,   0x8060040000ULL,
    0x4402000000ULL,     0x21c100b200ULL,    0x400410080ULL,
    0x3f7f05fffc0ULL,    0x4228040808010ULL, 0x200040404040ULL,
    0x400080808080ULL,   0x200200801000ULL,  0x240080840000ULL,
    0x18000c03fff8ULL,   0xa5840208020ULL,   0x58408404010ULL,
    0x2022000408020ULL,  0x402000408080ULL,  0x804000810100ULL,
    0x100403c0403ffULL,  0x78402a8802000ULL, 0x101000804400ULL,
    0x80800104100ULL,    0x400480101008ULL,  0x1010102004040ULL,
    0x808090402020ULL,   0x7fefe08810010ULL, 0x3ff0f833fc080ULL,
    0x7fe08019003042ULL, 0x202040008040ULL,  0x1004008381008ULL,
    0x802003700808ULL,   0x208200400080ULL,  0x104100200040ULL,
    0x3ffdf7f833fc0ULL,  0x8840450020ULL,    0x20040100100ULL,
    0x7fffdd80140028ULL, 0x202020200040ULL,  0x1004010039004ULL,
    0x40041008000ULL,    0x3ffefe0c02200ULL, 0x1010806000ULL,
    0x08403000ULL,       0x100202000ULL,     0x40100200800ULL,
    0x404040404000ULL,   0x6020601803f4ULL,  0x3ffdfdfc28048ULL,
    0x820820020ULL,      0x10108060ULL,      0x00084030ULL,
    0x01002020ULL,       0x40408020ULL,      0x4040404040ULL,
    0x404040404040ULL,
};

constexpr std::uint64_t bishop_offsets[64] = {
    33104, 4094,  24764,
    13882, 23090, 32640,
    11558, 32912, 13674,
    6109,  26494, 17919,
    25757, 17338, 16983,
    16659, 13610, 2224,
    60405, 7983,  17,
    34321, 33216, 17127,
    6397,  22169, 42727,
    155,   8601,  21101,
    29885, 29340, 19785,
    12258, 50451, 1712,
    78475, 7855,  13642,
    8156,  4348,  28794,
    22578, 50315, 85452,
    32816, 13930, 17967,
    33200, 32456, 7762,
    7794,  22761, 14918,
    11620, 15925, 32528,
    12196, 32720, 26781,
    19817, 24732, 25468,
    10186,
};

constexpr std::uint64_t rook_magic[64] = {
    0x280077ffebfffeULL, 0x2004010201097fffULL, 0x10020010053fffULL,
    0x30002ff71ffffaULL, 0x7fd00441ffffd003ULL, 0x4001d9e03ffff7ULL,
    0x4000888847ffffULL, 0x6800fbff75fffdULL,   0x28010113ffffULL,
    0x20040201fcffffULL, 0x7fe80042ffffe8ULL,   0x1800217fffe8ULL,
    0x1800073fffe8ULL,   0x7fe8009effffe8ULL,   0x1800602fffe8ULL,
    0x30002fffffa0ULL,   0x300018010bffffULL,   0x3000c0085fffbULL,
    0x4000802010008ULL,  0x2002004002002ULL,    0x2002020010002ULL,
    0x1002020008001ULL,  0x4040008001ULL,       0x802000200040ULL,
    0x40200010080010ULL, 0x80010040010ULL,      0x4010008020008ULL,
    0x40020200200ULL,    0x10020020020ULL,      0x10020200080ULL,
    0x8020200040ULL,     0x200020004081ULL,     0xfffd1800300030ULL,
    0x7fff7fbfd40020ULL, 0x3fffbd00180018ULL,   0x1fffde80180018ULL,
    0xfffe0bfe80018ULL,  0x1000080202001ULL,    0x3fffbff980180ULL,
    0x1fffdff9000e0ULL,  0xfffeebfeffd800ULL,   0x7ffff7ffc01400ULL,
    0x408104200204ULL,   0x1ffff01fc03000ULL,   0xfffe7f8bfe800ULL,
    0x8001002020ULL,     0x3fff85fffa804ULL,    0x1fffd75ffa802ULL,
    0xffffec00280028ULL, 0x7fff75ff7fbfd8ULL,   0x3fff863fbf7fd8ULL,
    0x1fffbfdfd7ffd8ULL, 0xffff810280028ULL,    0x7ffd7f7feffd8ULL,
    0x3fffc0c480048ULL,  0x1ffffafd7ffd8ULL,    0xffffe4ffdfa3baULL,
    0x7fffef7ff3d3daULL, 0x3fffbfdfeff7faULL,   0x1fffeff7fbfc22ULL,
    0x20408001001ULL,    0x7fffeffff77fdULL,    0x3ffffbf7dfeecULL,
    0x1ffff9dffa333ULL,
};

constexpr std::uint64_t rook_offsets[64] = {
    41305, 14326, 24477,
    8223,  49795, 60546,
    28543, 79282, 6457,
    4125,  81021, 42341,
    14139, 19465, 9514,
    71090, 75419, 33476,
    27117, 85964, 54915,
    36544, 71854, 37996,
    30398, 55939, 53891,
    56963, 77451, 12319,
    88500, 51405, 72878,
    676,   83122, 22206,
    75186, 681,   36453,
    20369, 1981,  13343,
    10650, 57987, 26302,
    58357, 40546, 0,
    14967, 80361, 40905,
    58347, 20381, 81868,
    59381, 84404, 45811,
    62898, 45796, 66994,
    67204, 32448, 62946,
    17005,
};
// clang-format on

[[nodiscard]] constexpr std::array<Bitboard, 64> calculate_knight_masks() {
    std::array<Bitboard, 64> result = {};
    for (int i = 0; i < 64; ++i) {
        const auto sq = Square{i};
        const auto bb = Bitboard{sq};

        result[i] = bb.north().north().east();
        result[i] |= bb.north().north().west();
        result[i] |= bb.south().south().east();
        result[i] |= bb.south().south().west();
        result[i] |= bb.east().east().north();
        result[i] |= bb.east().east().south();
        result[i] |= bb.west().west().north();
        result[i] |= bb.west().west().south();

        assert(result[i]);
    }
    return result;
}

[[nodiscard]] constexpr std::array<Bitboard, 64> calculate_bishop_masks() {
    std::array<Bitboard, 64> result = {};
    for (int i = 0; i < 64; ++i) {
        const auto sq = Square{i};
        const int file = sq.file();
        const int rank = sq.rank();

        // Up 1 Right 1
        for (int y = rank + 1, x = file + 1; y <= 6 && x <= 6; ++y, ++x) {
            const auto nsq = Square{x + y * 8};
            result[i] |= Bitboard{nsq};
        }

        // Up 1 Left 1
        for (int y = rank + 1, x = file - 1; y <= 6 && x >= 1; ++y, --x) {
            const auto nsq = Square{x + y * 8};
            result[i] |= Bitboard{nsq};
        }

        // Down 1 Right 1
        for (int y = rank - 1, x = file + 1; y >= 1 && x <= 6; --y, ++x) {
            const auto nsq = Square{x + y * 8};
            result[i] |= Bitboard{nsq};
        }

        // Down 1 Left 1
        for (int y = rank - 1, x = file - 1; y >= 1 && x >= 1; --y, --x) {
            const auto nsq = Square{x + y * 8};
            result[i] |= Bitboard{nsq};
        }

        assert(result[i]);
    }
    return result;
}

[[nodiscard]] constexpr std::array<Bitboard, 64> generate_rook_masks() {
    std::array<Bitboard, 64> result = {};
    for (int i = 0; i < 64; ++i) {
        const auto sq = Square{i};
        const int file = sq.file();
        const int rank = sq.rank();

        // Right
        for (int r = rank + 1; r <= 6; ++r) {
            const auto nsq = Square{file + r * 8};
            result[i] |= Bitboard{nsq};
        }

        // Left
        for (int r = rank - 1; r >= 1; --r) {
            const auto nsq = Square{file + r * 8};
            result[i] |= Bitboard{nsq};
        }

        // Up
        for (int r = file + 1; r <= 6; ++r) {
            const auto nsq = Square{r + rank * 8};
            result[i] |= Bitboard{nsq};
        }

        // Down
        for (int r = file - 1; r >= 1; --r) {
            const auto nsq = Square{r + rank * 8};
            result[i] |= Bitboard{nsq};
        }

        assert(result[i]);
    }
    return result;
}

[[nodiscard]] constexpr std::array<Bitboard, 64> calculate_king_masks() {
    std::array<Bitboard, 64> result = {};
    for (int i = 0; i < 64; ++i) {
        const auto sq = Square{i};
        const auto bb = Bitboard{sq};
        result[i] = bb.adjacent();

        assert(result[i]);
    }
    return result;
}

constexpr Bitboard permute(const Bitboard set, const Bitboard subset) {
    return Bitboard{subset.value() - set.value()} & set;
}

constexpr Bitboard calculate_bishop_moves(const Square sq,
                                          const Bitboard blockers) {
    Bitboard result;
    const int file = sq.file();
    const int rank = sq.rank();

    // Up 1 Right 1
    for (int y = rank + 1, x = file + 1; y <= 7 && x <= 7; ++y, ++x) {
        const auto nsq = Square{x + y * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Up 1 Left 1
    for (int y = rank + 1, x = file - 1; y <= 7 && x >= 0; ++y, --x) {
        const auto nsq = Square{x + y * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Down 1 Right 1
    for (int y = rank - 1, x = file + 1; y >= 0 && x <= 7; --y, ++x) {
        const auto nsq = Square{x + y * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Down 1 Left 1
    for (int y = rank - 1, x = file - 1; y >= 0 && x >= 0; --y, --x) {
        const auto nsq = Square{x + y * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    return result;
}

constexpr Bitboard calculate_rook_moves(const Square sq,
                                        const Bitboard blockers) {
    Bitboard result;
    const int file = sq.file();
    const int rank = sq.rank();

    // Up
    for (int i = rank + 1; i <= 7; ++i) {
        const auto nsq = Square{file + i * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Down
    for (int i = rank - 1; i >= 0; --i) {
        const auto nsq = Square{file + i * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Right
    for (int i = file + 1; i <= 7; ++i) {
        const auto nsq = Square{i + rank * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    // Left
    for (int i = file - 1; i >= 0; --i) {
        const auto nsq = Square{i + rank * 8};
        const auto bb = Bitboard{nsq};
        result |= bb;
        if (blockers & bb) {
            break;
        }
    }

    return result;
}

constexpr auto knight_masks = calculate_knight_masks();
constexpr auto bishop_masks = calculate_bishop_masks();
constexpr auto rook_masks = generate_rook_masks();
constexpr auto king_masks = calculate_king_masks();

std::array<std::uint64_t, 89524> generate_magic_moves() {
    std::array<std::uint64_t, 89524> result = {};

    for (int i = 0; i < 64; ++i) {
        assert(bishop_masks[i]);
        assert(rook_masks[i]);

        Bitboard perm;
        const auto sq = Square{i};

        // Bishops
        perm = 0;
        do {
            std::uint64_t *index =
                (std::uint64_t *)(result.data() + bishop_offsets[i] +
                                  (((perm & bishop_masks[i]).value() *
                                    bishop_magic[i]) >>
                                   55));
            *index = calculate_bishop_moves(sq, perm).value();
        } while ((perm = permute(bishop_masks[i], perm)));

        // Rooks
        perm = 0;
        do {
            std::uint64_t *index =
                (std::uint64_t *)(result.data() + rook_offsets[i] +
                                  (((perm & rook_masks[i]).value() *
                                    rook_magic[i]) >>
                                   52));
            *index = calculate_rook_moves(sq, perm).value();
        } while ((perm = permute(rook_masks[i], perm)));
    }

    return result;
}

const auto magic_moves = generate_magic_moves();

Bitboard knight_moves(const Square sq, const Bitboard &occ) {
    return knight_masks[static_cast<int>(sq)] & ~occ;
}

Bitboard bishop_moves(const Square sq, const Bitboard &occ) {
    const int idx = static_cast<int>(sq);
    return *(magic_moves.data() + bishop_offsets[idx] +
             (((occ & bishop_masks[idx]).value() * bishop_magic[idx]) >> 55));
}

Bitboard rook_moves(const Square sq, const Bitboard &occ) {
    const int idx = static_cast<int>(sq);
    return *(magic_moves.data() + rook_offsets[idx] +
             (((occ & rook_masks[idx]).value() * rook_magic[idx]) >> 52));
}

Bitboard queen_moves(const Square sq, const Bitboard &occ) {
    return bishop_moves(sq, occ) | rook_moves(sq, occ);
}

Bitboard king_moves(const Square sq, const Bitboard &occ) {
    return king_masks[static_cast<int>(sq)] & ~occ;
}

}  // namespace libchess::movegen
