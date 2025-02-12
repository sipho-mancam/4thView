#include "AppMain.h"

AppMain::AppMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainClass())
{
    ui->setupUi(this);
}

AppMain::~AppMain()
{
    delete ui;
}
