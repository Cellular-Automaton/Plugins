///
/// main.cpp
///

#include "../PLC/Lenia/include/Lenia.hh"

int main(int argc, const char **argv)
{
    PLC::Lenia game;

    std::vector<PLC::Lenia> result = game.activate(12);

    std::cout << "Result 0:" << std::endl;
    result[0].show();
    std::cout << "Result 1:" << std::endl;
    result[1].show();
    std::cout << "Result 2:" << std::endl;
    result[2].show();
    std::cout << "Result 3:" << std::endl;
    result[3].show();
    std::cout << "Result 4:" << std::endl;
    result[4].show();
    std::cout << "Result 5:" << std::endl;
    result[5].show();
    std::cout << "Result 6:" << std::endl;
    result[6].show();
    std::cout << "Result 7:" << std::endl;
    result[7].show();
    std::cout << "Result 8:" << std::endl;
    result[8].show();
    std::cout << "Result 9:" << std::endl;
    result[9].show();
    std::cout << "Result 10:" << std::endl;
    result[10].show();
    std::cout << "Last Result (" << result.size() - 1 << "):" << std::endl;
    result[result.size() - 1].show();
    return 0;
}
