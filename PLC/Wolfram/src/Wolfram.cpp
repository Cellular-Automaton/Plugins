///
/// Wolfram.cpp
///

#include "../include/Wolfram.hh"

PLC::Wolfram::Wolfram()
{
    this->tab = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    this->generation_id = 110;
}

PLC::Wolfram::Wolfram(std::vector<size_t> tab_init)
{
    this->tab = std::move(tab_init);
    this->generation_id = 110;
}

PLC::Wolfram::Wolfram(std::vector<size_t> tab_init, size_t generation_id_init)
{
    this->tab = std::move(tab_init);
    this->generation_id = generation_id_init;
}

std::vector<PLC::Wolfram> PLC::Wolfram::activate(size_t nb_loop)
{
    std::vector<Wolfram> result;

    for (size_t i = 0; i < nb_loop; ++i) {
        Wolfram tmp (this->tab, this->generation_id);
        result.push_back(tmp);
        this->run();
    }
    return result;
}

void PLC::Wolfram::run()
{
    std::vector<size_t> new_tab = this->tab;

    for (size_t i = 0; i < this->tab.size(); ++i) {
        new_tab[i] = this->calculate(i);
    }
    this->tab = new_tab;
}

void PLC::Wolfram::show()
{
    for (size_t i = 0; i < this->tab.size(); ++i) {
        std::cout << (this->tab[i] == 1 ? "#" : ".");
    }
    std::cout << std::endl;
}

size_t PLC::Wolfram::calculate(size_t idx)
{
    size_t triplet = 0;

    for (int i = -1; i <= 1; i++) {
        if ((idx + i) < 0 || (idx + i) >= this->tab.size() || (this->tab[idx + i] == 0 && (1 - i) == 0))
            continue;
        triplet += pow(this->tab[idx + i] * 2, (1 - i));
    }
    return (this->generation_id >> triplet) & 1;
}
