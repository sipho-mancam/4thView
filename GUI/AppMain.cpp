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
    
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QSize rsize = ui->graphicsView->size();
    QRectF rect(0, 0, 560, 520);
    QGraphicsEllipseItem* ellipse = scene->addEllipse(rect,
        QPen(QColor(255, 255, 255)), 
        QBrush(QColorConstants::DarkGreen));
    int width = 100, height = 200;

    scene->addRect(QRect(280 - width / 2, 260 - height / 2, 100, 200), QPen(), QBrush(QColorConstants::Cyan));
    scene->addEllipse(QRect(20, 20, 520, 480), QPen(QColorConstants::White, 3));
    QFont font;
    font.setBold(true);
    font.setPointSizeF(40);
    QGraphicsItem* text = (QGraphicsItem*)scene->addText(QString("Leg"), font );
    text->setPos(280 - width / 2 - 130, 260 - height / 2 + 50);
;
    text = (QGraphicsItem*)scene->addText(QString("Off"), font);
    text->setPos(280 + width - 20, 260 - height / 2 + 50);

    outputHandle = new StdoutStreamBuffer(this);
    connect(outputHandle, &StdoutStreamBuffer::outputCaptured, this, &AppMain::appendOutput);

}

AppMain::~AppMain()
{
    delete ui;
}
