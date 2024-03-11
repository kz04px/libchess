cmake_minimum_required(VERSION 3.12)

# Project
project(Libchess VERSION 1.0 LANGUAGES CXX)

# Base directory relative includes
include_directories(.)
include_directories(./src/)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wodr -Wpacked -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(
    libchess_obj
    OBJECT
    src/attackers.cpp
    src/checkers.cpp
    src/check_evasions.cpp
    src/count_moves.cpp
    src/get_fen.cpp
    src/is_legal.cpp
    src/king_allowed.cpp
    src/legal_captures.cpp
    src/legal_moves.cpp
    src/legal_noncaptures.cpp
    src/makemove.cpp
    src/movegen.cpp
    src/perft.cpp
    src/pinned.cpp
    src/predict_hash.cpp
    src/set_fen.cpp
    src/square_attacked.cpp
    src/squares_attacked.cpp
    src/undomove.cpp
    src/valid.cpp
    src/zobrist.cpp
)

# Add the static library
add_library(
    libchess_static
    STATIC
    $<TARGET_OBJECTS:libchess_obj>
)

# Add the shared library
add_library(
    libchess_shared
    SHARED
    $<TARGET_OBJECTS:libchess_obj>
)

target_include_directories(
    libchess_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    libchess_shared
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add the test executable
add_executable(
    libchess_test
    tests/main.cpp
    tests/bitboard.cpp
    tests/checkers.cpp
    tests/consistency.cpp
    tests/draw.cpp
    tests/fen.cpp
    tests/hash.cpp
    tests/in_check.cpp
    tests/is_capture.cpp
    tests/is_checkmate.cpp
    tests/is_legal.cpp
    tests/is_stalemate.cpp
    tests/legal_moves.cpp
    tests/move.cpp
    tests/movegen.cpp
    tests/parse_move.cpp
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
    ttperft
    examples/ttperft.cpp
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

# Add example
add_executable(
    ttsuite
    examples/ttsuite.cpp
)

# Add example
add_executable(
    suite960
    examples/suite960.cpp
)

set_target_properties(
    libchess_static
    PROPERTIES
    OUTPUT_NAME "chess"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/static"
)

set_target_properties(
    libchess_shared
    PROPERTIES
    OUTPUT_NAME "chess"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
)

set_property(TARGET libchess_test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)

target_link_libraries(libchess_test libchess_static)
target_link_libraries(perft libchess_static)
target_link_libraries(ttperft libchess_static)
target_link_libraries(split libchess_static)
target_link_libraries(suite libchess_static)
target_link_libraries(ttsuite libchess_static)
target_link_libraries(suite960 libchess_static)
