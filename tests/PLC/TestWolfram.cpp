///
/// TestWolfram.cpp
///

#include <gtest/gtest.h>
#include "../../PLC/include/Wolfram.hh"

using namespace PLC;

TEST(WolframTest, DefaultConstructor) {
    std::vector<size_t> tab_expected = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t generation_id_expected = 110;
    Wolfram w;

    ASSERT_EQ(w.getTab(), tab_expected);
    ASSERT_EQ(w.getGenerationId(), generation_id_expected);
}

TEST(WolframTest, ConstructorWithTabInit) {
    std::vector<size_t> tab_init = {0, 0, 1, 0, 0};
    size_t generation_id_expected = 110;
    Wolfram w(tab_init);

    ASSERT_EQ(w.getTab(), tab_init);
    ASSERT_EQ(w.getGenerationId(), generation_id_expected);
}

TEST(WolframTest, ConstructorWithTabInitAndGenerationIdInit) {
    std::vector<size_t> tab_init = {0, 0, 1, 0, 0};
    std::size_t generation_id_init = 90;
    Wolfram w(tab_init, generation_id_init);

    ASSERT_EQ(w.getTab(), tab_init);
    ASSERT_EQ(w.getGenerationId(), generation_id_init);
}

TEST(WolframTest, Activate) {
    std::vector<size_t> tab_expected = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t generation_id_expected = 110;
    Wolfram w;

    EXPECT_EQ(w.getTab(), tab_expected);
    EXPECT_EQ(w.getGenerationId(), generation_id_expected);

    size_t nb_loop = 3;
    auto result = w.activate(nb_loop);

    ASSERT_EQ(result.size(), nb_loop);
}

TEST(WolframTest, Calculate) {
    std::vector<size_t> tab_expected = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t generation_id_expected = 110;
    Wolfram w;

    EXPECT_EQ(w.getTab(), tab_expected);
    EXPECT_EQ(w.getGenerationId(), generation_id_expected);

    size_t value1 = w.calculate(0);
    size_t value2 = w.calculate(8);
    ASSERT_EQ(value1, 0);
    ASSERT_EQ(value2, 1);
}

TEST(WolframTest, Run) {
    std::vector<size_t> tab_expected = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t generation_id_expected = 110;
    Wolfram w;

    EXPECT_EQ(w.getTab(), tab_expected);
    EXPECT_EQ(w.getGenerationId(), generation_id_expected);

    w.run();
    std::vector<size_t> res1_expected = {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(w.getTab(), res1_expected);
    w.run();
    std::vector<size_t> res2_expected = {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(w.getTab(), res2_expected);
    w.run();
    std::vector<size_t> res3_expected = {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(w.getTab(), res3_expected);
}
