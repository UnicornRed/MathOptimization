#include <cmath>
#include "MathFunc.h"

double F_2D::FuncNull::Value(const Point<double>& p) const
{
    return 0; 
}

Point<double> F_2D::FuncNull::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0});

    res[0] = 0;
    res[1] = 0;
    
    return res;
}

double F_2D::FuncRosenbrock::Value(const Point<double>& p) const
{
    return (1 - p[0]) * (1 - p[0]) + (p[1] - p[0] * p[0]) * (p[1] - p[0] * p[0]); 
}

Point<double> F_2D::FuncRosenbrock::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0});

    res[0] = -2 * (1 - p[0]) - 4 * p[0] * (p[1] - p[0] * p[0]);
    res[1] = 2 * (p[1] - p[0] * p[0]);
    
    return res;
}

double F_2D::FuncQuadratic1::Value(const Point<double>& p) const
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + 2; 
}

Point<double> F_2D::FuncQuadratic1::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0});

    res[0] = 6 * p[0];
    res[1] = p[1];
    
    return res;
}

double F_2D::FuncSinSin::Value(const Point<double>& p) const
{
    return std::sin(M_PI * std::sin(p[0]) + M_PI * std::sin(p[1])); 
}

Point<double> F_2D::FuncSinSin::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0});

    res[0] = std::cos(M_PI * std::sin(p[0]) + M_PI * std::sin(p[1])) * M_PI * std::cos(p[0]);
    res[1] = std::cos(M_PI * std::sin(p[0]) + M_PI * std::sin(p[1])) * M_PI * std::cos(p[1]);
    
    return res;
}

double F_2D::FuncHimmelblau::Value(const Point<double>& p) const
{
    return (p[0] * p[0] + p[1] - 11) * (p[0] * p[0] + p[1] - 11) + (p[0] + p[1] * p[1] - 7) * (p[0] + p[1] * p[1] - 7); 
}

Point<double> F_2D::FuncHimmelblau::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0});

    res[0] = 4 * p[0] * (p[0] * p[0] + p[1] - 11) + 2 * (p[0] + p[1] * p[1] - 7);
    res[1] = 2 * (p[0] * p[0] + p[1] - 11) + 4 * p[1] * (p[0] + p[1] * p[1] - 7);
    
    return res;
}

double F_3D::FuncQuadratic1::Value(const Point<double>& p) const
{
    return 3 * p[0] * p[0] + 0.5 * p[1] * p[1] + p[2] * p[2] + 0.3 * p[0] * p[1] + p[2] + 3 * p[1] + 2; 
}

Point<double> F_3D::FuncQuadratic1::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0, 0});

    res[0] = 6 * p[0] + 0.3 * p[1];
    res[1] = p[1] + 0.3 * p[0] + 3;
    res[2] = 2 * p[2] + 1;
    
    return res;
}

double F_4D::FuncQuadratic1::Value(const Point<double>& p) const
{
    return  (1 - p[0]) * (1 - p[0]) + (p[0] - p[1]) * (p[0] - p[1]) + (p[1] - p[2]) * (p[1] - p[2]) + (p[2] - p[3]) * (p[2] - p[3]); 
}

Point<double> F_4D::FuncQuadratic1::Gradient(const Point<double>& p) const
{
    Point<double> res({0, 0, 0, 0});

    res[0] = -2 * (1 - p[0]) + 2 * (p[0] - p[1]);
    res[1] = -2 * (p[0] - p[1]) + 2 * (p[1] - p[2]);
    res[2] = -2 * (p[1] - p[2]) + 2 * (p[2] - p[3]);
    res[3] = -2 * (p[2] - p[3]);
    
    return res;
}