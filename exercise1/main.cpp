#include <iostream>

#include "transformate2d.h"

int main(int argc, char* argv[])
{
    Eigen::Vector2d v(0, 1);
    std::cout << myRotate(v, 0.5) << "\n";
    std::cout << eRotate(v, 0.5) << "\n";
    return  0;
}
