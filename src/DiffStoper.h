/// @file
/// @brief Realization of the Number Stopper and the Absolute Stopper.
/// @details File contains the definition of classes of the Number Stopper and the Absolute Stopper.
#pragma once

#include <vector>
#include "Optimization.h"

/// @brief Class of the Number Stopper.
/// @tparam T Typename of a point's coordinate.
template <typename T>
class NumStop : public GeneralStop<T>
{
private:
    size_t maxStep;
public:
    /// @brief Constructor of the Number Stopper.
    /// @param _maxStep Maximum count of a step of iteration.
    NumStop(size_t _maxStep = 100) : maxStep(_maxStep) {};

    /// @brief Function of a condition for stoping.
    /// @param[in] pathway Pathway of a optimization.
    /// @return Result of a condition.
    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
bool NumStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    return pathway.size() < maxStep;
}

/// @brief Class of the Absolute Stopper.
/// @tparam T Typename of a point's coordinate.
template <typename T>
class AbsStop : public GeneralStop<T>
{
private:
    GeneralFunction<T>& f;
    size_t maxStep;
    T epsilon;
public:
    /// @brief Constructor of the Absolute Stopper.
    /// @param _f Function for optimization.
    /// @param _maxStep Maximum count of a step of iteration.
    /// @param _epsilon Condition of stopping.
    AbsStop(GeneralFunction<T>& _f, size_t _maxStep = 100, T _epsilon = 0.001)
        : f(_f), maxStep(_maxStep), epsilon(_epsilon) {};

    /// @brief Function of a condition for stoping.
    /// @param[in] pathway Pathway of a optimization.
    /// @return Result of a condition.
    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
bool AbsStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    if (pathway.size() >= maxStep)
        return false;

    for (auto it = pathway.rbegin() + 1; it < pathway.rend(); ++it)
    {
        if (f.Value(pathway.back()) < f.Value(*it))
        {
            if (f.Value(*it) - f.Value(pathway.back()) < epsilon)
                return false;
            else
                return true;
        }
    }
    
    return true;
}