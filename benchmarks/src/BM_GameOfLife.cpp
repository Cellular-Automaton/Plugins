///
/// BM_GameOfLife.cpp
///

#include <benchmark/benchmark.h>

#include "../../PLC/include/GameOfLife.hh"

constexpr int ITERATIONS{1000};

static void bm_game_of_life(benchmark::State &state)
{
    PLC::GameOfLife game;

    for (auto _ : state) {
        game.run();
    }
}

BENCHMARK(bm_game_of_life)->Iterations(ITERATIONS);
