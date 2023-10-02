#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "Point.h"

template <typename T>
class Optimization;

template <typename T>
class GeneralStop
{
private:
public:
    GeneralStop() {};

    virtual bool condition(const std::vector<Point<T>>& pathway) const = 0;
};

template <typename T>
class NumStop : public GeneralStop<T>
{
private:
    size_t maxStep;
public:
    NumStop(size_t _maxStep = 100) : maxStep(_maxStep) {};

    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
class AbsStop : public GeneralStop<T>
{
private:
    std::function<T(const Point<T>&)> f;
    size_t maxStep;
    double epsilon;
public:
    AbsStop(std::function<double(Point<T>)> _f, size_t _maxStep = 100, double _epsilon = 0.001)
        : f(_f), maxStep(_maxStep), epsilon(_epsilon) {};

    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
class Optimization
{
private:
    GeneralStop<T> stopIteration;
    Point<T> nowPoint;
    std::vector<Point<T>> pathway;
    std::function<T(const Point<T>&)> f;
protected:
    virtual Point<T> NextPoint(const Point<T>& point) = 0;
    virtual void SetStart(const Point<T>& startPoint) = 0;
public:
    Optimization(std::function<double(Point<T>)> _f, const GeneralStop<T>& _stopIteration);
    
    void DoOptimize(const Point<T>& start);
};

template <typename T>
class DetermOptimization : public Optimization<T>
{
private:
    double beta;
    Point<T> conjugateVector;
    std::function<T(const Point<T>&)> gradF;

    T argminAlpha(const Point<T>& point, const Point<T>& conjugateVector);
protected:
    Point<T> NextPoint(const Point<T>& point) override;
    void SetStart(const Point<T>& startPoint) override;
public:
    DetermOptimization(std::function<double(Point<T>)> _f, std::function<double(Point<T>)> _gradF, const GeneralStop<T>& _stopIteration);
};

template <typename T>
class StochastOptimization : public Optimization<T>
{
private:
    /* data */
protected:
    Point<T> NextPoint(const Point<T>& point) override;
public:
    StochastOptimization(/* args */);
};
