#include "Point.h"

namespace F_2D
{
    double FuncRosenbrock(const Point<double>& p);

    Point<double> FuncRosenbrockGrad(const Point<double>& p);

    double FuncQuadratic1(const Point<double>& p);

    Point<double> FuncQuadratic1Grad(const Point<double>& p);
}

namespace F_3D
{
    double FuncQuadratic1(const Point<double>& p);

    Point<double> FuncQuadratic1Grad(const Point<double>& p);
}