#include <QObject>
#include <QMessageBox>
#include <fstream>
#include "gui_optim.h"
#include "ui_gui_optim.h"

constexpr char copyright[] = "Oleynik Michael, 2023(C)";


GUI_Optim::GUI_Optim(std::vector<FunctionData<double>>& _f, QWidget *parent) :
    QMainWindow(parent),
    set(_f),
    drawBack(false),
    ui(new Ui::GUI_Optim)
{
    ui->setupUi(this);

    scene = new MyGraphicsScene(ui->GraphicsFunction);

    ui->GraphicsFunction->setRenderHints(QPainter::Antialiasing);
    ui->GraphicsFunction->setScene(scene);

    QObject::connect(scene, &MyGraphicsScene::signalTargetCoordinate, this, &GUI_Optim::press_mouse_scene);
    QObject::connect(&set, &Settings::okButtonPress, this, &GUI_Optim::on_actionOptimize_triggered);
    ui->statusbar->addWidget(new QLabel(copyright));
    set.setModal(true);
}

GUI_Optim::~GUI_Optim()
{
    delete scene;
    delete ui;
}

void GUI_Optim::on_actionSettings_triggered()
{
    set.exec();
}


void GUI_Optim::on_actionExit_triggered()
{
    QApplication::quit();
}

void GUI_Optim::drawFunction()
{
    scene->clear();

    if (set.GetMaxArea().size() != 2 || !set.GetDrawGraph())
    {
        drawBack = false;
        return;
    }

    drawBack = true;
    QPen pen(QColor(200, 50, 30));
    QBrush brush(QColor(200, 50, 30));
    scene->update(0, 0, 0, 0);

    int sizeRect = set.GetAccuracy();
    int width = ui->GraphicsFunction->width() / sizeRect;
    int height = ui->GraphicsFunction->height() / sizeRect;
    int color;

    double maxValue = set.GetFunction().Value(set.GetMaxArea());
    double minValue = maxValue, value;

    for (int i{}; i < width + 1; ++i)
    {
        for (int j{}; j < height + 1; ++j)
        {
            value = set.GetFunction().Value(Point<double>({set.GetMinArea()[0] + (set.GetMaxArea()[0] - set.GetMinArea()[0]) * i / width, set.GetMinArea()[1] + (set.GetMaxArea()[1] - set.GetMinArea()[1]) * j / height}));

            maxValue = std::max(maxValue, value);
            minValue = std::min(minValue, value);
        }
    }

    for (int i{-width / 2}; i <= width / 2; ++i)
    {
        for (int j{-height / 2}; j <= height / 2; ++j)
        {
            value = set.GetFunction().Value(Point<double>({set.GetMinArea()[0] + (set.GetMaxArea()[0] - set.GetMinArea()[0]) * (i + width / 2) / width, set.GetMinArea()[1] + (set.GetMaxArea()[1] - set.GetMinArea()[1]) * (j + height / 2) / height}));
            color = int((value - minValue) / (maxValue - minValue) * 255);
            pen.setColor(QColor(0, color, color));
            brush.setColor(QColor(0, color, color));
            scene->addRect(sizeRect * i, -sizeRect * j, sizeRect, sizeRect, pen, brush);
        }
    }

    pen.setColor(QColor(255, 0, 0));

    Point<double> beg = *set.GetOptim()->getPathway().begin(), end;

    for (size_t i{1}; i < set.GetOptim()->getPathway().size(); ++i)
    {
        end = set.GetOptim()->getPathway()[i];

        scene->addLine(((beg[0] - set.GetMinArea()[0]) * width / (set.GetMaxArea()[0] - set.GetMinArea()[0]) - width / 2) * sizeRect,
                       -((beg[1] - set.GetMinArea()[1]) * height / (set.GetMaxArea()[1] - set.GetMinArea()[1]) - height / 2) * sizeRect,
                       ((end[0] - set.GetMinArea()[0]) * width / (set.GetMaxArea()[0] - set.GetMinArea()[0]) - width / 2) * sizeRect,
                       -((end[1] - set.GetMinArea()[1]) * height / (set.GetMaxArea()[1] - set.GetMinArea()[1]) - height / 2) * sizeRect, pen);

        beg = end;
    }
}

void GUI_Optim::on_actionOptimize_triggered()
{
    try
    {
        set.GetStopNum().SetParam(set.GetNumIter());
        set.GetStopAbs().SetParam(set.GetFunction(), set.GetNumIter(), set.GetEpsilonAbs());
        set.GetOptimDeter().SetParam(set.GetFunction(), *set.GetStoper(), set.GetEpsilon(), set.GetStep());
        set.GetOptimStoch().SetParam(set.GetFunction(), *set.GetStoper(), set.GetProb(), set.GetDelta(), set.GetSeed(), set.GetAlpha());
        set.GetOptim()->SetArea(set.GetMinArea(), set.GetMaxArea());
        set.GetOptim()->DoOptimize(set.GetStart());
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    std::stringstream ss;

    if (set.GetOptim()->getPathway().size() > 0)
    {
        ss << set.GetOptim()->getPathway().back();
        ui->resultPoint->setText(ss.str().c_str());
        ss.str("");
        ss << set.GetOptim()->getValueLastPoint();
        ui->resultValue->setText(ss.str().c_str());
        ss.str("");
        ss << set.GetOptim()->getPathway().size();
        ui->resultCount->setText(ss.str().c_str());
        ss.str("");
    }

    drawFunction();

    set.SetNewSeed();
}


void GUI_Optim::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   if (drawBack)
       drawFunction();
}

void GUI_Optim::on_actionHelp_triggered()
{
    std::string str;
    std::stringstream ss;
    std::ifstream in("help");

    if (in.is_open())
    {
        while (in >> str)
            ss << str << " ";

        QMessageBox::about(this, "Help", ss.str().c_str());

        in.close();
    }
}

void GUI_Optim::press_mouse_scene(QPointF point)
{
    if (set.GetMaxArea().size() == 2 && drawBack)
    {
        Point<double> start({(point.x() / ui->GraphicsFunction->width() + 0.5) * (set.GetMaxArea()[0] - set.GetMinArea()[0]),
                             (-point.y() / ui->GraphicsFunction->height() + 0.5) * (set.GetMaxArea()[1] - set.GetMinArea()[1])});

        start = start + set.GetMinArea();
        std::cout << start << std::endl;

        if (!(start[0] < set.GetMinArea()[0] || start[1] < set.GetMinArea()[1] ||
              start[0] > set.GetMaxArea()[0] || start[1] > set.GetMaxArea()[1]))
        {
            set.SetStartPoint(start);

            on_actionOptimize_triggered();
        }
    }
}

