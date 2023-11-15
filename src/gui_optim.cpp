#include "gui_optim.h"
#include "ui_gui_optim.h"

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
