#include <iostream>
#include "CursesOptim.h"
#include "MathFunc.h"

#ifdef DEBUG_DO
#include "DiffStoper.h"
#include "OptMethod.h"
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

#ifdef DEBUG_DO
    AbsStop<double> stoper(*F[5], 100, 0.001);
    stoper.SetParam(*F[5], 100, 0.001);
    DetermOptimization<double> MyDetOpt(*F[5], stoper, 1e-6, 1e-2);

    MyDetOpt.SetArea(Point<double>({-1, -1, -1}), Point<double>({1, 1, 1}));
    MyDetOpt.SetParam(*F[5], stoper, 1e-6, 1e-2);
    MyDetOpt.DoOptimize(Point<double>({0.5, 0.5, 0.5}));

    std::cout << MyDetOpt.getValueLastPoint() << std::endl;
    std::cout << MyDetOpt.getPathway().back() << std::endl;

    //test();
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