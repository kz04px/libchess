#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::uint64_t Position::perft(const int depth) noexcept {
    if (depth == 0) {
        return 1;
    } else if (depth == 1) {
        return count_moves();
    }

    std::uint64_t nodes = 0;

    const auto moves = legal_moves();
    for (const auto &move : moves) {
        makemove(move);
        nodes += perft(depth - 1);
        undomove();
    }

    return nodes;
}

}  // namespace libchess
