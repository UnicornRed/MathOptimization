#pragma once

#include <vector>
#include "Optimization.h"

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
bool NumStop<T>::condition(const std::vector<Point<T>>& pathway) const
{
    return pathway.size() < maxStep;
}

template <typename T>
class AbsStop : public GeneralStop<T>
{
private:
    GeneralFunction<T>& f;
    size_t maxStep;
    T epsilon;
public:
    AbsStop(GeneralFunction<T>& _f, size_t _maxStep = 100, T _epsilon = 0.001)
        : f(_f), maxStep(_maxStep), epsilon(_epsilon) {};

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