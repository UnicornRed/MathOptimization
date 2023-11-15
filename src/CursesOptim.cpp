#include <chrono>
#include "CursesOptim.h"

#define EPSILON 1e-6;
#define formatT "%f"

CursesOptim::CursesOptim(std::vector<FunctionData<T>>& _f) : f(_f),
                                                          generator(std::chrono::system_clock::now().time_since_epoch().count()),
                                                          numStop(),
                                                          absStop(_f[1].f),
                                                          determOptimization(_f[1].f, numStop, epsilon, epsilonStep),
                                                          stochastOptimization(_f[1].f, numStop, prob, delta)
{
    int raw, col;

    initscr(); 

    getmaxyx(stdscr, raw, col);
    MyWin = {raw, col, false, "Mathematical function optimization program", "(C)Oleynik Michael, 2023", stdscr};
    allWin.push_back(&MyWin);
    WINDOW *menu = subwin(MyWin.win, MyWin.raw - 2, (MyWin.col - 2) / 3, 1, 1);
    refresh();
    getmaxyx(menu, raw, col);
    MyMenu = {raw, col, false, "Menu", "", menu};
    allWin.push_back(&MyMenu);
    WINDOW *function = subwin(MyWin.win, 2 * (MyWin.raw - 2) / 3, 2 * (MyWin.col - 2) / 3, 1, (MyWin.col - 2) / 3 + 1);
    refresh();
    getmaxyx(function, raw, col);
    MyFunction = {raw, col, false, "Function", "", function};
    allWin.push_back(&MyFunction);
    WINDOW *result = subwin(MyWin.win,  (MyWin.raw) / 3, 2 * (MyWin.col - 2) / 3, 2 * (MyWin.raw - 2) / 3 + 1, (MyWin.col - 2) / 3 + 1);
    refresh();
    getmaxyx(result, raw, col);
    MyResult = {raw, col, false, "Result", "", result};
    allWin.push_back(&MyResult);

    PrintAllWin(allWin);

    MyMenuParam = {MenuParam::Function, false, numIter, epsilon, epsilonStep, generator(), Point<T>({-1.0, -1.0}), Point<T>({1.0, 1.0}),
                   Point<T>({0.5, 0.5}), prob, delta, alpha, nullptr, &f[0].f, nullptr, MyMenu, 0, 0, 0, 0, int(f.size())};
}

CursesOptim::~CursesOptim()
{
    endwin();
}

void CursesOptim::PrintHeading(const WindowParam& wp)
{
    int x = 1, y = 0;
    box(wp.win, 0, 0);

    mvwaddstr(wp.win, y, x, wp.top.c_str());
    mvwaddstr(wp.win, wp.raw - 1, wp.col - wp.bottom.length() - 1, wp.bottom.c_str());
}

void CursesOptim::PrintWindow(WindowParam& wp)
{
    int raw, col;
    wclear(wp.win);

    getmaxyx(wp.win, raw, col);
    wp.raw = raw;
    wp.col = col;

    PrintHeading(wp);

    wrefresh(wp.win);
}

void CursesOptim::PrintAllWin(std::vector<WindowParam*>& wpAll)
{
    for (auto& i : wpAll)
        if (!i->hide)
            PrintWindow(*i);
}

