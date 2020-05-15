#include "libchess/position.hpp"

namespace libchess {

[[nodiscard]] std::size_t Position::count_moves() const noexcept {
    return legal_moves().size();
}

}  // namespace libchess
