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
    std::function<T(const Point<T>&)> f;

    Optimization(std::function<T(const Point<T>&)> _f, GeneralStop<T>& _stopIteration);

    void SetArea(const Point<T>& _min, const Point<T>& _max);
    
    void DoOptimize(const Point<T>& start);

    inline const std::vector<Point<T>> getPathway() const { return pathway; }
    inline const T getValueLastPoint() const { return f(pathway.back()); }

    virtual ~Optimization() {}
};

template <typename T>
Optimization<T>::Optimization(std::function<T(const Point<T>&)> _f, GeneralStop<T>& _stopIteration)
    : stopIteration(_stopIteration), f(_f)
{
    SetArea(Point<T>({-1, -1, -1}), Point<T>({1, 1, 1}));
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

template <typename T>
class DetermOptimization : public Optimization<T>
{
private:
    T beta;
    T epsilon;
    T epsilonStep;
    Point<T> conjugateVector;
    std::function<Point<T>(const Point<T>&)> gradF;

    T OneDimensionalOptim(const T& argMin, const T& argMax, T& res, std::function<T(const T&)> oneF);
    T MinAlpha(const Point<T>& point, const Point<T>& conjugateVector);
protected:
    Point<T> NextPoint(const Point<T>& point) override;
    void SetStart(const Point<T>& startPoint) override;
public:
    DetermOptimization(std::function<T(const Point<T>&)> _f, std::function<Point<T>(const Point<T>&)> _gradF, GeneralStop<T>& _stopIteration,
                       const T& _epsilon, const T& _epsilonStep);
};

template <typename T>
DetermOptimization<T>::DetermOptimization(std::function<T(const Point<T>&)> _f, std::function<Point<T>(const Point<T>&)> _gradF, GeneralStop<T>& _stopIteration,
                                          const T& _epsilon, const T& _epsilonStep)
    : Optimization<T>(_f, _stopIteration), epsilon(_epsilon), epsilonStep(_epsilonStep), gradF(_gradF)
{

}

template <typename T>
T DetermOptimization<T>::OneDimensionalOptim(const T& argMin, const T& argMax, T& res, std::function<T(const T&)> oneF)
{
#ifdef DEBUG
    std::cout << "ArgMax: " << argMax << std::endl;
#endif
    if (argMin == argMax)
        return argMin;

    enum Edge { left, right, nothing } lastEdge;
    T valueLeft, valueRight, leftEdge, rightEdge, leftSetEdge, rightSetEdge, minValue = oneF(argMin), middle, value;
    T epsilonOne = this->epsilon * (argMax - argMin), epsilonStepOne = epsilonStep * (argMax - argMin);
    res = (argMin + epsilonOne) / 2;

    for (T i = argMin; i + epsilonStepOne <= argMax; i = i + epsilonStepOne)
    {
        leftEdge = i, rightEdge = i + epsilonStepOne;
        lastEdge = nothing;

        while (rightEdge - leftEdge > epsilonOne)
        {
            leftSetEdge = rightEdge - (rightEdge - leftEdge) / PHI;
            rightSetEdge = leftEdge + (rightEdge - leftEdge) / PHI;

            switch (lastEdge)
            {
            case nothing:
                valueLeft = oneF(rightSetEdge);
            case left:
                valueRight = valueLeft;
                valueLeft = oneF(leftSetEdge);

                lastEdge = right;
                break;
            
            case right:
                valueRight = valueLeft;
                valueLeft = oneF(leftSetEdge);

                lastEdge = left;
                break;
            }

            if (valueLeft < valueRight)
                rightEdge = rightSetEdge;
            else
                leftEdge = leftSetEdge;
        }

        middle = (leftEdge + rightEdge) / 2;
        value = oneF(middle);
#ifdef DEBUG
    std::cout << "Value function: " << value << std::endl;
    std::cout << "Alpha: " << middle << std::endl << std::endl;
#endif
        if (value < minValue)
        {
            minValue = value;
            res = middle;
        }
    }

    return minValue;
}

template <typename T>
void DetermOptimization<T>::SetStart(const Point<T>& startPoint)
{
    conjugateVector = -gradF(startPoint);
}

template <typename T>
T DetermOptimization<T>::MinAlpha(const Point<T>& point, const Point<T>& conjugateVector)
{
    T minAlpha = ((conjugateVector[0] >= 0 ? this->area.maxArea[0] : this->area.minArea[0]) - point[0]) / conjugateVector[0];

    for (size_t i{}; i < conjugateVector.size(); ++i)
        minAlpha = std::min(minAlpha,
                            ((conjugateVector[i] >= 0 ? this->area.maxArea[i] : this->area.minArea[i]) - point[i]) / conjugateVector[i]);

    return minAlpha;
}

template <typename T>
Point<T> DetermOptimization<T>::NextPoint(const Point<T>& p)
{
    T alpha;
    Point<T> nextP;

    OneDimensionalOptim(0, MinAlpha(p, conjugateVector), alpha, [this, &p](const T al)
    {
        return this->f(p + al * this->conjugateVector);
    });

    nextP = p + alpha * conjugateVector;
    beta = (gradF(nextP) * (gradF(nextP) + (-gradF(p)))) / (gradF(p) * gradF(p));
#ifdef DEBUG
    std::cout << "Alpha: " << alpha << std::endl;
    std::cout << "Now point: ";
    p.printPoint();
    std::cout << "Next point: ";
    nextP.printPoint();
    std::cout << "Conjugate Vector: ";
    conjugateVector.printPoint();
    std::cout << "Gradient (now point): ";
    gradF(p).printPoint();
    std::cout << "Gradient (next point): ";
    gradF(nextP).printPoint();
    std::cout << "Beta: " << beta  << std::endl << std::endl;
#endif
    conjugateVector = (-gradF(nextP)) + beta * conjugateVector;

    return nextP;
}



template <typename T>
class StochastOptimization : public Optimization<T>
{
private:
    T delta;
    T deltaStart;
    T probability;
    T alpha;
    std::mt19937 generator;
    std::uniform_real_distribution<T> distr;
    CubicArea<T> sphereArea;

    void NewStochPoint(Point<T>& nextPointHelp, const Point<T>& start, const Point<T>& end);
    void IntersectionArea(CubicArea<T>& _sphereArea, const CubicArea<T>& _area);
protected:
    Point<T> NextPoint(const Point<T>& point) override;
    void SetStart(const Point<T>& startPoint) override;
public:
    StochastOptimization(std::function<T(const Point<T>&)> _f, GeneralStop<T>& _stopIteration, T _probability, T _delta,
                         size_t _seed = 0, T _alpha = static_cast<T>(1));
};

template <typename T>
StochastOptimization<T>::StochastOptimization(std::function<T(const Point<T>&)> _f, GeneralStop<T>& _stopIteration, T _probability, T _delta,
                                              size_t _seed, T _alpha) : Optimization<T>(_f, _stopIteration), delta(_delta), deltaStart(_delta),
                                                                        probability(_probability), alpha(_alpha), generator(_seed),
                                                                        distr(static_cast<T>(0), static_cast<T>(1)),
                                                                        sphereArea(Point<T>({0, 0, 0}), Point<T>({0, 0, 0}))
{

}

template <typename T>
void StochastOptimization<T>::SetStart(const Point<T>& startPoint)
{

}

template <typename T>
void StochastOptimization<T>::NewStochPoint(Point<T>& nextPointHelp, const Point<T>& start, const Point<T>& end)
{
    for (size_t i{}; i < nextPointHelp.size(); ++i)
    {
        std::uniform_real_distribution<T> unifDistrInArea(start[i], end[i]);

        nextPointHelp[i] = unifDistrInArea(generator);
    }
}

template <typename T>
void StochastOptimization<T>::IntersectionArea(CubicArea<T>& _sphereArea, const CubicArea<T>& _area)
{
    for (size_t i{}; i < _sphereArea.minArea.size(); ++i)
    {
        _sphereArea.minArea[i] = std::max(_sphereArea.minArea[i], _area.minArea[i]);
        _sphereArea.maxArea[i] = std::min(_sphereArea.maxArea[i], _area.maxArea[i]);
    }
}

template <typename T>
Point<T> StochastOptimization<T>::NextPoint(const Point<T>& point)
{
    Point<T> nextPointHelp = point;
    sphereArea.minArea = point + (-Point<T>(std::vector<T>(point.size(), delta)));
    sphereArea.maxArea = point + Point<T>(std::vector<T>(point.size(), delta));
    IntersectionArea(sphereArea, this->area);

    if (distr(generator) < probability)
        NewStochPoint(nextPointHelp, sphereArea.minArea, sphereArea.maxArea);
    else
        NewStochPoint(nextPointHelp, this->area.minArea, this->area.maxArea);

    if (this->f(nextPointHelp) >= this->f(point))
    {
        delta = deltaStart;

        return point;
    }
    else
    {
        delta = delta * alpha;
        
        return nextPointHelp;
    }
}