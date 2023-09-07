#include <array>
#include <libchess/position.hpp>
#include <string>
#include <tuple>
#include "catch.hpp"

TEST_CASE("Position::get_fen()") {
    const std::array<std::string, 8> fens = {{
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w k - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen};
        REQUIRE(pos.get_fen() == fen);
    }
}

TEST_CASE("Position::get_fen() DFRC") {
    const std::array<std::string, 11> fens = {{
        "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8 11",
        "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0 13",
        "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b HAha e3 0 3",
        "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFfd - 0 1",
        "rbknbrnq/pppppppp/8/8/8/8/PPPPPPPP/BQNBRNKR w HEfa - 0 1",
        "qrkbnnbr/pppppppp/8/8/8/8/PPPPPPPP/BBNRKQRN w GDhb - 0 1",
        "rknqbbrn/pppppppp/8/8/8/8/PPPPPPPP/RNBKNBQR w HAga - 0 1",
        "brkqnnrb/pppppppp/8/8/8/8/PPPPPPPP/BRQKNBNR w HBgb - 0 1",
        "nnqrbkrb/pppppppp/8/8/8/8/PPPPPPPP/RBKNQRBN w FAgd - 0 1",
        "qrnkbbnr/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRNQ w FAhb - 0 1",
        "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/RQKRBBNN w DAhf - 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen, true};
        REQUIRE(pos.get_fen(true) == fen);
    }
}

TEST_CASE("FEN - DFRC handling KQkq") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 6> fens = {{
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w HAha - 0 1"},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1"},
        {"r5kr/8/8/8/8/8/8/R5KR w KQkq - 0 1", "r5kr/8/8/8/8/8/8/R5KR w HAha - 0 1"},
        {"rk5r/8/8/8/8/8/8/RK5R w KQkq - 0 1", "rk5r/8/8/8/8/8/8/RK5R w HAha - 0 1"},
        {"1r2k1r1/8/8/8/8/8/8/1R2K1R1 w KQkq - 0 1", "1r2k1r1/8/8/8/8/8/8/1R2K1R1 w GBgb - 0 1"},
        {"2r1kr2/8/8/8/8/8/8/2R1KR2 w KQkq - 0 1", "2r1kr2/8/8/8/8/8/8/2R1KR2 w FCfc - 0 1"},
    }};

    for (const auto &[fen, expected] : fens) {
        INFO(fen);

        auto pos = libchess::Position{fen, true};
        CHECK(pos.get_fen(true) == expected);
    }
}

TEST_CASE("Counters") {
    using tuple_type = std::tuple<std::string, int, int>;

    const std::array<tuple_type, 15> moves = {{
        {"e2e4", 0, 1},
        {"c7c5", 0, 2},
        {"g1f3", 1, 2},
        {"d7d6", 0, 3},
        {"d2d4", 0, 3},
        {"c5d4", 0, 4},
        {"f3d4", 0, 4},
        {"g8f6", 1, 5},
        {"b1c3", 2, 5},
        {"g7g6", 0, 6},
        {"c1e3", 1, 6},
        {"f8g7", 2, 7},
        {"f1e2", 3, 7},
        {"e8g8", 4, 8},
        {"e1g1", 5, 8},
    }};

    auto pos = libchess::Position{"startpos"};
    for (const auto &[movestr, half, full] : moves) {
        INFO(movestr);
        const auto move = pos.parse_move(movestr);
        pos.makemove(move);
        REQUIRE(pos.halfmoves() == half);
        REQUIRE(pos.fullmoves() == full);
    }
}

TEST_CASE("FEN - 960") {
    const std::array<std::string, 14> fens = {{
        "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFhf - 0 1",
        "bnnbrqkr/pppppppp/8/8/8/8/PPPPPPPP/BNNBRQKR w HEhe - 0 1",
        "bqrknbnr/pppppppp/8/8/8/8/PPPPPPPP/BQRKNBNR w HChc - 0 1",
        "rknbbrqn/pppppppp/8/8/8/8/PPPPPPPP/RKNBBRQN w FAfa - 0 1",
        "rkbrnbnq/pppppppp/8/8/8/8/PPPPPPPP/RKBRNBNQ w DAda - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w H - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w A - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w h - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w a - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w HA - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w ha - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w Hh - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w Aa - 0 1",
    }};

    for (const auto &fen : fens) {
        INFO(fen);

        auto pos = libchess::Position{fen, true};
        CHECK(pos.get_fen(true) == fen);

        pos.set_fen(fen, true);
        CHECK(pos.get_fen(true) == fen);
    }
}

TEST_CASE("FEN - Illegal castling") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 5> tests = {{
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"},
        {"4k2r/8/8/8/8/8/8/4K2R w KQkq - 0 1", "4k2r/8/8/8/8/8/8/4K2R w Kk - 0 1"},
        {"r3k3/8/8/8/8/8/8/R3K3 w KQkq - 0 1", "r3k3/8/8/8/8/8/8/R3K3 w Qq - 0 1"},
        {"4k3/8/8/8/8/8/8/4K3 w KQkq - 0 1", "4k3/8/8/8/8/8/8/4K3 w - - 0 1"},
        {"1b1r2r1/p1pppkp1/5p1p/3n4/1P3P2/2p5/P7/B1N1K2N w KQ - 0 17",
         "1b1r2r1/p1pppkp1/5p1p/3n4/1P3P2/2p5/P7/B1N1K2N w - - 0 17"},
    }};

    for (const auto &[fen, corrected] : tests) {
        INFO(fen);
        auto pos = libchess::Position{fen};
        CHECK(pos.get_fen() == corrected);
    }
}

TEST_CASE("FEN - DFRC Illegal castling") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 4> tests = {{
        {"r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1", "r3k2r/8/8/8/8/8/8/R3K2R w HAha - 0 1"},
        {"4k2r/8/8/8/8/8/8/4K2R w HAha - 0 1", "4k2r/8/8/8/8/8/8/4K2R w Hh - 0 1"},
        {"r3k3/8/8/8/8/8/8/R3K3 w HAha - 0 1", "r3k3/8/8/8/8/8/8/R3K3 w Aa - 0 1"},
        {"4k3/8/8/8/8/8/8/4K3 w HAha - 0 1", "4k3/8/8/8/8/8/8/4K3 w - - 0 1"},
    }};

    for (const auto &[fen, corrected] : tests) {
        INFO(fen);
        auto pos = libchess::Position{fen, true};
        CHECK(pos.get_fen(true) == corrected);
    }
}
