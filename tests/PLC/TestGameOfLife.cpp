///
/// TestGameOfLife.cpp
///

#include <gtest/gtest.h>
#include "../../PLC/include/GameOfLife.hh"

using namespace PLC;

TEST(GameOfLifeTest, DefaultConstructor) {
    std::vector<std::vector<size_t> > tab_expected = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
    };
    std::vector<size_t> life_expected = {2, 3};
    std::vector<size_t> born_expected = {3};

    GameOfLife g;

    ASSERT_EQ(g.getTab(), tab_expected);
    ASSERT_EQ(g.getLife(), life_expected);
    ASSERT_EQ(g.getBorn(), born_expected);
}

TEST(GameOfLifeTest, ConstructorWithTabInit) {
    std::vector<std::vector<size_t> > tab_init = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    std::vector<size_t> life_expected = {2, 3};
    std::vector<size_t> born_expected = {3};
    GameOfLife g(tab_init);

    ASSERT_EQ(g.getTab(), tab_init);
    ASSERT_EQ(g.getLife(), life_expected);
    ASSERT_EQ(g.getBorn(), born_expected);
}

TEST(GameOfLifeTest, ConstructorWithTabInitAndGenerationIdInit) {
    std::vector<std::vector<size_t> > tab_init = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    std::vector<size_t> life_init = {4, 3};
    std::vector<size_t> born_init = {1, 9};
    GameOfLife g(tab_init, life_init, born_init);

    ASSERT_EQ(g.getTab(), tab_init);
    ASSERT_EQ(g.getLife(), life_init);
    ASSERT_EQ(g.getBorn(), born_init);
}

TEST(GameOfLifeTest, Activate) {
    std::vector<std::vector<size_t> > tab_expected = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
    };
    std::vector<size_t> life_expected = {2, 3};
    std::vector<size_t> born_expected = {3};

    GameOfLife g;

    EXPECT_EQ(g.getTab(), tab_expected);
    EXPECT_EQ(g.getLife(), life_expected);
    EXPECT_EQ(g.getBorn(), born_expected);

    size_t nb_loop = 3;
    auto result = g.activate(nb_loop);

    ASSERT_EQ(result.size(), nb_loop);
}

TEST(GameOfLifeTest, Calculate) {
    std::vector<std::vector<size_t> > tab_expected = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
    };
    std::vector<size_t> life_expected = {2, 3};
    std::vector<size_t> born_expected = {3};

    GameOfLife g;

    EXPECT_EQ(g.getTab(), tab_expected);
    EXPECT_EQ(g.getLife(), life_expected);
    EXPECT_EQ(g.getBorn(), born_expected);

    size_t value1 = g.calculate(0, 0);
    size_t value2 = g.calculate(2, 1);
    size_t value3 = g.calculate(1, 2);

    ASSERT_EQ(value1, 0);
    ASSERT_EQ(value2, 3);
    ASSERT_EQ(value3, 1);
}

TEST(GameOfLifeTest, Run) {
    std::vector<std::vector<size_t> > result_expected = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
    };
    std::vector<std::vector<size_t> > tab_expected = {
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
    };
    std::vector<size_t> life_expected = {2, 3};
    std::vector<size_t> born_expected = {3};

    GameOfLife g;

    EXPECT_EQ(g.getTab(), tab_expected);
    EXPECT_EQ(g.getLife(), life_expected);
    EXPECT_EQ(g.getBorn(), born_expected);

    g.run();
    ASSERT_EQ(g.getTab(), result_expected);
    g.run();
    ASSERT_EQ(g.getTab(), tab_expected);
}
