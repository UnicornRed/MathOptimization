#include <iostream>
#include "MathFunc.h"
#include "CursesOptim.h"

#ifdef GUI
#include "GUI_Optim.h"
#endif

int main()
{
    GeneralFunction<double>* F[] = { new F_2D::FuncNull(), new F_2D::FuncQuadratic1(), new F_2D::FuncRosenbrock(),
                                     new F_2D::FuncSinSin(), new F_2D::FuncHimmelblau(),
                                     new F_3D::FuncQuadratic1(),
                                     new F_4D::FuncQuadratic1()};

    std::vector<FunctionData> f = {{"0", *F[0], Point<double>({-1, -1}), Point<double>({1, 1}), Point<double>({0.5, 0.5})},
                                   {"3x^2+0.5y^2+2", *F[1], Point<double>({-1, -1}), Point<double>({1, 1}), Point<double>({0.5, 0.5})},
                                   {"Rosenbrock", *F[2], Point<double>({-1, -0.1}), Point<double>({1.1, 1.1}), Point<double>({-0.5, 0.5})},
                                   {"sin(pi*sin(x)+pi*sin(y))", *F[3], Point<double>({0, 0}), Point<double>({5, 5}), Point<double>({0.5, 0.5})},
                                   {"Himmelblau", *F[4], Point<double>({-5, -5}), Point<double>({5, 5}), Point<double>({0, 0})},
                                   {"3x^2+0.5y^2+z^2+0.3xy+z+4y+2", *F[5],Point<double>({-4, -4, -4}),
                                    Point<double>({1, 1, 1}), Point<double>({0.5, 0.5, 0.5})},
                                   {"(1-x)^2+(x-y)^2+(y-z)^2+(z-w)^2", *F[6], Point<double>({0, 0, 0, 0}),
                                    Point<double>({2, 2, 2, 2}), Point<double>({0.5, 0.5, 0.5, 0.5})}};

#ifdef GUI
    test();
#else
    try
    {
        CursesOptim MyCO(f);

        MyCO.main_loop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
#endif

    for (const auto& i : F)
        delete i;

    return 0;
}