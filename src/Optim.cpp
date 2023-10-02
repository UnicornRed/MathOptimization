#include <iostream>
#include "Optimization.h"

// double f(const Point<double>& p)
// {
//     return 3 * p.x[0] * p.x[0] + 0.5 * p.x[1] * p.x[1] + p.x[2] * p.x[2] + 0.3 * p.x[0] * p.x[0] + p.x[2] + 4 * p.x[1] + 2; 
// }

void printPoint(const Point<double>& p)
{
    for_each(p.begin(), p.end(), [](const auto& a)
    {
        std::cout << a << " ";
    });

    std::cout << "\n";
}

int main()
{
    Point<double> p1({1.0, 2.0, 3.0}), p2({-1, 2, 10});

    printPoint(p1);
    printPoint(p2);
    std::cout << p1 * p2 << std::endl;
    printPoint(p1 + p2);
    printPoint(-1.0 * p1);
    printPoint(p1 * 2.0);
    return 0;
}