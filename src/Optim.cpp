#include <iostream>
#include "CursesOptim.h"
#include "MathFunc.h"
#include "DiffStoper.h"

int main()
{
    std::vector<FunctionData> f = {{"3x^2+0.5y^2+2", F_2D::FuncQuadratic1, F_2D::FuncQuadratic1Grad,
                                    Point<double>({-1, -1}), Point<double>({1, 1}), Point<double>({0.5, 0.5})},
                                   {"Rosenbrock", F_2D::FuncRosenbrock, F_2D::FuncRosenbrockGrad,
                                    Point<double>({-1, -0.1}), Point<double>({1.1, 1.1}), Point<double>({-0.5, 0.5})},
                                   {"3x^2+0.5y^2+z^2+0.3xy+z+4y+2", F_3D::FuncQuadratic1, F_3D::FuncQuadratic1Grad,
                                    Point<double>({-1, -1, -1}), Point<double>({1, 1, 1}), Point<double>({0.5, 0.5, 0.5})}};

    NumStop<double> numStop(100);
    DetermOptimization<double> MDO(f[2].f, f[2].gradF, numStop, 1e-6, 1e-2);
    MDO.SetArea(f[2].minArea, f[2].maxArea);
    MDO.DoOptimize(f[2].start);
    std::cout << MDO.getValueLastPoint() << std::endl;

    CursesOptim MyCO(f);

    MyCO.main_loop();

    return 0;
}