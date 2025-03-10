/********************************************************************************
** Form generated from reading UI file 'WidgetFGYXkw.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WIDGETFGYXKW_H
#define WIDGETFGYXKW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_distance_widget
{
public:
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_4;
    QPushButton *pushButton;

    void setupUi(QWidget *distance_widget)
    {
        if (distance_widget->objectName().isEmpty())
            distance_widget->setObjectName("distance_widget");
        distance_widget->resize(400, 103);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(distance_widget->sizePolicy().hasHeightForWidth());
        distance_widget->setSizePolicy(sizePolicy);
        distance_widget->setMaximumSize(QSize(400, 200));
        horizontalLayout_3 = new QHBoxLayout(distance_widget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        graphicsView = new QGraphicsView(distance_widget);
        graphicsView->setObjectName("graphicsView");
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMaximumSize(QSize(50, 50));

        horizontalLayout_2->addWidget(graphicsView);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(distance_widget);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(distance_widget);
        label_3->setObjectName("label_3");
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        label_2 = new QLabel(distance_widget);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setPointSize(12);
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_4 = new QLabel(distance_widget);
        label_4->setObjectName("label_4");
        label_4->setFont(font1);

        gridLayout->addWidget(label_4, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        pushButton = new QPushButton(distance_widget);
        pushButton->setObjectName("pushButton");
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(30, 30));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        pushButton->setIcon(icon);

        horizontalLayout_2->addWidget(pushButton);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(distance_widget);

        QMetaObject::connectSlotsByName(distance_widget);
    } // setupUi

    void retranslateUi(QWidget *distance_widget)
    {
        distance_widget->setWindowTitle(QCoreApplication::translate("distance_widget", "Form", nullptr));
        label->setText(QCoreApplication::translate("distance_widget", "Distance: ", nullptr));
        label_3->setText(QCoreApplication::translate("distance_widget", "23", nullptr));
        label_2->setText(QCoreApplication::translate("distance_widget", "distance", nullptr));
        label_4->setText(QCoreApplication::translate("distance_widget", "0.0123", nullptr));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class distance_widget: public Ui_distance_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WIDGETFGYXKW_H
