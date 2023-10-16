#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <chrono>
#include <random>
#include "Point.h"

#define PHI (1 + std::sqrt(5) / 2)

template <typename T>
class GeneralStop
{
private:
public:
    GeneralStop() {};

    virtual bool condition(const std::vector<Point<T>>& pathway) const = 0;

    virtual ~GeneralStop() {}
};

template <typename T>
class GeneralFunction
{
public:
    GeneralFunction() = default;

    virtual T Value(const Point<T>&) const = 0;
    virtual Point<T> Gradient(const Point<T>&) const = 0;

    virtual ~GeneralFunction() {}
};

template <typename T>
struct CubicArea
{
    Point<T> minArea;
    Point<T> maxArea;
};

template <typename T>
class Optimization
{
private:
    GeneralStop<T>& stopIteration;
    Point<T> nowPoint;
    std::vector<Point<T>> pathway;
protected:
    CubicArea<T> area;
    virtual Point<T> NextPoint(const Point<T>& point) = 0;
    virtual void SetStart(const Point<T>& startPoint) = 0;
public:
    GeneralFunction<T>& f;

    Optimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration);

    void SetArea(const Point<T>& _min, const Point<T>& _max);
    
    void DoOptimize(const Point<T>& start);

    inline const std::vector<Point<T>> getPathway() const { return pathway; }
    inline const T getValueLastPoint() const { return f.Value(pathway.back()); }

    virtual ~Optimization() {}
};

template <typename T>
Optimization<T>::Optimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration)
    : stopIteration(_stopIteration), f(_f)
{

}

template <typename T>
void Optimization<T>::SetArea(const Point<T>& _min, const Point<T>& _max)
{
    area.minArea = _min;
    area.maxArea = _max;

    pathway.clear();
}

template <typename T>
void Optimization<T>::DoOptimize(const Point<T>& start)
{
    nowPoint = start;
    SetStart(start);
    pathway.push_back(nowPoint);

    while (stopIteration.condition(pathway))
    {
        nowPoint = NextPoint(nowPoint);
        pathway.push_back(nowPoint);
    }
}