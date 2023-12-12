#include <QMessageBox>
#include <exception>
#include "settings.h"
#include "ui_settings.h"

Settings::Settings(std::vector<FunctionData<double>>& _f, QWidget *parent) :
    QDialog(parent),
    f(_f),
    generator(std::chrono::system_clock::now().time_since_epoch().count()),
    numStop(),
    absStop(_f[1].f),
    determOptimization(_f[1].f, numStop, epsilon, epsilonStep),
    stochastOptimization(_f[1].f, numStop, prob, delta),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    MyMenuParam = {numIter, epsilon, epsilonStep, epsilonAbs, generator(), Point<double>({-1.0, -1.0}), Point<double>({1.0, 1.0}),
                   Point<double>({0.5, 0.5}), prob, delta, alpha, &numStop, &f[0].f, &determOptimization, accuracyImg};

    ui->radioButtonMethod->setChecked(true);
    ui->radioButtonStoper->setChecked(true);

    for (const auto& i : f)
        ui->ListFunctions->addItem(QString(i.name.c_str()));

    pointsMax[0][0] = ui->editMaxx1; pointsMax[0][1] = ui->editMaxy1;
    pointsMax[1][0] = ui->editMaxx2; pointsMax[1][1] = ui->editMaxy2; pointsMax[1][2] = ui->editMaxz2;
    pointsMax[2][0] = ui->editMaxx3; pointsMax[2][1] = ui->editMaxy3; pointsMax[2][2] = ui->editMaxz3; pointsMax[2][3] = ui->editMaxw3;
    pointsMin[0][0] = ui->editMinx1; pointsMin[0][1] = ui->editMiny1;
    pointsMin[1][0] = ui->editMinx2; pointsMin[1][1] = ui->editMiny2; pointsMin[1][2] = ui->editMinz2;
    pointsMin[2][0] = ui->editMinx3; pointsMin[2][1] = ui->editMiny3; pointsMin[2][2] = ui->editMinz3; pointsMin[2][3] = ui->editMinw3;
    pointsStart[0][0] = ui->editStartx1; pointsStart[0][1] = ui->editStarty1;
    pointsStart[1][0] = ui->editStartx2; pointsStart[1][1] = ui->editStarty2; pointsStart[1][2] = ui->editStartz2;
    pointsStart[2][0] = ui->editStartx3; pointsStart[2][1] = ui->editStarty3; pointsStart[2][2] = ui->editStartz3; pointsStart[2][3] = ui->editStartw3;

    std::stringstream ss;

    ui->editEpsilon->setText((ss.str(""), ss << epsilon, ss.str().c_str()));
    ui->editNumber->setText((ss.str(""), ss << numIter, ss.str().c_str()));
    ui->editStep->setText((ss.str(""), ss << epsilonStep, ss.str().c_str()));
    ui->editEpsilonAbs->setText((ss.str(""), ss << epsilonAbs, ss.str().c_str()));
    ui->editSeed->setText((ss.str(""), ss << MyMenuParam.seed, ss.str().c_str()));
    ui->editAlpha->setText((ss.str(""), ss << alpha, ss.str().c_str()));
    ui->editProb->setText((ss.str(""), ss << prob, ss.str().c_str()));
    ui->editDelta->setText((ss.str(""), ss << delta, ss.str().c_str()));
    ui->editAccuracy->setText((ss.str(""), ss << accuracyImg, ss.str().c_str()));

    ui->sliderAlpha->setValue(static_cast<int>(alpha * 100));
    ui->sliderDelta->setValue(static_cast<int>(delta * 100));
    ui->sliderProb->setValue(static_cast<int>(prob * 100));

    ui->ListFunctions->setCurrentItem(ui->ListFunctions->item(0));

    SetF(0);
}

void Settings::SetF(size_t numF)
{
    std::stringstream ss;
    size_t sizeFPoint = f[numF].maxArea.size();

    ui->stackedWidgetMax->setCurrentIndex(sizeFPoint - 2);
    ui->stackedWidgetMin->setCurrentIndex(sizeFPoint - 2);
    ui->stackedWidgetStart->setCurrentIndex(sizeFPoint - 2);

    for (size_t i{}; i < sizeFPoint; ++i)
    {
       pointsMax[sizeFPoint - 2][i]->setText((ss.str(""), ss << f[numF].maxArea[i], ss.str().c_str()));
       pointsMin[sizeFPoint - 2][i]->setText((ss.str(""), ss << f[numF].minArea[i], ss.str().c_str()));
       pointsStart[sizeFPoint - 2][i]->setText((ss.str(""), ss << f[numF].start[i], ss.str().c_str()));
    }
}

