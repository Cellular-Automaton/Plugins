///
/// GameOfLife.hh
///

#pragma once

#include "PLC.hh"

namespace PLC
{
    class GameOfLife
    {
        public:
            GameOfLife();
            GameOfLife(std::vector<std::vector<size_t> > tab_init);
            GameOfLife(std::vector<std::vector<size_t> > tab_init, std::vector<size_t> life_init, std::vector<size_t> born_init);
            ~GameOfLife() = default;

            std::vector<std::vector<size_t> > getTab();
            std::vector<size_t> getLife();
            std::vector<size_t> getBorn();

            std::vector<GameOfLife> activate(size_t nb_loop);
            void run();
            void show();
            size_t calculate(size_t x, size_t y);

        private:
            std::vector<std::vector<size_t> > tab;
            std::vector<size_t> life;
            std::vector<size_t> born;
    };
}; // namespace PLC
