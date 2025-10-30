#include <chrono>
#include <iostream>
#include <libchess/position.hpp>
#include "tt.hpp"

struct TTEntry {
    std::uint64_t hash;
    std::uint64_t nodes;
    std::uint8_t depth;
};

[[nodiscard]] std::uint64_t ttperft(TT<TTEntry> &tt, libchess::Position &pos, const std::uint8_t depth) {
    if (depth == 0) {
        return 1;
    } else if (depth == 1) {
        return pos.count_moves();
    }

    // Poll TT
    const auto &entry = tt.poll(pos.hash());
    if (pos.hash() == entry.hash && entry.depth == depth) {
        return entry.nodes;
    }

    std::uint64_t nodes = 0;

    const auto moves = pos.legal_moves();
    for (const auto &move : moves) {
        pos.makemove(move);
        nodes += ttperft(tt, pos, depth - 1);
        pos.undomove();
    }

    // Create TT entry
    tt.add(pos.hash(), {pos.hash(), nodes, depth});

    return nodes;
}

int main(int argc, char **argv) {
    int depth = 6;
    std::string fen;

    if (argc > 1) {
        depth = std::stoi(std::string(argv[1]));
        depth = std::max(depth, 1);
    }

    if (argc > 2) {
        for (int i = 2; i < argc; ++i) {
            if (fen.empty()) {
                fen = std::string(argv[i]);
            } else {
                fen += " " + std::string(argv[i]);
            }
        }
    } else {
        fen = "startpos";
    }

    TT<TTEntry> tt{256};
    auto pos = libchess::Position(fen);

    std::cout << pos << std::endl;
    std::cout << std::endl;

    for (int i = 0; i <= depth; ++i) {
        const auto t0 = std::chrono::high_resolution_clock::now();
        const auto nodes = ttperft(tt, pos, i);
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        std::cout << "depth " << i;
        std::cout << " time " << dt.count();
        std::cout << " nodes " << nodes;
        if (dt.count() > 0) {
            const std::uint64_t nps = nodes / dt.count() * 1000;
            std::cout << " nps " << nps;
        }
        std::cout << std::endl;
    }

    return 0;
}