bool IsDoubleNumb(std::string str)
{
    size_t minus = *str.begin() == '-';
    bool flagE = false, flagDot = false;

    for (size_t i = minus; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]) && str[i] != 'e' && str[i] != '-' && str[i] != '.')
            return false;
        else
        {
            if (str[i] == 'e')
            {
                if (flagE)
                    return false;
                else
                {
                    flagE = true;

                    if (i < str.length() - 1 && str[i + 1] == '-')
                    {
                        minus = 2;
                        ++i;
                    }
                }
            }
            else if (str[i] == '-')
            {
                if (flagE)
                    return false;
                else
                {
                    if (minus == 1)
                        return false;
                    else
                        ++minus;
                }
            }
            else if (str[i] == '.')
            {
                if (flagE)
                    return false;
                else
                {
                   if (flagDot)
                       return false;
                   else
                       flagDot = true;
                }
            }
        }
    }

    return true;
}

bool IsIntNumb(std::string str)
{
    for (const char& c : str)
        if (!std::isdigit(c))
            return false;

    return true;
}

bool Settings::CheckInput()
{
    size_t num;
    double x, y, z;
    std::string warnings = "";

    if (!IsIntNumb(ui->editNumber->text().toStdString()))
        warnings += "Incorrect input in [Number Iteration] field.\n";
    else
    {
        num = ui->editNumber->text().toUInt();

        if (num > 1000000)
            warnings += "[Number Iteration] must be less or equal than 1000000.\n";
    }

    if (!IsDoubleNumb(ui->editEpsilon->text().toStdString()))
        warnings += "Incorrect input in [Epsilon] field.\n";
    else
    {
        x = ui->editEpsilon->text().toDouble();

        if (x <= 0)
            warnings += "[Epsilon] must be greater than 0.\n";
    }

    if (!IsDoubleNumb(ui->editStep->text().toStdString()))
        warnings += "Incorrect input in [Step] field.\n";
    else
    {
        x = ui->editStep->text().toDouble();

        if (x <= 0)
            warnings += "[Step] must be greater than 0.\n";
    }

    if (!IsDoubleNumb(ui->editAlpha->text().toStdString()))
        warnings += "Incorrect input in [Alpha] field.\n";
    else
    {
        x = ui->editAlpha->text().toDouble();

        if (x <= 0 || x > 1)
            warnings += "[Alpha] must be greater than 0 and less or equal than 1.\n";
    }

    if (!IsDoubleNumb(ui->editDelta->text().toStdString()))
        warnings += "Incorrect input in [Delta] field.\n";
    else
    {
        x = ui->editDelta->text().toDouble();

        if (x <= 0)
            warnings += "[Delta] must be greater than 0.\n";
    }

    if (!IsDoubleNumb(ui->editProb->text().toStdString()))
        warnings += "Incorrect input in [Prob] field.\n";
    else
    {
        x = ui->editProb->text().toDouble();

        if (x <= 0 || x > 1)
            warnings += "[Prob] must be greater than 0 and less or equal than 1.\n";
    }

    if (!IsDoubleNumb(ui->editEpsilonAbs->text().toStdString()))
        warnings += "Incorrect input in [EpsilonStop] field.\n";
    else
    {
        x = ui->editEpsilonAbs->text().toDouble();

        if (x <= 0)
            warnings += "[EpsilonStop] must be greater than 0.\n";
    }

    if (!IsIntNumb(ui->editSeed->text().toStdString()))
        warnings += "Incorrect input in [Seed] field.\n";

    std::vector<std::string> coord = {"x", "y", "z", "w"};
    int curInd = ui->stackedWidgetMax->currentIndex();

    for (int j{}; j < curInd + 2; ++j)
    {
        if (!IsDoubleNumb(pointsMax[curInd][j]->text().toStdString()))
            warnings += "Incorrect input in [Maximum point " + coord[j] + "] field.\n";
        if (!IsDoubleNumb(pointsMin[curInd][j]->text().toStdString()))
            warnings += "Incorrect input in [Minimum point " + coord[j] + "] field.\n";
        if (!IsDoubleNumb(pointsStart[curInd][j]->text().toStdString()))
            warnings += "Incorrect input in [Starting point " + coord[j] + "] field.\n";

        x = pointsStart[curInd][j]->text().toDouble();
        y = pointsMax[curInd][j]->text().toDouble();
        z = pointsMin[curInd][j]->text().toDouble();

        if (y <= z)
            warnings += "[Maximum point " + coord[j] + "] must be greater than [Minimum point " + coord[j] + "].\n";

        if (x > y || x < z)
            warnings += "[Starting point " + coord[j] + "] must be greater than [Minimum point " + coord[j] + "] and less than [Maximum point " + coord[j] + "].\n";
        }

    if (!IsIntNumb(ui->editAccuracy->text().toStdString()))
        warnings += "Incorrect input in [Accuracy of Graph] field.\n";
    else
    {
        num = ui->editAccuracy->text().toUInt();

        if (num > 50)
            warnings += "[Accuracy of Graph] must be less or equal than 50.\n";
    }

    if (warnings == "")
        return true;
    else
    {
        QMessageBox::warning(this, "Warning!", warnings.c_str());

        return false;
    }
}

