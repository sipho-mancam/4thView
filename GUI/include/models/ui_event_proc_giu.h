/********************************************************************************
** Form generated from reading UI file 'event_proc_giukQmXOb.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EVENT_PROC_GIUKQMXOB_H
#define EVENT_PROC_GIUKQMXOB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_event_processor_dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *pushButton;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *event_processor_dialog)
    {
        if (event_processor_dialog->objectName().isEmpty())
            event_processor_dialog->setObjectName("event_processor_dialog");
        event_processor_dialog->resize(288, 146);
        gridLayout = new QGridLayout(event_processor_dialog);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(event_processor_dialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(event_processor_dialog);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(event_processor_dialog);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton = new QPushButton(event_processor_dialog);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        label_3 = new QLabel(event_processor_dialog);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit_2 = new QLineEdit(event_processor_dialog);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout->addWidget(lineEdit_2, 2, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName("hboxLayout");
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(event_processor_dialog);
        okButton->setObjectName("okButton");

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(event_processor_dialog);
        cancelButton->setObjectName("cancelButton");

        hboxLayout->addWidget(cancelButton);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 2);


        retranslateUi(event_processor_dialog);
        QObject::connect(okButton, &QPushButton::clicked, event_processor_dialog, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, event_processor_dialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(event_processor_dialog);
    } // setupUi

    void retranslateUi(QDialog *event_processor_dialog)
    {
        event_processor_dialog->setWindowTitle(QCoreApplication::translate("event_processor_dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("event_processor_dialog", "Event Name: ", nullptr));
        label_2->setText(QCoreApplication::translate("event_processor_dialog", "Inning;", nullptr));
        pushButton->setText(QCoreApplication::translate("event_processor_dialog", "1st", nullptr));
        label_3->setText(QCoreApplication::translate("event_processor_dialog", "Ball:", nullptr));
        okButton->setText(QCoreApplication::translate("event_processor_dialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("event_processor_dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class event_processor_dialog: public Ui_event_processor_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EVENT_PROC_GIUKQMXOB_H
