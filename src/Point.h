#pragma once

#include <iostream>
#include <vector>

template <typename T, class Container = std::vector<T>>
class Point
{
private:
    Container x;
public:
    Point() {};
    Point(const Container& _x) : x(_x) {};

    auto begin() { return x.begin(); }
    auto end() { return x.end(); }

    auto begin() const  { return x.cbegin(); }
    auto end() const { return x.cend(); }

    size_t size() const { return x.size(); }

    T& operator[](size_t i) { return x[i]; }

    const T& operator[](size_t i) const { return x[i]; }

    Point<T, Container> operator-()
    {
        std::for_each(begin(), end(), [](T& p)
        {
            p = p * (-1);
        });

        return *this;
    }

    void printPoint() const;

    template <typename V, class Container_>
    friend Point<V, Container_> operator+(const Point<V, Container_>& p1, const Point<V, Container_>& p2);
    template <typename V, class Container_>
    friend Point<V, Container_> operator*(const V& alpha, const Point<V, Container_>& p);
};

template <typename T, class Container>
double operator*(const Point<T, Container>& p1, const Point<T, Container>& p2)
{
    if (p1.size() != p2.size())
        throw std::out_of_range("The dimensions of the points are not equal.");

    double res = 0;
    
    for (size_t i{0}; i < p1.size(); ++i)
        res += p1[i] * p2[i];

    return res;
}

template <typename T, class Container>
Point<T, Container> operator+(const Point<T, Container>& p1, const Point<T, Container>& p2)
{
    if (p1.size() != p2.size())
        throw std::out_of_range("The dimensions of the points are not equal.");

    Point<T, Container> p;

    std::copy(p1.begin(), p1.end(), std::back_inserter(p.x));
    
    for (size_t i{0}; i < p.size(); ++i)
        p[i] = p[i] + p2[i];

    return p;
}

template <typename T, class Container>
Point<T, Container> operator*(const T& alpha, const Point<T, Container>& p)
{
    Point<T, Container> res;

    std::copy(p.begin(), p.end(), std::back_inserter(res.x));
    
    std::for_each(res.begin(), res.end(), [&alpha](T& element)
    {
        element = element * alpha;
    });

    return res;
}

template <typename T, class Container>
Point<T, Container> operator*(const Point<T, Container>& p, const T& alpha)
{
    return alpha * p;
}

template <typename T, class Container>
void Point<T, Container>::printPoint() const
{
    for_each(begin(), end(), [](const auto& a)
    {
        std::cout << a << " ";
    });

    std::cout << "\n";
}