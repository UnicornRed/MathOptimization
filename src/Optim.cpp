#include <iostream>
#include "Optimization.h"
#include "DiffStoper.h"

double f1(const Point<double>& p)
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + p[2] * p[2] + 0.3 * p[0] * p[1] + p[2] + 4 * p[1] + 2; 
}

Point<double> gradF1(const Point<double>& p)
{
    Point<double> res({0, 0, 0});

    res[0] = 6 * p[0] + 0.3 * p[1];
    res[1] = p[1] + 0.3 * p[0] + 4;
    res[2] = 2 * p[2] + 1;
    
    return res;
}

double f2(const Point<double>& p)
{
    return (1 - p[0]) * (1 - p[0]) + (p[1] - p[0] * p[0]) * (p[1] - p[0] * p[0]); 
}

Point<double> gradF2(const Point<double>& p)
{
    Point<double> res({0, 0});

    res[0] = -2 * (1 - p[0]) - 4 * p[0] * (p[1] - p[0] * p[0]);
    res[1] = 2 * (p[1] - p[0] * p[0]);
    
    return res;
}

double f3(const Point<double>& p)
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + p[2] * p[2] + 2; 
}

Point<double> gradF3(const Point<double>& p)
{
    Point<double> res({0, 0, 0});

    res[0] = 6 * p[0];
    res[1] = p[1];
    res[2] = 2 * p[2];
    
    return res;
}

int main()
{
    NumStop<double> stoperNum(100);
    DetermOptimization<double> MyDetOpt(f2, gradF2, stoperNum, 1e-10, 1e-2);

    MyDetOpt.SetArea(Point<double>({-1.0, -1.0}), Point<double>({1.1, 1.1}));
    MyDetOpt.DoOptimize(Point<double>({-0.5, 0.5}));

    for (const auto& i : MyDetOpt.getPathway())
        i.printPoint();

    std::cout << MyDetOpt.getPathway().size() << std::endl;
    MyDetOpt.getPathway().back().printPoint();
    std::cout << MyDetOpt.getValueLastPoint() << std::endl;

    AbsStop<double> stoperAbs(f2, 10000, 1e-6);
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    StochastOptimization<double> MyStochOpt(f2, stoperAbs, 0.9, 0.1, seed, 0.2);

    MyStochOpt.SetArea(Point<double>({-1.0, -1.0}), Point<double>({1.1, 1.1}));
    MyStochOpt.DoOptimize(Point<double>({-0.5, 0.5}));

    for (const auto& i : MyStochOpt.getPathway())
        i.printPoint();

    std::cout << MyStochOpt.getPathway().size() << std::endl;
    std::cout << MyStochOpt.getValueLastPoint() << std::endl;

    return 0;
}