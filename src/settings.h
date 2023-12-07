#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include <sstream>
#include <string>
#include "Optimization.h"
#include "OptMethod.h"
#include "DiffStoper.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
private:
    struct MenuParam
    {
        size_t numIter;
        double epsilon;
        double epsilonStep;
        double epsilonAbs;
        size_t seed;
        Point<double> minArea;
        Point<double> maxArea;
        Point<double> start;
        double prob;
        double delta;
        double alpha;
        GeneralStop<double>* stoper;
        GeneralFunction<double>* f;
        Optimization<double>* Opt;
        size_t accuracyImg;
    } MyMenuParam;

    std::vector<FunctionData<double>> f;
    std::mt19937 generator;
    NumStop<double> numStop;
    AbsStop<double> absStop;
    DetermOptimization<double> determOptimization;
    StochastOptimization<double> stochastOptimization;
    QLineEdit* pointsMax[3][4];
    QLineEdit* pointsMin[3][4];
    QLineEdit* pointsStart[3][4];

    void SetF(size_t numF);

    bool CheckInput();

    bool ApplyInput();

public:
    explicit Settings(std::vector<FunctionData<double>>& _f, QWidget *parent = nullptr);
    ~Settings();

    inline size_t GetSeed() const { return MyMenuParam.seed; }
    inline double GetEpsilon() const { return MyMenuParam.epsilon; }
    inline double GetEpsilonAbs() const { return MyMenuParam.epsilonAbs; }
    inline double GetStep() const { return MyMenuParam.epsilonStep; }
    inline size_t GetNumIter() const { return MyMenuParam.numIter; }
    inline const Point<double>& GetMinArea() const { return MyMenuParam.minArea; }
    inline const Point<double>& GetMaxArea() const { return MyMenuParam.maxArea; }
    inline const Point<double>& GetStart() const { return MyMenuParam.start; }
    inline double GetProb() const { return MyMenuParam.prob; }
    inline double GetDelta() const { return MyMenuParam.delta; }
    inline double GetAlpha() const { return MyMenuParam.alpha; }
    inline GeneralStop<double>* GetStoper() const { return MyMenuParam.stoper; }
    inline NumStop<double>& GetStopNum() { return numStop; }
    inline AbsStop<double>& GetStopAbs() { return absStop; }
    inline GeneralFunction<double>& GetFunction() const { return *MyMenuParam.f; }
    inline Optimization<double>* GetOptim() { return MyMenuParam.Opt; }
    inline DetermOptimization<double>& GetOptimDeter() { return determOptimization; }
    inline StochastOptimization<double>& GetOptimStoch() { return stochastOptimization; }
    inline size_t GetAccuracy() const { return MyMenuParam.accuracyImg; }

    bool GetDrawGraph() const;

    void SetNewSeed();

private slots:
    void on_radioButtonMethod_2_clicked();

    void on_radioButtonMethod_clicked();

    void on_ListFunctions_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_radioButtonStoper_clicked();

    void on_radioButtonStoper_2_clicked();

    void on_editProb_textChanged(const QString &arg1);

    void on_editDelta_textChanged(const QString &arg1);

    void on_editAlpha_textChanged(const QString &arg1);

    void on_sliderProb_valueChanged(int value);

    void on_sliderDelta_valueChanged(int value);

    void on_sliderAlpha_valueChanged(int value);

    void on_applyButton_clicked();

    void on_okButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::Settings *ui;

    static constexpr size_t numIter = 100;
    static constexpr double epsilon = 1e-6;
    static constexpr double epsilonAbs = 1e-3;
    static constexpr double epsilonStep = 1e-2;
    static constexpr double prob = 0.6;
    static constexpr double delta = 0.1;
    static constexpr double alpha = 0.2;
    static constexpr size_t accuracyImg = 10;
};

#endif // SETTINGS_H
