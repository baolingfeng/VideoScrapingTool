/********************************************************************************
** Form generated from reading UI file 'cvtoolwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVTOOLWINDOW_H
#define UI_CVTOOLWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CVToolWindowClass
{
public:

    void setupUi(QWidget *CVToolWindowClass)
    {
        if (CVToolWindowClass->objectName().isEmpty())
            CVToolWindowClass->setObjectName(QStringLiteral("CVToolWindowClass"));
        CVToolWindowClass->resize(600, 400);

        retranslateUi(CVToolWindowClass);

        QMetaObject::connectSlotsByName(CVToolWindowClass);
    } // setupUi

    void retranslateUi(QWidget *CVToolWindowClass)
    {
        CVToolWindowClass->setWindowTitle(QApplication::translate("CVToolWindowClass", "CVToolWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class CVToolWindowClass: public Ui_CVToolWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVTOOLWINDOW_H
