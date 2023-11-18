#ifndef GUI_OPTIM_H
#define GUI_OPTIM_H

#include <QMainWindow>

namespace Ui {
class GUI_Optim;
}

class GUI_Optim : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI_Optim(QWidget *parent = nullptr);
    ~GUI_Optim();

private slots:
    void on_actionSettings_triggered();

    void on_actionExit_triggered();

private:
    Ui::GUI_Optim *ui;
};

#endif // GUI_OPTIM_H
