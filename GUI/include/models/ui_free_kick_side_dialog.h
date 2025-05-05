/********************************************************************************
** Form generated from reading UI file 'free_kick_side_dialogeIKjQq.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FREE_KICK_SIDE_DIALOGEIKJQQ_H
#define FREE_KICK_SIDE_DIALOGEIKJQQ_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_freeKickSideDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *leftSide;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *rightSide;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *leftbtn;
    QPushButton *rightbtn;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *freeKickSideDialog)
    {
        if (freeKickSideDialog->objectName().isEmpty())
            freeKickSideDialog->setObjectName("freeKickSideDialog");
        freeKickSideDialog->resize(235, 218);
        freeKickSideDialog->setModal(true);
        verticalLayout = new QVBoxLayout(freeKickSideDialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(freeKickSideDialog);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        leftSide = new QGraphicsView(freeKickSideDialog);
        leftSide->setObjectName("leftSide");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftSide->sizePolicy().hasHeightForWidth());
        leftSide->setSizePolicy(sizePolicy);
        leftSide->setMaximumSize(QSize(100, 100));
        leftSide->setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::TextAntialiasing);

        horizontalLayout_2->addWidget(leftSide);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        rightSide = new QGraphicsView(freeKickSideDialog);
        rightSide->setObjectName("rightSide");
        sizePolicy.setHeightForWidth(rightSide->sizePolicy().hasHeightForWidth());
        rightSide->setSizePolicy(sizePolicy);
        rightSide->setMaximumSize(QSize(100, 100));

        horizontalLayout_2->addWidget(rightSide);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        leftbtn = new QPushButton(freeKickSideDialog);
        leftbtn->setObjectName("leftbtn");

        horizontalLayout_3->addWidget(leftbtn);

        rightbtn = new QPushButton(freeKickSideDialog);
        rightbtn->setObjectName("rightbtn");

        horizontalLayout_3->addWidget(rightbtn);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(freeKickSideDialog);
        okButton->setObjectName("okButton");

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(freeKickSideDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(freeKickSideDialog);
        QObject::connect(okButton, &QPushButton::clicked, freeKickSideDialog, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, freeKickSideDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(freeKickSideDialog);
    } // setupUi

    void retranslateUi(QDialog *freeKickSideDialog)
    {
        freeKickSideDialog->setWindowTitle(QCoreApplication::translate("freeKickSideDialog", "Select Free Kick Side", nullptr));
        label->setText(QCoreApplication::translate("freeKickSideDialog", "Select Kick Side", nullptr));
        leftbtn->setText(QCoreApplication::translate("freeKickSideDialog", "Set Left", nullptr));
        rightbtn->setText(QCoreApplication::translate("freeKickSideDialog", "Set Right", nullptr));
        okButton->setText(QCoreApplication::translate("freeKickSideDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("freeKickSideDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class freeKickSideDialog: public Ui_freeKickSideDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FREE_KICK_SIDE_DIALOGEIKJQQ_H