bool Settings::ApplyInput()
{
    if (!CheckInput())
        return false;

    MyMenuParam.numIter = ui->editNumber->text().toUInt();
    MyMenuParam.epsilon = ui->editEpsilon->text().toDouble();
    MyMenuParam.epsilonAbs = ui->editEpsilonAbs->text().toDouble();
    MyMenuParam.alpha = ui->editAlpha->text().toDouble();
    MyMenuParam.delta = ui->editDelta->text().toDouble();
    MyMenuParam.prob = ui->editProb->text().toDouble();
    MyMenuParam.epsilonStep = ui->editStep->text().toDouble();
    MyMenuParam.seed = ui->editSeed->text().toUInt();
    MyMenuParam.accuracyImg = ui->editAccuracy->text().toUInt();

    MyMenuParam.maxArea = f[ui->ListFunctions->row(ui->ListFunctions->currentItem())].maxArea;
    MyMenuParam.minArea = f[ui->ListFunctions->row(ui->ListFunctions->currentItem())].minArea;
    MyMenuParam.start = f[ui->ListFunctions->row(ui->ListFunctions->currentItem())].start;

    int curInd = ui->stackedWidgetMax->currentIndex();

    for (int i{}; i < curInd + 2; ++i)
    {
       MyMenuParam.maxArea[i] = pointsMax[curInd][i]->text().toDouble();
       MyMenuParam.minArea[i] = pointsMin[curInd][i]->text().toDouble();
       MyMenuParam.start[i] = pointsStart[curInd][i]->text().toDouble();
    }

    if (ui->radioButtonMethod->isChecked())
        MyMenuParam.Opt = &determOptimization;

    if (ui->radioButtonMethod_2->isChecked())
        MyMenuParam.Opt = &stochastOptimization;

    if (ui->radioButtonStoper->isChecked())
        MyMenuParam.stoper = &numStop;

    if (ui->radioButtonStoper_2->isChecked())
        MyMenuParam.stoper = &absStop;

    MyMenuParam.f = &f[ui->ListFunctions->row(ui->ListFunctions->currentItem())].f;

    return true;
}

bool Settings::GetDrawGraph() const
{
    return ui->checkDrawGraph->isChecked();
}

void Settings::SetNewSeed()
{
    std::stringstream ss;
    MyMenuParam.seed = generator();

    ui->editSeed->setText((ss.str(""), ss << MyMenuParam.seed, ss.str().c_str()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_radioButtonMethod_clicked()
{
    ui->stackedWidgetParam->setCurrentIndex(0);
}

void Settings::on_radioButtonMethod_2_clicked()
{
    ui->stackedWidgetParam->setCurrentIndex(1);
}

void Settings::on_ListFunctions_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous != nullptr)
        for (size_t i{}; i < f.size(); ++i)
        {
            if (previous->text() == QString(f[i].name.c_str()))
            {
               SetF(i);
               break;
            }
        }

    for (size_t i{}; i < f.size(); ++i)
    {
        if (current->text() == QString(f[i].name.c_str()))
        {
           SetF(i);
           break;
        }
    }
}


void Settings::on_radioButtonStoper_clicked()
{
    ui->stackedWidgetEpsilonAbs->setCurrentIndex(0);
}


void Settings::on_radioButtonStoper_2_clicked()
{
    ui->stackedWidgetEpsilonAbs->setCurrentIndex(1);
}


void Settings::on_editProb_textChanged(const QString &arg1)
{
    std::stringstream ss;

    ss << arg1.toStdString();
    ss.exceptions(std::ios_base::failbit);

    double x;

    try
    {
        ss >> x;

        ui->sliderProb->setValue(static_cast<int>(x * 100));
    }  catch (...)
    {

    }
}


void Settings::on_editDelta_textChanged(const QString &arg1)
{
    std::stringstream ss;

    ss << arg1.toStdString();
    ss.exceptions(std::ios_base::failbit);

    double x;

    try
    {
        ss >> x;

        ui->sliderDelta->setValue(static_cast<int>(x * 100));
    }  catch (...)
    {

    }
}


void Settings::on_editAlpha_textChanged(const QString &arg1)
{
    std::stringstream ss;

    ss << arg1.toStdString();
    ss.exceptions(std::ios_base::failbit);

    double x;

    try
    {
        ss >> x;

        ui->sliderAlpha->setValue(static_cast<int>(x * 100));
    }  catch (...)
    {

    }
}


void Settings::on_sliderProb_valueChanged(int value)
{
    std::stringstream ss;

    ui->editProb->setText((ss.str(""), ss << static_cast<double>(value) / 100, ss.str().c_str()));
}


void Settings::on_sliderDelta_valueChanged(int value)
{
    std::stringstream ss;

    ui->editDelta->setText((ss.str(""), ss << static_cast<double>(value) / 100, ss.str().c_str()));
}


void Settings::on_sliderAlpha_valueChanged(int value)
{
    std::stringstream ss;

    ui->editAlpha->setText((ss.str(""), ss << static_cast<double>(value) / 100, ss.str().c_str()));
}

void Settings::on_applyButton_clicked()
{
    ApplyInput();
}


void Settings::on_okButton_clicked()
{
    if (ApplyInput())
    {
        QWidget::close();

        emit okButtonPress();
    }
}


void Settings::on_closeButton_clicked()
{
    QWidget::close();
}

