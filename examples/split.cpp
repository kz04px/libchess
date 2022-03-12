#include <chrono>
#include <iostream>
#include <libchess/position.hpp>

int main(int argc, char **argv) {
    int depth = 1;
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

    auto pos = libchess::Position(fen);

    std::cout << pos << std::endl;
    std::cout << std::endl;

    const auto moves = pos.legal_moves();
    std::uint64_t sum = 0;
    int idx = 1;

    const auto t0 = std::chrono::high_resolution_clock::now();
    for (const auto &move : moves) {
        std::cout << idx << " ";
        std::cout << move << ": ";

        pos.makemove(move);
        const auto nodes = pos.perft(depth - 1);
        pos.undomove();

        sum += nodes;
        std::cout << nodes << "\n";
        idx++;
    }
    const auto t1 = std::chrono::high_resolution_clock::now();
    const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "\n";
    std::cout << "Time " << dt.count() << "ms\n";
    std::cout << "Nodes " << sum << "\n";
    if (dt.count() > 0) {
        const auto nps = (sum * 1000) / dt.count();
        std::cout << "NPS: " << nps << "\n";
    }

    return 0;
}
