///
/// main.cpp
///

#include "../PLC/include/MultiChannel.hh"

int main(int argc, const char **argv)
{
    PLC::MultiChannel game;
    for (int i = 0; i < 50; i++)
    {
        std::cout << "Result " << i << ":" << std::endl;
        game.show();
        game.run();
    }
    return 0;
}
