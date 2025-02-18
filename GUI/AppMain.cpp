#include "AppMain.h"
#include <QtWidgets/QGraphicsItem>
#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>



AppMain::AppMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainClass()),
    eventMan(nullptr)
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

void AppMain::closeEvent(QCloseEvent* e)
{
    if (eventMan) {
        eventMan->exit();
   }
}

void AppMain::setStreamDataStore(StreamDataStore* sDs)
{
    streamDs = sDs;
    auto cricketOvalCb = std::bind(&CricketOvalScene::updateFrameData, static_cast<CricketOvalScene*>(scene), std::placeholders::_1);
    streamDs->registerUICallback(cricketOvalCb);
}
