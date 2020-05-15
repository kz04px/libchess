#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] bool Position::is_legal(const Move &m) const noexcept {
    const auto moves = legal_moves();

    for (const auto &move : moves) {
        if (move == m) {
            return true;
        }
    }

    return false;
}

}  // namespace libchess
