#include "AppMain.h"
#include <QtWidgets/QGraphicsItem>
#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>



AppMain::AppMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainClass())
{
    ui->setupUi(this);
    outputHandle = new StdoutStreamBuffer(this);
    connect(outputHandle, &StdoutStreamBuffer::outputCaptured, this, &AppMain::appendOutput);

    scene = (QGraphicsScene*) new CricketOvalScene();
    ui->graphicsView->setScene(scene);
}

AppMain::~AppMain()
{
    delete ui;
}
