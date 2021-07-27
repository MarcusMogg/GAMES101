#pragma once

#include <eigen3/Eigen/Core>
#include<cmath>

const double PI = acos(-1);

// 将input绕原点逆时针旋转theta
inline Eigen::Vector2d rotate(const Eigen::Vector2d& input, double theta)
{
    Eigen::Matrix2d rotation;
    rotation << cos(theta), sin(theta), sin(theta), -cos(theta);
    return rotation * input;
}

// 将input 平移 x
inline Eigen::Vector2d translation(const Eigen::Vector2d& input, const Eigen::Vector2d& x)
{
    return x + input;
}
