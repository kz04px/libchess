#include <array>
#include <libchess/position.hpp>
#include <string>
#include <tuple>
#include "catch.hpp"

TEST_CASE("Position::get_fen()") {
    const std::array<std::string, 19> fens = {{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Q - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w k - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1",
                                               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1",
                                               "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8 11",
                                               "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0 13",
                                               "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 3",
                                               "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQkq - 0 1",
                                               "rbknbrnq/pppppppp/8/8/8/8/PPPPPPPP/BQNBRNKR w KQkq - 0 1",
                                               "qrkbnnbr/pppppppp/8/8/8/8/PPPPPPPP/BBNRKQRN w KQkq - 0 1",
                                               "rknqbbrn/pppppppp/8/8/8/8/PPPPPPPP/RNBKNBQR w KQkq - 0 1",
                                               "brkqnnrb/pppppppp/8/8/8/8/PPPPPPPP/BRQKNBNR w KQkq - 0 1",
                                               "nnqrbkrb/pppppppp/8/8/8/8/PPPPPPPP/RBKNQRBN w KQkq - 0 1",
                                               "qrnkbbnr/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRNQ w KQkq - 0 1",
                                               "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/RQKRBBNN w KQkq - 0 1"}};

    for (const auto &fen : fens) {
        INFO(fen);
        const libchess::Position pos{fen};
        REQUIRE(pos.get_fen() == fen);
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

TEST_CASE("FEN - 960"){
    using pair_type = std::pair<std::string, std::string>;

        const std::array<pair_type, 5> tests = {{
            {"bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQkq - 0 1",
             "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFhf - 0 1"},
            {"bnnbrqkr/pppppppp/8/8/8/8/PPPPPPPP/BNNBRQKR w KQkq - 0 1",
             "bnnbrqkr/pppppppp/8/8/8/8/PPPPPPPP/BNNBRQKR w HEhe - 0 1"},
            {"bqrknbnr/pppppppp/8/8/8/8/PPPPPPPP/BQRKNBNR w KQkq - 0 1",
             "bqrknbnr/pppppppp/8/8/8/8/PPPPPPPP/BQRKNBNR w HChc - 0 1"},
            {"rknbbrqn/pppppppp/8/8/8/8/PPPPPPPP/RKNBBRQN w KQkq - 0 1",
             "rknbbrqn/pppppppp/8/8/8/8/PPPPPPPP/RKNBBRQN w FAfa - 0 1"},
            {"rkbrnbnq/pppppppp/8/8/8/8/PPPPPPPP/RKBRNBNQ w KQkq - 0 1",
             "rkbrnbnq/pppppppp/8/8/8/8/PPPPPPPP/RKBRNBNQ w DAda - 0 1"},
        }};

        for (const auto &[fen_out, fen_in] : tests) {
            INFO(fen_in);
            auto pos = libchess::Position{fen_in};
            CHECK(pos.get_fen() == fen_out);
            pos.set_fen(fen_in);
            CHECK(pos.get_fen() == fen_out);
        }
}
