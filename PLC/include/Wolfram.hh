///
/// Wolfram.hh
///

#pragma once

#include "PLC.hh"

namespace PLC
{
    class Wolfram
    {
        public:
            Wolfram();
            Wolfram(std::vector<size_t> tab_init);
            Wolfram(std::vector<size_t> tab_init, size_t generation_id_init);
            ~Wolfram() = default;

            std::vector<size_t> getTab();
            size_t getGenerationId();

            std::vector<Wolfram> activate(size_t nb_loop);
            void run();
            void show();
            size_t calculate(size_t idx);

        private:
            std::vector<size_t> tab;
            size_t generation_id;
    };
}; // namespace PLC
