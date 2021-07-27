#include "gtest/gtest.h"
#include "main.h"

const double absErr = 1e8;

inline void checkEq(const Eigen::Vector2d& a, const Eigen::Vector2d& b)
{
    EXPECT_NEAR(a[0], b[0], absErr);
    EXPECT_NEAR(a[1], b[1], absErr);
}

// Demonstrate some basic assertions.
TEST(PS0, rotation) {
    checkEq(rotate({ 0,1 }, PI), Eigen::Vector2d(0, -1));
    checkEq(rotate({ 0,1 }, PI / 2), Eigen::Vector2d(-1, 0));
    checkEq(rotate({ 1,0 }, PI), Eigen::Vector2d(-1, 0));
    checkEq(rotate({ 1,0 }, PI / 2), Eigen::Vector2d(0, 1));
}