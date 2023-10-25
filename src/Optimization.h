/// @mainpage Optimization Method
/// @author Oleynik Michael
/// @date 03.05.2023

/// @file
/// @brief Abstract classes for optimization methods, functions and stoppers.
/// @details File contains the definition of abstract classes for optimization methods, functions and stoppers.
/// For using them you need to use inheritance. You cannot use them by themselves.
#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <chrono>
#include <random>
#include "Point.h"

#define PHI (1 + std::sqrt(5) / 2)

/// @brief Abstract class for stoppers.
/// @tparam T Typename for a value of a function.
template <typename T>
class GeneralStop
{
private:
public:
    /// @brief Default constructor.
    GeneralStop() {};

    /// @brief Function of a condition for stoping.
    /// @param[in] pathway Pathway of a optimization.
    /// @return Result of a condition.
    virtual bool condition(const std::vector<Point<T>>& pathway) const = 0;

    /// @brief Virtual destructor.
    virtual ~GeneralStop() {}
};

/// @brief Abstract class for function.
/// @tparam T Typename for a value of a function.
template <typename T>
class GeneralFunction
{
public:
    /// @brief Default constructor.
    GeneralFunction() = default;

    /// @brief Function culculated a value of function.
    /// @param[in] _ Point of a argument of a function.
    /// @return Value of a function.
    virtual T Value(const Point<T>&) const = 0;
    virtual Point<T> Gradient(const Point<T>&) const = 0;

    /// @brief Virtual destructor.
    virtual ~GeneralFunction() {}
};

/// @brief Struct of a area for optimization.
/// @tparam T Typename of a point's coordinate.
template <typename T>
struct CubicArea
{
    /// @brief Minimum point of a area.
    Point<T> minArea;
    
    /// @brief Maximum point of a area.
    Point<T> maxArea;
};

/// @brief Abstract class for optimization methods.
/// @tparam T Typename for a value of a function.
template <typename T>
class Optimization
{
private:
    GeneralStop<T>& stopIteration;
    Point<T> nowPoint;
    std::vector<Point<T>> pathway;
protected:
    CubicArea<T> area;
    GeneralFunction<T>& f;
    virtual Point<T> NextPoint(const Point<T>& point) = 0;
    virtual void SetStart(const Point<T>& startPoint) = 0;
public:
    /// @brief Constructor of a optimization.
    /// @param[in] _f Function for optimization.
    /// @param[in] _stopIteration Stopper for stoping.
    Optimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration);

    /// @brief Sets minimum and maximum point of an area.
    /// @param[in] _min Minimum point of a area.
    /// @param[in] _max Maximum point of a area.
    void SetArea(const Point<T>& _min, const Point<T>& _max);
    
    /// @brief Function whith optimase math functions.
    /// @param[in] start Start point of a pathway.
    void DoOptimize(const Point<T>& start);

    inline const std::vector<Point<T>> getPathway() const { return pathway; }
    inline const T getValueLastPoint() const { return f.Value(pathway.back()); }

    /// @brief Virtual destructor.
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