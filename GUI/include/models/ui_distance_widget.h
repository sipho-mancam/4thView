/********************************************************************************
** Form generated from reading UI file 'distance_widgethOARbP.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DISTANCE_WIDGETHOARBP_H
#define DISTANCE_WIDGETHOARBP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DistanceD
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *select_color;
    QHBoxLayout *horizontalLayout;
    QPushButton *player1_btn;
    QPushButton *player2_btn;
    QGraphicsView *graphicsView;
    QHBoxLayout *hboxLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DistanceD)
    {
        if (DistanceD->objectName().isEmpty())
            DistanceD->setObjectName("DistanceD");
        DistanceD->resize(274, 152);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DistanceD->sizePolicy().hasHeightForWidth());
        DistanceD->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(DistanceD);
        verticalLayout->setObjectName("verticalLayout");
        select_color = new QPushButton(DistanceD);
        select_color->setObjectName("select_color");

        verticalLayout->addWidget(select_color);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        player1_btn = new QPushButton(DistanceD);
        player1_btn->setObjectName("player1_btn");

        horizontalLayout->addWidget(player1_btn);

        player2_btn = new QPushButton(DistanceD);
        player2_btn->setObjectName("player2_btn");

        horizontalLayout->addWidget(player2_btn);


        verticalLayout->addLayout(horizontalLayout);

        graphicsView = new QGraphicsView(DistanceD);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setEnabled(true);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMaximumSize(QSize(256, 40));

        verticalLayout->addWidget(graphicsView);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName("hboxLayout");
        okButton = new QPushButton(DistanceD);
        okButton->setObjectName("okButton");

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(DistanceD);
        cancelButton->setObjectName("cancelButton");

        hboxLayout->addWidget(cancelButton);


        verticalLayout->addLayout(hboxLayout);


        retranslateUi(DistanceD);
        QObject::connect(okButton, &QPushButton::clicked, DistanceD, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, DistanceD, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DistanceD);
    } // setupUi

    void retranslateUi(QDialog *DistanceD)
    {
        DistanceD->setWindowTitle(QCoreApplication::translate("DistanceD", "Distance Dialog", nullptr));
        select_color->setText(QCoreApplication::translate("DistanceD", "Select Line Color", nullptr));
        player1_btn->setText(QCoreApplication::translate("DistanceD", "Select Player 1", nullptr));
        player2_btn->setText(QCoreApplication::translate("DistanceD", "Select Player 2", nullptr));
        okButton->setText(QCoreApplication::translate("DistanceD", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("DistanceD", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DistanceD: public Ui_DistanceD {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DISTANCE_WIDGETHOARBP_H
