///
/// GameOfLife.cpp
///

#include "../include/GameOfLife.hh"

PLC::GameOfLife::GameOfLife()
{
    this->tab = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
    };
    this->life = {2, 3};
    this->born = {3};
}

PLC::GameOfLife::GameOfLife(std::vector<std::vector<size_t>> tab_init)
{
    this->tab = std::move(tab_init);
    this->life = {2, 3};
    this->born = {3};
}

PLC::GameOfLife::GameOfLife(std::vector<std::vector<size_t>> tab_init, std::vector<size_t> life_init, std::vector<size_t> born_init)
{
    this->tab = std::move(tab_init);
    this->life = life_init;
    this->born = born_init;
}

std::vector<PLC::GameOfLife> PLC::GameOfLife::activate(size_t nb_loop)
{
    std::vector<GameOfLife> result;

    for (size_t i = 0; i < nb_loop; ++i) {
        GameOfLife tmp (this->tab, this->life, this->born);
        result.push_back(tmp);
        this->run();
    }
    return result;
}

static bool is_in_vector(std::vector<size_t> vec, size_t value)
{
    for (auto &elem : vec) {
        if (elem == value) {
            return true;
        }
    }
    return false;
}

void PLC::GameOfLife::run()
{
    std::vector<std::vector<size_t>> new_tab = this->tab;

    for (size_t i = 0; i < this->tab.size(); ++i) {
        for (size_t j = 0; j < this->tab[0].size(); ++j) {
            size_t nb_life = this->calculate(i, j);
            if (this->tab[i][j] == 1) {
                if (!is_in_vector(this->life, nb_life)) {
                    new_tab[i][j] = 0;
                }
            } else {
                if (is_in_vector(this->born, nb_life)) {
                    new_tab[i][j] = 1;
                }
            }
        }
    }
    this->tab = new_tab;
}

void PLC::GameOfLife::show()
{
    for (size_t i = 0; i < this->tab.size(); ++i) {
        for (size_t j = 0; j < this->tab[0].size(); ++j) {
            std::cout << (this->tab[i][j] == 1 ? "#" : ".");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

size_t PLC::GameOfLife::calculate(size_t x, size_t y)
{
    size_t nb_life = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            int raw_idx = x + i;
            int col_idx = y + j;

            if (raw_idx >= 0 && raw_idx < tab.size() &&
                col_idx >= 0 && col_idx < tab[0].size() &&
                tab[raw_idx][col_idx] == 1) {
                ++nb_life;
            }
        }
    }

    return nb_life;
}
