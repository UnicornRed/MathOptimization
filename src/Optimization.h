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

static const size_t MAXSTEP = 100;

/// @brief Abstract class for stoppers.
/// @tparam T Typename for a value of a function.
template <typename T>
class GeneralStop
{
protected:
    size_t maxStep;
public:
    /// @brief Default constructor.
    GeneralStop(size_t _maxStep = MAXSTEP) : maxStep(_maxStep) {};

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
    GeneralStop<T>* stopIteration;
    Point<T> nowPoint;
    std::vector<Point<T>> pathway;
protected:
    CubicArea<T> area;
    GeneralFunction<T>* f;

    /// @brief Takes next point of the pathway.
    /// @param point Last point.
    /// @return Next point.
    virtual Point<T> NextPoint(const Point<T>& point) = 0;
    virtual void SetStart(const Point<T>& startPoint) = 0;
    void SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration);

    /// @brief It checked correct of field.
    virtual void CorrectField() = 0;
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
    inline const T getValueLastPoint() const { return f->Value(pathway.back()); }

    /// @brief Virtual destructor.
    virtual ~Optimization() {}
};

template <typename T>
Optimization<T>::Optimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration)
    : stopIteration(&_stopIteration), f(&_f)
{

}

template <typename T>
void Optimization<T>::SetArea(const Point<T>& _min, const Point<T>& _max)
{
    if (_min.size() != _max.size())
        throw std::invalid_argument("Size of points not equal.");

    for (size_t i{}; i < _min.size(); ++i)
        if (_min[i] > _max[i])
            throw std::invalid_argument("The minimum point must be less than the maximum point in all coordinates.");

    area.minArea = _min;
    area.maxArea = _max;

    pathway.clear();
}

template <typename T>
void Optimization<T>::SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration)
{
    f = &_f;
    stopIteration = &_stopIteration;
}

template <typename T>
void Optimization<T>::DoOptimize(const Point<T>& start)
{
    if (start.size() != area.minArea.size())
        throw std::invalid_argument("Size of start point is not equal size of area.");

    for (size_t i{}; i < start.size(); ++i)
        if (start[i] > area.maxArea[i] || start[i] < area.minArea[i])
            throw std::invalid_argument("The start point must be less than the maximum point and greater than the minimum point in all coordinates.");

    nowPoint = start;
    SetStart(start);
    pathway.push_back(nowPoint);

    while (stopIteration->condition(pathway))
    {
        nowPoint = NextPoint(nowPoint);
        pathway.push_back(nowPoint);
    }
}