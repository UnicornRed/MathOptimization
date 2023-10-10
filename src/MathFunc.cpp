#include "MathFunc.h"

double F_2D::FuncRosenbrock(const Point<double>& p)
{
    return (1 - p[0]) * (1 - p[0]) + (p[1] - p[0] * p[0]) * (p[1] - p[0] * p[0]); 
}

Point<double> F_2D::FuncRosenbrockGrad(const Point<double>& p)
{
    Point<double> res({0, 0});

    res[0] = -2 * (1 - p[0]) - 4 * p[0] * (p[1] - p[0] * p[0]);
    res[1] = 2 * (p[1] - p[0] * p[0]);
    
    return res;
}

double F_2D::FuncQuadratic1(const Point<double>& p)
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + 2; 
}

Point<double> F_2D::FuncQuadratic1Grad(const Point<double>& p)
{
    Point<double> res({0, 0});

    res[0] = 6 * p[0];
    res[1] = p[1];
    
    return res;
}

double F_3D::FuncQuadratic1(const Point<double>& p)
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + p[2] * p[2] + 0.3 * p[0] * p[1] + p[2] + 3 * p[1] + 2; 
}

Point<double> F_3D::FuncQuadratic1Grad(const Point<double>& p)
{
    Point<double> res({0, 0, 0});

    res[0] = 6 * p[0] + 0.3 * p[1];
    res[1] = p[1] + 0.3 * p[0] + 3;
    res[2] = 2 * p[2] + 1;
    
    return res;
}