#include "gui_optim.h"
#include "ui_gui_optim.h"
#include "settings.h"

GUI_Optim::GUI_Optim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI_Optim)
{
    ui->setupUi(this);
}

GUI_Optim::~GUI_Optim()
{
    delete ui;
}

void GUI_Optim::on_actionSettings_triggered()
{
    Settings set;
    set.setModal(true);
    set.exec();
}


void GUI_Optim::on_actionExit_triggered()
{
    QApplication::quit();
}

