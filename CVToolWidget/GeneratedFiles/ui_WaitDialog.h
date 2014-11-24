/********************************************************************************
** Form generated from reading UI file 'WaitDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITDIALOG_H
#define UI_WAITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_WaitDialog
{
public:
    QLabel *label;

    void setupUi(QDialog *WaitDialog)
    {
        if (WaitDialog->objectName().isEmpty())
            WaitDialog->setObjectName(QStringLiteral("WaitDialog"));
        WaitDialog->resize(400, 300);
        label = new QLabel(WaitDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 100, 46, 13));

        retranslateUi(WaitDialog);

        QMetaObject::connectSlotsByName(WaitDialog);
    } // setupUi

    void retranslateUi(QDialog *WaitDialog)
    {
        WaitDialog->setWindowTitle(QApplication::translate("WaitDialog", "Dialog", 0));
        label->setText(QApplication::translate("WaitDialog", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class WaitDialog: public Ui_WaitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITDIALOG_H