void CursesOptim::PrintFunction(WindowParam& Function, const Point<T>& min, const Point<T>& max, const Point<T>& res, const GeneralFunction<T>& f)
{
    PrintWindow(Function);

    if (min.size() > 2)
    {
        mvwprintw(Function.win, 1, 1, "The dimension of the function is greater than 2!");
        wrefresh(Function.win);

        return;
    }

    Point<T> width = max + (-min);
    T maxValue = f.Value(Point<T>({min[0] + width[0] * (1.0 / (Function.col - 2)), min[1] + width[1] * (1.0 / (Function.raw - 2))}));
    T minValue = maxValue, value;

    for (int i{1}; i <= (Function.col - 2); ++i)
    {
        for (int j{1}; j <= (Function.raw - 2); ++j)
        {
            value = f.Value(Point<T>({min[0] + width[0] * (T(i) / (Function.col - 2)), min[1] + width[1] * (T(j) / (Function.raw - 2))}));

            maxValue = std::max(maxValue, value);
            minValue = std::min(minValue, value); 
        }
    }

    maxValue += (maxValue - minValue) * EPSILON;

    for (int i{1}; i <= (Function.col - 2); ++i)
    {
        for (int j{1}; j <= (Function.raw - 2); ++j)
        {
            int MARKER = 0;

            if (std::abs(min[0] + width[0] * (T(i) / (Function.col - 2)) - res[0]) < width[0] * (T(1) / (Function.col - 2)) / 2 &&
                std::abs(min[1] + width[1] * (T(j) / (Function.raw - 2)) - res[1]) < width[1] * (T(1) / (Function.raw - 2)) / 2)
                MARKER = A_REVERSE;

            value = f.Value(Point<T>({min[0] + width[0] * (T(i) / (Function.col - 2)), min[1] + width[1] * (T(j) / (Function.raw - 2))}));
            int letter = int((value - minValue) / (maxValue - minValue) * 52);
            mvwaddch(Function.win, Function.raw - 1 - j, i, (letter % 2 ? 'A' + letter / 2 : 'a' + letter / 2) | MARKER);
        }
    }

    wrefresh(Function.win);
}

void CursesOptim::PrintResult(WindowParam& Result, const Optimization<T>& Opt)
{
    PrintWindow(Result);

    int x = 1, y = 1;

    mvwprintw(Result.win, y, x, "Point of min: %-.30s", Opt.getPathway().back().printPoint("%8.5f").c_str());
    mvwprintw(Result.win, ++y, x, "Value in point: " formatT, Opt.getValueLastPoint());
    mvwprintw(Result.win, ++y, x, "Count of iterations: %lu", Opt.getPathway().size());

    wrefresh(Result.win);
}

void CursesOptim::PrintError(WindowParam& Result, const std::string& message)
{
    PrintWindow(Result);

    int x = 1, y = 0;
    std::string buff = message;
    int widthWindow = MyResult.col;

    mvwprintw(Result.win, ++y, x, "Error!");

    while (buff.length() > 0)
    {
        mvwprintw(Result.win, ++y, x, "%-.*s", widthWindow, buff.substr(0, widthWindow).c_str());
        buff = buff.substr(buff.length() < static_cast<size_t>(widthWindow) ? buff.length() : widthWindow);
    }

    wrefresh(Result.win);
}

void CursesOptim::PrintCondition(int y, int x, MenuParam::Cond condition, const MenuParam& MyMenuParam, const WindowParam& Menu, const char* str)
{
    if (MyMenuParam.condition == condition && !MyMenuParam.choose)
        mvwprintw(Menu.win, y, x, ">");
    
    mvwprintw(Menu.win, y, x + 1, "%s", str);
}

void CursesOptim::PrintOption(int y, int x, int numOption, int valueOption, const MenuParam& MyMenuParam, const WindowParam& Menu, const char* str)
{
    if (valueOption == numOption && MyMenuParam.choose)
        mvwprintw(Menu.win, y, x + 1, ">");

    mvwprintw(Menu.win, y, x + 2, "%.30s", str);
}

void CursesOptim::ScanSizeTOption(int y, int x, int numOption, int valueOption, size_t& sizeTParam, const WindowParam& Menu, bool newparam)
{
    if (valueOption == numOption && newparam)
        mvwscanw(Menu.win, y, x + 10, "%lu", &sizeTParam);
    else
        mvwprintw(Menu.win, y, x + 10, "%lu", sizeTParam);
}

void CursesOptim::ScanDoubleOption(int y, int x, int numOption, int valueOption, T& doubleParam, const WindowParam& Menu, bool newparam)
{
    if (valueOption == numOption && newparam)
        mvwscanw(Menu.win, y, x + 10, "%lf", &doubleParam);
    else
        mvwprintw(Menu.win, y, x + 10, "%-6g", doubleParam);
}

