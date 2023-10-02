#include "Optimization.h"

template <typename T>
bool NumStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    return pathway.size() < maxStep;
}

template <typename T>
bool AbsStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    if (pathway.size() >= maxStep)
        return false;

    for (auto& it = pathway.rbegin(); it < pathway.rend(); ++it)
        if (f(pathway.back()) < f(*it))
        {
            if (f(*it) - f(pathway.back()) < epsilon)
                return false;
            else
                return true;
        }
    
    return true;
}

template <typename T>
Optimization<T>::Optimization(std::function<double(Point<T>)> _f, const GeneralStop<T>& _stopIteration)
    : f(_f), stopIteration(_stopIteration)
{

}

template <typename T>
void Optimization<T>::DoOptimize(const Point<T>& start)
{
    nowPoint = start;
    SetStart(start);
    pathway.push_back(nowPoint);

    while (stopIteration.condition())
    {
        nowPoint = NextPoint(nowPoint);
        pathway.push_back(nowPoint);
    }
}

template <typename T>
DetermOptimization<T>::DetermOptimization(std::function<double(Point<T>)> _f, std::function<double(Point<T>)> _gradF, const GeneralStop<T>& _stopIteration)
    : Optimization<T>(_f, _stopIteration), gradF(_gradF)
{
    
}

template <typename T>
Point<T> DetermOptimization<T>::NextPoint(const Point<T>& p)
{

}