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
public:
    /// @brief Constructor of the Number Stopper.
    /// @param _maxStep Maximum count of a step of iteration.
    NumStop(size_t _maxStep = MAXSTEP) : GeneralStop<T>(_maxStep) {};

    void SetParam(size_t _maxStep = MAXSTEP);

    /// @brief Function of a condition for stoping.
    /// @param[in] pathway Pathway of a optimization.
    /// @return Result of a condition.
    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
void NumStop<T>::SetParam(size_t _maxStep)
{
    this->maxStep = _maxStep;
}

template <typename T>
bool NumStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    return pathway.size() < this->maxStep;
}

/// @brief Class of the Absolute Stopper.
/// @tparam T Typename of a point's coordinate.
template <typename T>
class AbsStop : public GeneralStop<T>
{
private:
    GeneralFunction<T>& f;
    T epsilon;
public:
    /// @brief Constructor of the Absolute Stopper.
    /// @param _f Function for optimization.
    /// @param _maxStep Maximum count of a step of iteration.
    /// @param _epsilon Condition of stopping.
    AbsStop(GeneralFunction<T>& _f, size_t _maxStep = MAXSTEP, T _epsilon = 0.001)
        : GeneralStop<T>(_maxStep), f(_f), epsilon(_epsilon) {};

    void SetParam(GeneralFunction<T>& _f, size_t _maxStep = MAXSTEP, T _epsilon = 0.001);

    /// @brief Function of a condition for stoping.
    /// @param[in] pathway Pathway of a optimization.
    /// @return Result of a condition.
    bool condition(const std::vector<Point<T>>& pathway) const override;
};

template <typename T>
void AbsStop<T>::SetParam(GeneralFunction<T>& _f, size_t _maxStep, T _epsilon)
{
    f = _f;
    this->maxStep = _maxStep;
    epsilon = _epsilon;
}

template <typename T>
bool AbsStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    if (pathway.size() >= this->maxStep)
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