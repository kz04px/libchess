# libchess
A C++17 library that should provide legal move generation and be fairly performant.

---

## Build Instructions
```bash
git clone https://github.com/kz04px/libchess/
mkdir libchess/build
cd libchess/build
cmake ..
cmake --build .
```

---

## Example Programs
```
perft    -- Counts the number of nodes at a given depth
suite    -- Runs perft on a set of 126 positions
suite960 -- Runs perft on a set of chess960 positions
ttperft  -- Same as perft but with a transposition table
ttsuite  -- Same as suite but with a transposition table
split    -- Runs perft on each move in a position
```

---

## Usage
Both static and shared libraries are available, but the source files can also be included in a project directly.

---

## Tests
Tests can be run with ./libchess-tests

---

## License
libchess is released under the MIT license.

---

## Credits
- Thanks to [Catch2](https://github.com/catchorg/Catch2) - "A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD - using C++11, C++14, C++17 and later (or C++03 on the Catch1.x branch)"
