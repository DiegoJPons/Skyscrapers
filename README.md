# Skyscrapers

A **C** implementation of the [Skyscrapers](https://www.brainbashers.com/showskyscraper.asp) logic puzzle for **Stony Brook CSE 220** (Systems Fundamentals I). The project provides an **interactive** mode where you place building heights on an **N×N** grid (**4×4**–**8×8**) from clues around the edges, and a **solver** that fills the board using constraint-based heuristics (edge clues, propagation, elimination, and sequence filtration). Automated tests exercise the solver via **Google Test**.

---

## Features

- **Interactive game** — `hw1_game` reads the board size, initial grid, and clue string from the command line, then prompts for piece, row, and column until the puzzle is solved or you quit.
- **Solver** — `hw1_solver` runs `solve()` on the same inputs; optional printing of the final board after solving (for local debugging).
- **Heuristics** — Edge clue initialization, constraint propagation, process of elimination, and clue elimination (sequence filtration) as specified in the assignment; no brute-force backtracking required for credit.
- **Tests** — `hw1_solver_tests` links **Google Test** and runs unit tests against `hw1.c`.

---

## Tech stack

| Area | Technologies |
|------|----------------|
| Language | C11 |
| Build | CMake |
| Solver tests | C++, Google Test (`gtest`), pthread |
| Math | libm |

---

## Project structure

```
include/          hw1.h — declarations shared by game, solver, and tests
src/
  hw1.c           Core logic (board init, game rules, solver)
  hw1_game.c      Interactive main
  hw1_solver.c    Solver main
tests/
  unit_tests.cpp  Google Test cases for solve()
CMakeLists.txt    Targets: hw1_game, hw1_solver, hw1_solver_tests
```

The repository root is the CMake source directory; build artifacts go under `build/`.

---

## Build & run

```bash
cmake -S . -B build
cmake --build build
```

**Interactive example** (Linux / macOS / Codespaces):

```bash
./build/hw1_game 4 "3-2-431-1-4--1-4" "2124242121333321"
```

**Solver** (after implementing `solve()`):

```bash
./build/hw1_solver 4 "3-2-431-1-4--1-4" "2124242121333321"
./build/hw1_solver_tests
```

---

## Note

This repository is coursework for CSE 220. Puzzle rules follow the course specification; follow your instructor’s policies on collaboration and use of generative AI.
