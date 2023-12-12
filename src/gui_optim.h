#ifndef GUI_OPTIM_H
#define GUI_OPTIM_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "mygraphicsscene.h"
#include "settings.h"

namespace Ui {
class GUI_Optim;
}

class GUI_Optim : public QMainWindow
{
    Q_OBJECT

    Settings set;

    bool drawBack;

public:
    GUI_Optim(std::vector<FunctionData<double>>& _f, QWidget *parent = nullptr);
    ~GUI_Optim();

private slots:
    void on_actionSettings_triggered();

    void on_actionExit_triggered();

    void on_actionOptimize_triggered();

    void resizeEvent(QResizeEvent* event);

    void on_actionHelp_triggered();

    void press_mouse_scene(QPointF point);

private:
    void drawFunction();

    Ui::GUI_Optim *ui;
    MyGraphicsScene *scene;
};

#endif // GUI_OPTIM_H
