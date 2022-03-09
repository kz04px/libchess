#include "libchess/movegen.hpp"
#include <array>
#include <cassert>
#include <cstdint>

namespace libchess::movegen {

constexpr std::pair<std::uint64_t, int> bishop_stuff[64] = {
    {0x007fbfbfbfbfbfffULL, 5378},  {0x0000a060401007fcULL, 4093},  {0x0001004008020000ULL, 4314},
    {0x0000806004000000ULL, 6587},  {0x0000100400000000ULL, 6491},  {0x000021c100b20000ULL, 6330},
    {0x0000040041008000ULL, 5609},  {0x00000fb0203fff80ULL, 22236}, {0x0000040100401004ULL, 6106},
    {0x0000020080200802ULL, 5625},  {0x0000004010202000ULL, 16785}, {0x0000008060040000ULL, 16817},
    {0x0000004402000000ULL, 6842},  {0x0000000801008000ULL, 7003},  {0x000007efe0bfff80ULL, 4197},
    {0x0000000820820020ULL, 7356},  {0x0000400080808080ULL, 4602},  {0x00021f0100400808ULL, 4538},
    {0x00018000c06f3fffULL, 29531}, {0x0000258200801000ULL, 45393}, {0x0000240080840000ULL, 12420},
    {0x000018000c03fff8ULL, 15763}, {0x00000a5840208020ULL, 5050},  {0x0000020008208020ULL, 4346},
    {0x0000804000810100ULL, 6074},  {0x0001011900802008ULL, 7866},  {0x0000804000810100ULL, 32139},
    {0x000100403c0403ffULL, 57673}, {0x00078402a8802000ULL, 55365}, {0x0000101000804400ULL, 15818},
    {0x0000080800104100ULL, 5562},  {0x00004004c0082008ULL, 6390},  {0x0001010120008020ULL, 7930},
    {0x000080809a004010ULL, 13329}, {0x0007fefe08810010ULL, 7170},  {0x0003ff0f833fc080ULL, 27267},
    {0x007fe08019003042ULL, 53787}, {0x003fffefea003000ULL, 5097},  {0x0000101010002080ULL, 6643},
    {0x0000802005080804ULL, 6138},  {0x0000808080a80040ULL, 7418},  {0x0000104100200040ULL, 7898},
    {0x0003ffdf7f833fc0ULL, 42012}, {0x0000008840450020ULL, 57350}, {0x00007ffc80180030ULL, 22813},
    {0x007fffdd80140028ULL, 56693}, {0x00020080200a0004ULL, 5818},  {0x0000101010100020ULL, 7098},
    {0x0007ffdfc1805000ULL, 4451},  {0x0003ffefe0c02200ULL, 4709},  {0x0000000820806000ULL, 4794},
    {0x0000000008403000ULL, 13364}, {0x0000000100202000ULL, 4570},  {0x0000004040802000ULL, 4282},
    {0x0004010040100400ULL, 14964}, {0x00006020601803f4ULL, 4026},  {0x0003ffdfdfc28048ULL, 4826},
    {0x0000000820820020ULL, 7354},  {0x0000000008208060ULL, 4848},  {0x0000000000808020ULL, 15946},
    {0x0000000001002020ULL, 14932}, {0x0000000401002008ULL, 16588}, {0x0000004040404040ULL, 6905},
    {0x007fff9fdf7ff813ULL, 16076},
};

constexpr std::pair<std::uint64_t, int> rook_stuff[64] = {
    {0x00280077ffebfffeULL, 26304}, {0x2004010201097fffULL, 35520}, {0x0010020010053fffULL, 38592},
    {0x0040040008004002ULL, 8026},  {0x7fd00441ffffd003ULL, 22196}, {0x4020008887dffffeULL, 80870},
    {0x004000888847ffffULL, 76747}, {0x006800fbff75fffdULL, 30400}, {0x000028010113ffffULL, 11115},
    {0x0020040201fcffffULL, 18205}, {0x007fe80042ffffe8ULL, 53577}, {0x00001800217fffe8ULL, 62724},
    {0x00001800073fffe8ULL, 34282}, {0x00001800e05fffe8ULL, 29196}, {0x00001800602fffe8ULL, 23806},
    {0x000030002fffffa0ULL, 49481}, {0x00300018010bffffULL, 2410},  {0x0003000c0085fffbULL, 36498},
    {0x0004000802010008ULL, 24478}, {0x0004002020020004ULL, 10074}, {0x0001002002002001ULL, 79315},
    {0x0001001000801040ULL, 51779}, {0x0000004040008001ULL, 13586}, {0x0000006800cdfff4ULL, 19323},
    {0x0040200010080010ULL, 70612}, {0x0000080010040010ULL, 83652}, {0x0004010008020008ULL, 63110},
    {0x0000040020200200ULL, 34496}, {0x0002008010100100ULL, 84966}, {0x0000008020010020ULL, 54341},
    {0x0000008020200040ULL, 60421}, {0x0000820020004020ULL, 86402}, {0x00fffd1800300030ULL, 50245},
    {0x007fff7fbfd40020ULL, 76622}, {0x003fffbd00180018ULL, 84676}, {0x001fffde80180018ULL, 78757},
    {0x000fffe0bfe80018ULL, 37346}, {0x0001000080202001ULL, 370},   {0x0003fffbff980180ULL, 42182},
    {0x0001fffdff9000e0ULL, 45385}, {0x00fffefeebffd800ULL, 61659}, {0x007ffff7ffc01400ULL, 12790},
    {0x003fffbfe4ffe800ULL, 16762}, {0x001ffff01fc03000ULL, 0},     {0x000fffe7f8bfe800ULL, 38380},
    {0x0007ffdfdf3ff808ULL, 11098}, {0x0003fff85fffa804ULL, 21803}, {0x0001fffd75ffa802ULL, 39189},
    {0x00ffffd7ffebffd8ULL, 58628}, {0x007fff75ff7fbfd8ULL, 44116}, {0x003fff863fbf7fd8ULL, 78357},
    {0x001fffbfdfd7ffd8ULL, 44481}, {0x000ffff810280028ULL, 64134}, {0x0007ffd7f7feffd8ULL, 41759},
    {0x0003fffc0c480048ULL, 1394},  {0x0001ffffafd7ffd8ULL, 40910}, {0x00ffffe4ffdfa3baULL, 66516},
    {0x007fffef7ff3d3daULL, 3897},  {0x003fffbfdfeff7faULL, 3930},  {0x001fffeff7fbfc22ULL, 72934},
    {0x0000020408001001ULL, 72662}, {0x0007fffeffff77fdULL, 56325}, {0x0003ffffbf7dfeecULL, 66501},
    {0x0001ffff9dffa333ULL, 14826},
};

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

constexpr Bitboard calculate_bishop_moves(const Square sq, const Bitboard blockers) {
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

constexpr Bitboard calculate_rook_moves(const Square sq, const Bitboard blockers) {
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

std::array<std::uint64_t, 88772> generate_magic_moves() {
    std::array<std::uint64_t, 88772> result = {};

    for (int i = 0; i < 64; ++i) {
        assert(bishop_masks[i]);
        assert(rook_masks[i]);

        Bitboard perm;
        const auto sq = Square{i};

        // Bishops
        perm.clear();
        do {
            std::uint64_t *index =
                (std::uint64_t *)(result.data() + bishop_stuff[i].second +
                                  (((perm & bishop_masks[i]).value() * bishop_stuff[i].first) >> 55));
            *index = calculate_bishop_moves(sq, perm).value();
        } while ((perm = permute(bishop_masks[i], perm)));

        // Rooks
        perm.clear();
        do {
            std::uint64_t *index = (std::uint64_t *)(result.data() + rook_stuff[i].second +
                                                     (((perm & rook_masks[i]).value() * rook_stuff[i].first) >> 52));
            *index = calculate_rook_moves(sq, perm).value();
        } while ((perm = permute(rook_masks[i], perm)));
    }

    return result;
}

const auto magic_moves = generate_magic_moves();

Bitboard knight_moves(const Square sq) {
    return knight_masks[static_cast<int>(sq)];
}

Bitboard bishop_moves(const Square sq, const Bitboard &occ) {
    const int idx = static_cast<int>(sq);
    return Bitboard(*(magic_moves.data() + bishop_stuff[idx].second +
                      (((occ & bishop_masks[idx]).value() * bishop_stuff[idx].first) >> 55)));
}

Bitboard rook_moves(const Square sq, const Bitboard &occ) {
    const int idx = static_cast<int>(sq);
    return Bitboard(*(magic_moves.data() + rook_stuff[idx].second +
                      (((occ & rook_masks[idx]).value() * rook_stuff[idx].first) >> 52)));
}

Bitboard queen_moves(const Square sq, const Bitboard &occ) {
    return bishop_moves(sq, occ) | rook_moves(sq, occ);
}

Bitboard king_moves(const Square sq) {
    return king_masks[static_cast<int>(sq)];
}

}  // namespace libchess::movegen
