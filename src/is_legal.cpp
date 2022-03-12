#include <algorithm>
#include <ranges>
#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] bool Position::is_legal(const Move &m) const noexcept {
    const auto moves = legal_moves();
    return std::ranges::any_of(moves, [m](const auto &move) {
        return move == m;
    });
}

}  // namespace libchess
