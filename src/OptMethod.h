/// @file
/// @brief Realization of the Conjugate Vector Method and the Stochastic Method.
/// @details File contains the definition of template classes of the Conjugate Vector Method and the Stochastic Method.
#pragma once

#include "Optimization.h"

/// @brief Class of the Conjugate Vector Method.
/// @tparam T Typename for a value of a function.
template <typename T>
class DetermOptimization : public Optimization<T>
{
private:
    T beta;
    T epsilon;
    T epsilonStep;
    Point<T> conjugateVector;

    T OneDimensionalOptim(const T& argMin, const T& argMax, T& res, std::function<T(const T&)> oneF);
    T MinAlpha(const Point<T>& point, const Point<T>& conjugateVector);
protected:
    Point<T> NextPoint(const Point<T>& point) override;
    void SetStart(const Point<T>& startPoint) override;
public:
    /// @brief Constructor of optimization of the Conjugate Vector Method.
    /// @param[in] _f Function for optimization.
    /// @param[in] _stopIteration Stopper for stoping.
    /// @param[in] _epsilon Condition of stopping for one dimension optimization.
    /// @param[in] _epsilonStep Step width in one dimension optimization.
    DetermOptimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _epsilon, const T& _epsilonStep);

    void SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _epsilon, const T& _epsilonStep);
};

template <typename T>
DetermOptimization<T>::DetermOptimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _epsilon, const T& _epsilonStep)
    : Optimization<T>(_f, _stopIteration), epsilon(_epsilon), epsilonStep(_epsilonStep)
{

}

template <typename T>
void DetermOptimization<T>::SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _epsilon, const T& _epsilonStep)
{
    Optimization<T>::SetParam(_f, _stopIteration);
    epsilon = _epsilon;
    epsilonStep = _epsilonStep;
}

template <typename T>
T DetermOptimization<T>::OneDimensionalOptim(const T& argMin, const T& argMax, T& res, std::function<T(const T&)> oneF)
{
    if (argMin >= argMax)
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
    conjugateVector = -this->f->Gradient(startPoint);
}

template <typename T>
T DetermOptimization<T>::MinAlpha(const Point<T>& point, const Point<T>& conjugateVector)
{
    bool flag = true;
    T minAlpha{};

    for (size_t i{}; i < conjugateVector.size(); ++i)
        if (std::abs(conjugateVector[i]) > 1e-10)
        {
            if (!flag)
                minAlpha = std::min(minAlpha,
                    ((conjugateVector[i] > 0 ? this->area.maxArea[i] : this->area.minArea[i]) - point[i]) / conjugateVector[i]);
            else
            {
                minAlpha = ((conjugateVector[i] > 0 ? this->area.maxArea[i] : this->area.minArea[i]) - point[i]) / conjugateVector[i];

                flag = false;
            }
#ifdef DEBUG_DO
    std::cout << "Min Alpha (iter): " << minAlpha << std::endl;
#endif
        }

    return minAlpha;
}

template <typename T>
Point<T> DetermOptimization<T>::NextPoint(const Point<T>& p)
{
    T alpha;
    Point<T> nextP;

#ifdef DEBUG_DO
    std::cout << std::endl << "Point: " << p.printPoint("%f") << std::endl;
    std::cout << "Conjugate Vector: " << conjugateVector.printPoint("%f") << std::endl;
#endif
    OneDimensionalOptim({}, MinAlpha(p, conjugateVector), alpha, [this, &p](const T al)
    {
        return this->f->Value(p + al * this->conjugateVector);
    });

    nextP = p + alpha * conjugateVector;
    
    if (this->f->Gradient(p) * this->f->Gradient(p))
        beta = (this->f->Gradient(nextP) * (this->f->Gradient(nextP) + (-this->f->Gradient(p)))) / (this->f->Gradient(p) * this->f->Gradient(p));
    else
        beta = 0;

    conjugateVector = (-this->f->Gradient(nextP)) + beta * conjugateVector;

    return nextP;
}


/// @brief Class of the Stochastic Method.
/// @tparam T Typename for a value of a function.
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
    /// @brief Constructor of optimization of the Conjugate Vector Method.
    /// @param[in] _f Function for optimization.
    /// @param[in] _stopIteration Stopper for stoping.
    /// @param[in] _probability Probability of throwing a point into the delta neighborhood.
    /// @param[in] _delta Width of the delta neighborhood.
    /// @param _seed Seed for a generator.
    /// @param[in] _alpha Сoefficient of narrowing of the delta neighborhood.
    StochastOptimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _probability, const T& _delta,
                         size_t _seed = 0, const T& _alpha = static_cast<T>(1));

    void SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _probability, const T& _delta,
                  size_t _seed = 0, const T& _alpha = static_cast<T>(1));
};

template <typename T>
StochastOptimization<T>::StochastOptimization(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _probability, const T& _delta,
                                              size_t _seed, const T& _alpha) : Optimization<T>(_f, _stopIteration), delta(_delta), deltaStart(_delta),
                                                                               probability(_probability), alpha(_alpha), generator(_seed),
                                                                               distr(static_cast<T>(0), static_cast<T>(1))
{

}

template <typename T>
void StochastOptimization<T>::SetParam(GeneralFunction<T>& _f, GeneralStop<T>& _stopIteration, const T& _probability, const T& _delta,
                                       size_t _seed, const T& _alpha)
{
    Optimization<T>::SetParam(_f, _stopIteration);
    delta = _delta;
    probability = _probability;
    alpha = _alpha;
    generator.seed(_seed);
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

    if (this->f->Value(nextPointHelp) >= this->f->Value(point))
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