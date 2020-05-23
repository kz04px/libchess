cmake_minimum_required(VERSION 3.12)

# Project
project(Libchess VERSION 1.0 LANGUAGES CXX)

# Base directory relative includes
include_directories(.)
include_directories(./src/)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-DNO_HASH -Wall -pedantic -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wodr -Wpacked -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the static library
add_library(
    libchess-static
    STATIC
    src/attackers.cpp
    src/checkers.cpp
    src/check_evasions.cpp
    src/count_moves.cpp
    src/get_fen.cpp
    src/is_legal.cpp
    src/king_allowed.cpp
    src/legal_moves.cpp
    src/makemove.cpp
    src/movegen.cpp
    src/pawn_moves_black.cpp
    src/pawn_moves_white.cpp
    src/perft.cpp
    src/pinned.cpp
    src/promotions_black.cpp
    src/promotions_white.cpp
    src/set_fen.cpp
    src/square_attacked.cpp
    src/squares_attacked.cpp
    src/undomove.cpp
)

# Add the shared library
add_library(
    libchess-shared
    SHARED
    src/attackers.cpp
    src/checkers.cpp
    src/check_evasions.cpp
    src/count_moves.cpp
    src/get_fen.cpp
    src/is_legal.cpp
    src/king_allowed.cpp
    src/legal_moves.cpp
    src/makemove.cpp
    src/movegen.cpp
    src/pawn_moves_black.cpp
    src/pawn_moves_white.cpp
    src/perft.cpp
    src/pinned.cpp
    src/promotions_black.cpp
    src/promotions_white.cpp
    src/set_fen.cpp
    src/square_attacked.cpp
    src/squares_attacked.cpp
    src/undomove.cpp
)

# Add the test executable
add_executable(
    libchess-test
    tests/main.cpp
    tests/checkers.cpp
    tests/consistency.cpp
    tests/fen.cpp
    tests/hash.cpp
    tests/in_check.cpp
    tests/is_capture.cpp
    tests/is_legal.cpp
    tests/iterator.cpp
    tests/movegen.cpp
    tests/passed_pawns.cpp
    tests/perft.cpp
    tests/pinned.cpp
    tests/squares_attacked.cpp
)

# Add example
add_executable(
    perft
    examples/perft.cpp
)

# Add example
add_executable(
    split
    examples/split.cpp
)

# Add example
add_executable(
    suite
    examples/suite.cpp
)

set_target_properties(
    libchess-static
    PROPERTIES
    OUTPUT_NAME "chess"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
)

set_target_properties(
    libchess-shared
    PROPERTIES
    OUTPUT_NAME "chess"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
)

set_property(TARGET libchess-test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)

target_link_libraries(libchess-test libchess-static)
target_link_libraries(perft libchess-static)
target_link_libraries(split libchess-static)
target_link_libraries(suite libchess-static)