void CursesOptim::ScanPointOption(int y, int x, int numOption, int valueOption, Point<T>& pointParam, const WindowParam& Menu, bool newparam)
{
    if (valueOption == numOption && newparam)
        for (size_t i{}; i < pointParam.size(); ++i)
            mvwscanw(Menu.win, y, x + 10 + i * 8, "%lf", &pointParam[i]);
    else
        for (size_t i{}; i < pointParam.size(); ++i)
            mvwprintw(Menu.win, y, x + 10 + i * 8, "%6.4f ", pointParam[i]);
}

void CursesOptim::PrintMenu(WindowParam& Menu, bool newparam)
{
    echo();
    PrintWindow(Menu);

    int y = Menu.raw - 2, x = 1;

    mvwprintw(Menu.win, y, x, "'q' - quit program");
    mvwprintw(Menu.win, --y, x, "'r' - start optimization");

    if (!MyMenuParam.choose)
        mvwprintw(Menu.win, --y, x, "key right - choose option");
    else
    {
        mvwprintw(Menu.win, --y, x, "key left - cancel option");

        if (MyMenuParam.condition == MenuParam::Param)
        {
            if (newparam)
                mvwprintw(Menu.win, --y, x, "ENTER - input parameter");
            else
                mvwprintw(Menu.win, --y, x, "'n' - new parameter");
        }
    }

    y = 1;

    PrintCondition(y, x, MenuParam::Function, MyMenuParam, Menu, "Functions");

    for (int i{}; i < int(f.size()); ++i)
        PrintOption(++y, x, MyMenuParam.numF, i, MyMenuParam, Menu, f[i].name.c_str());

    PrintCondition(++y, x, MenuParam::Method, MyMenuParam, Menu, "Methods");
        PrintOption(++y, x, MyMenuParam.numMethod, 0, MyMenuParam, Menu, "Deterministic");
        PrintOption(++y, x, MyMenuParam.numMethod, 1, MyMenuParam, Menu, "Stochastic");

    PrintCondition(++y, x, MenuParam::Stoper, MyMenuParam, Menu, "Stopers");
        PrintOption(++y, x, MyMenuParam.numStoper, 0, MyMenuParam, Menu, "Number");
        PrintOption(++y, x, MyMenuParam.numStoper, 1, MyMenuParam, Menu, "Abs");

    PrintCondition(++y, x, MenuParam::Param, MyMenuParam, Menu, "Parameters");
        PrintOption(++y, x, MyMenuParam.numParam, 0, MyMenuParam, Menu, "NumIter");
        ScanSizeTOption(y, x, MyMenuParam.numParam, 0, MyMenuParam.numIter, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 1, MyMenuParam, Menu, "Epsilon");
        ScanDoubleOption(y, x, MyMenuParam.numParam, 1, MyMenuParam.epsilon, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 2, MyMenuParam, Menu, "Step");
        ScanDoubleOption(y, x, MyMenuParam.numParam, 2, MyMenuParam.epsilonStep, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 3, MyMenuParam, Menu, "Seed");
        ScanSizeTOption(y, x, MyMenuParam.numParam, 3, MyMenuParam.seed, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 4, MyMenuParam, Menu, "Min");
        ScanPointOption(y, x, MyMenuParam.numParam, 4, MyMenuParam.minArea, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 5, MyMenuParam, Menu, "Max");
        ScanPointOption(y, x, MyMenuParam.numParam, 5, MyMenuParam.maxArea, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 6, MyMenuParam, Menu, "Start");
        ScanPointOption(y, x, MyMenuParam.numParam, 6, MyMenuParam.start, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 7, MyMenuParam, Menu, "Prob");
        ScanDoubleOption(y, x, MyMenuParam.numParam, 7, MyMenuParam.prob, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 8, MyMenuParam, Menu, "Delta");
        ScanDoubleOption(y, x, MyMenuParam.numParam, 8, MyMenuParam.delta, Menu, newparam);
        PrintOption(++y, x, MyMenuParam.numParam, 9, MyMenuParam, Menu, "Alpha");
        ScanDoubleOption(y, x, MyMenuParam.numParam, 9, MyMenuParam.alpha, Menu, newparam);

    noecho();
    wrefresh(Menu.win);
}

