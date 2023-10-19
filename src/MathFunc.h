#include "Optimization.h"

namespace F_2D
{
    class FuncNull : public GeneralFunction<double>
    {
    public:
        FuncNull() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };

    class FuncRosenbrock : public GeneralFunction<double>
    {
    public:
        FuncRosenbrock() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };

    class FuncQuadratic1 : public GeneralFunction<double>
    {
    public:
        FuncQuadratic1() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };

    class FuncSinSin : public GeneralFunction<double>
    {
    public:
        FuncSinSin() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };

    class FuncHimmelblau : public GeneralFunction<double>
    {
    public:
        FuncHimmelblau() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };
}

namespace F_3D
{
    class FuncQuadratic1 : public GeneralFunction<double>
    {
    public:
        FuncQuadratic1() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };
}

namespace F_4D
{
    class FuncQuadratic1 : public GeneralFunction<double>
    {
    public:
        FuncQuadratic1() = default;

        double Value(const Point<double>& p) const override;

        Point<double> Gradient(const Point<double>& p) const override;
    };
}