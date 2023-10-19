#pragma once

#include <functional>
#include <string>
#include <ncurses.h>
#include <vector>
#include "Point.h"
#include "Optimization.h"

using T = double;

struct FunctionData
{
    std::string name;
    GeneralFunction<T>& f;
    Point<T> minArea;
    Point<T> maxArea;
    Point<T> start;
};

class CursesOptim
{
private:
    struct WindowParam
    {
        int raw;
        int col;
        bool hide;
        std::string top;
        std::string bottom;
        WINDOW *win;
    };

    struct MenuParam
    {
        enum Cond {Function, Method, Stoper, Param} condition;
        bool choose;
        size_t numIter;
        T epsilon;
        T epsilonStep;
        size_t seed;
        Point<T> minArea;
        Point<T> maxArea;
        Point<T> start;
        T prob;
        T delta;
        T alpha;
        GeneralStop<T>* stoper;
        GeneralFunction<T>* f;
        Optimization<T>* Opt;
        WindowParam wp;
        int numF;
        int numStoper;
        int numMethod;
        int numParam;
        int countF;
    } MyMenuParam;

    std::vector<FunctionData> f;
    std::vector<WindowParam*> allWin;
    std::mt19937 generator;
    WindowParam MyWin, MyMenu, MyResult, MyFunction;

    void PrintHeading(const WindowParam& wp);

    void PrintWindow(WindowParam& wp);

    void PrintAllWin(std::vector<WindowParam*>& wpAll);

    void PrintFunction(WindowParam& Function, const Point<T>& min, const Point<T>& max, const Point<T>& res, const GeneralFunction<T>& f);

    void PrintResult(WindowParam& Result, const Optimization<T>& Opt);

    void PrintCondition(int y, int x, MenuParam::Cond condition, const MenuParam& MyMenuParam, const WindowParam& Menu, const char* str);

    void PrintOption(int y, int x, int numOption, int valueOption, const MenuParam& MyMenuParam, const WindowParam& Menu, const char* str);

    void ScanSizeTOption(int y, int x, int numOption, int valueOption, size_t& sizeTParam, const WindowParam& Menu, bool newparam);

    void ScanDoubleOption(int y, int x, int numOption, int valueOption, T& doubleParam, const WindowParam& Menu, bool newparam);

    void ScanPointOption(int y, int x, int numOption, int valueOption, Point<T>& pointParam, const WindowParam& Menu, bool newparam);

    void PrintMenu(WindowParam& Menu, bool newparam);

    void changeOptim(int deltaOption);
public:
    CursesOptim(std::vector<FunctionData>& f);

    void main_loop();

    ~CursesOptim();
private:
    static const size_t numIter = 100;
    static constexpr T epsilon = 1e-6;
    static constexpr T epsilonStep = 1e-2;
    static constexpr T prob = 0.6;
    static constexpr T delta = 0.1;
    static constexpr T alpha = 0.2;
    static const int countParam = 10;
    static const int countMethod = 2;
    static const int countStoper = 2;
};