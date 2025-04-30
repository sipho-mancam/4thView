/********************************************************************************
** Form generated from reading UI file 'teams_configurationyegFkK.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEAMS_CONFIGURATIONYEGFKK_H
#define TEAMS_CONFIGURATIONYEGFKK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_teamsConfig
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGraphicsView *teamAColor;
    QLineEdit *teamAEdit;
    QPushButton *teamASave;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QGraphicsView *teamBColor;
    QLineEdit *teamBEdit;
    QPushButton *teamBSave;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *teamsConfig)
    {
        if (teamsConfig->objectName().isEmpty())
            teamsConfig->setObjectName("teamsConfig");
        teamsConfig->setWindowModality(Qt::WindowModality::ApplicationModal);
        teamsConfig->resize(551, 345);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(teamsConfig->sizePolicy().hasHeightForWidth());
        teamsConfig->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(teamsConfig);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(teamsConfig);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        teamAColor = new QGraphicsView(teamsConfig);
        teamAColor->setObjectName("teamAColor");

        verticalLayout->addWidget(teamAColor);

        teamAEdit = new QLineEdit(teamsConfig);
        teamAEdit->setObjectName("teamAEdit");
        teamAEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(teamAEdit);

        teamASave = new QPushButton(teamsConfig);
        teamASave->setObjectName("teamASave");

        verticalLayout->addWidget(teamASave);


        horizontalLayout->addLayout(verticalLayout);

        line = new QFrame(teamsConfig);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        horizontalLayout->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(teamsConfig);
        label_2->setObjectName("label_2");
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        teamBColor = new QGraphicsView(teamsConfig);
        teamBColor->setObjectName("teamBColor");

        verticalLayout_2->addWidget(teamBColor);

        teamBEdit = new QLineEdit(teamsConfig);
        teamBEdit->setObjectName("teamBEdit");
        teamBEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(teamBEdit);

        teamBSave = new QPushButton(teamsConfig);
        teamBSave->setObjectName("teamBSave");

        verticalLayout_2->addWidget(teamBSave);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(teamsConfig);
        okButton->setObjectName("okButton");

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(teamsConfig);
        cancelButton->setObjectName("cancelButton");

        hboxLayout->addWidget(cancelButton);


        verticalLayout_3->addLayout(hboxLayout);


        retranslateUi(teamsConfig);
        QObject::connect(okButton, &QPushButton::clicked, teamsConfig, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, teamsConfig, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(teamsConfig);
    } // setupUi

    void retranslateUi(QDialog *teamsConfig)
    {
        teamsConfig->setWindowTitle(QCoreApplication::translate("teamsConfig", "Teams Config", nullptr));
        label->setText(QCoreApplication::translate("teamsConfig", "Team A", nullptr));
        teamAEdit->setPlaceholderText(QCoreApplication::translate("teamsConfig", "Team Name", nullptr));
        teamASave->setText(QCoreApplication::translate("teamsConfig", "Save Team", nullptr));
        label_2->setText(QCoreApplication::translate("teamsConfig", "Team B", nullptr));
        teamBEdit->setPlaceholderText(QCoreApplication::translate("teamsConfig", "Team Name", nullptr));
        teamBSave->setText(QCoreApplication::translate("teamsConfig", "Save Team", nullptr));
        okButton->setText(QCoreApplication::translate("teamsConfig", "Save", nullptr));
        cancelButton->setText(QCoreApplication::translate("teamsConfig", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class teamsConfig: public Ui_teamsConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TEAMS_CONFIGURATIONYEGFKK_H