void CursesOptim::changeOptim(int deltaOption)
{
    switch (MyMenuParam.condition)
    {
    case MenuParam::Function:
        MyMenuParam.numF = (MyMenuParam.numF + MyMenuParam.countF + deltaOption) % MyMenuParam.countF;

        MyMenuParam.minArea = f[MyMenuParam.numF].minArea;
        MyMenuParam.maxArea = f[MyMenuParam.numF].maxArea;
        MyMenuParam.start = f[MyMenuParam.numF].start;
        break;
    
    case MenuParam::Method:
        MyMenuParam.numMethod = (MyMenuParam.numMethod + countMethod + deltaOption) % countMethod;
        break;

    case MenuParam::Stoper:
        MyMenuParam.numStoper = (MyMenuParam.numStoper + countStoper + deltaOption) % countStoper;
        break;

    case MenuParam::Param:
        MyMenuParam.numParam = (MyMenuParam.numParam + countParam + deltaOption) % countParam;
        break;
    }
}

void CursesOptim::main_loop()
{
    int key;
    bool process = true;

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    PrintMenu(MyMenu, false);

    while (process)
    {
        key = getch();

        switch (key)
        {
        case KEY_RIGHT:
            if (!MyMenuParam.choose)
                MyMenuParam.choose = true;

            break;
        case KEY_UP:
            if (!MyMenuParam.choose)
                MyMenuParam.condition = static_cast<MenuParam::Cond>((MyMenuParam.condition + 3) % 4);
            else
                changeOptim(-1);
            break;
        case KEY_DOWN:
            if (!MyMenuParam.choose)
                MyMenuParam.condition = static_cast<MenuParam::Cond>((MyMenuParam.condition + 1) % 4);
            else
                changeOptim(1);
            break;
        case KEY_LEFT:
            MyMenuParam.choose = false;
            break;
        case 'n':
            if (MyMenuParam.choose && MyMenuParam.condition == MenuParam::Param)
                PrintMenu(MyMenu, true);

            break;
        case 'r':
            try
            {
                MyMenuParam.f = &f[MyMenuParam.numF].f;

                if (MyMenuParam.numStoper == 0)
                {
                    numStop.SetParam(MyMenuParam.numIter);
                    MyMenuParam.stoper = &numStop;
                }
                if (MyMenuParam.numStoper == 1)
                {
                    absStop.SetParam(*MyMenuParam.f, MyMenuParam.numIter, MyMenuParam.epsilon);
                    MyMenuParam.stoper = &absStop;
                }

                if (MyMenuParam.numMethod == 0)
                {
                    determOptimization.SetParam(*MyMenuParam.f, *MyMenuParam.stoper, MyMenuParam.epsilon, MyMenuParam.epsilonStep);
                    MyMenuParam.Opt = &determOptimization;
                }
                if (MyMenuParam.numMethod == 1)
                {
                    stochastOptimization.SetParam(*MyMenuParam.f, *MyMenuParam.stoper, MyMenuParam.prob, MyMenuParam.delta, MyMenuParam.seed, MyMenuParam.alpha);
                    MyMenuParam.Opt = &stochastOptimization;
                }

                MyMenuParam.Opt->SetArea(MyMenuParam.minArea, MyMenuParam.maxArea);
                MyMenuParam.Opt->DoOptimize(MyMenuParam.start);

                PrintResult(MyResult, *MyMenuParam.Opt);
                PrintFunction(MyFunction, MyMenuParam.minArea, MyMenuParam.maxArea, MyMenuParam.Opt->getPathway().back(), *MyMenuParam.f);

                MyMenuParam.seed = generator();
            }
            catch(const std::exception& e)
            {
                PrintError(MyResult, e.what());
                PrintWindow(MyFunction);
            }

            break;
        case 'q':
            process = false;
        }

        PrintMenu(MyMenu, false);
    }
}