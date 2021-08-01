#include <random>

#include "transformate2d.h"

#include "gtest/gtest.h"

TEST(exercise0, rotation)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    const std::uniform_int_distribution<int> d1(100, 200);
    const std::uniform_real_distribution<double> d2(0, 2);

    for (int i = 0; i < 100; ++i)
    {
        Eigen::Vector2d v(d1(eng), d1(eng));
        const double theta = d2(eng);
        ASSERT_EQ(myRotate(v, theta), eRotate(v, theta));
    }
}

TEST(exercise0, scale)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    const std::uniform_int_distribution<int> d1(100, 200);
    const std::uniform_real_distribution<double> d2(0, 2);

    for (int i = 0; i < 100; ++i)
    {
        Eigen::Vector2d v(d1(eng), d1(eng));
        const double x = d2(eng), y = d2(eng);
        ASSERT_EQ(myScale(v, x, y), eScale(v, x, y));
    }
}