#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AppMain.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AppMainClass; };
QT_END_NAMESPACE

class AppMain : public QMainWindow
{
    Q_OBJECT

public:
    AppMain(QWidget *parent = nullptr);
    ~AppMain();

private:
    Ui::AppMainClass *ui;
    QGraphicsScene* scene;
};
