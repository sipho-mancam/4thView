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

    propsGroup = new PlayerPropertiesGroup(
        ui->label_3,
        ui->lineEdit_2,
        ui->lineEdit_3,
        ui->label_7
    );
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
    CricketOvalScene* cS = static_cast<CricketOvalScene*>(scene);
    QMetaObject::Connection con = connect(sDs, &StreamDataStore::dataChanged, 
        cS , &CricketOvalScene::dataChangeUpdate, Qt::QueuedConnection);

   con = connect(cS, &CricketOvalScene::selectedIdChangedSig, sDs, &StreamDataStore::setCurrentClicked);

   con = connect(sDs, &StreamDataStore::currentSelectedChangedSig, propsGroup, &PlayerPropertiesGroup::currentClickedTrack);


  if (con)
  {
      std::cout << "Connection was made successfully\n";
  }
  else {
      std::cout << "Failed to make the connection!!!!!!!!!!\n";
  }
}
