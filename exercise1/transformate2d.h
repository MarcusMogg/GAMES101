#pragma once

#include <cmath>
#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/Geometry"

const double PI = acos(-1);

// 将input绕原点逆时针旋转theta
inline Eigen::Vector2d myRotate(const Eigen::Vector2d& input, double thetaRadian)
{
    Eigen::Matrix2d rotation;
    thetaRadian *= PI;
    rotation << cos(thetaRadian), -sin(thetaRadian), sin(thetaRadian), cos(thetaRadian);
    return rotation * input;
}

inline Eigen::Vector2d eRotate(const Eigen::Vector2d& input, double thetaRadian)
{
    const Eigen::Rotation2D<double> rot(thetaRadian * PI);
    return rot * input;
}

// 缩放
inline Eigen::Vector2d myScale(const Eigen::Vector2d& input, double x, double y)
{
    Eigen::Matrix2d scale;
    scale << x, 0, 0, y;
    return scale * input;
}

inline Eigen::Vector2d eScale(const Eigen::Vector2d& input, double x, double y)
{
    const auto scale = Eigen::Scaling(x, y);
    return scale * input;
}

