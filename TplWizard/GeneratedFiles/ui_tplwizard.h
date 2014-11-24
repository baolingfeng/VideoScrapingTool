/********************************************************************************
** Form generated from reading UI file 'tplwizard.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TPLWIZARD_H
#define UI_TPLWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TplWizardClass
{
public:

    void setupUi(QWidget *TplWizardClass)
    {
        if (TplWizardClass->objectName().isEmpty())
            TplWizardClass->setObjectName(QStringLiteral("TplWizardClass"));
        TplWizardClass->resize(600, 400);

        retranslateUi(TplWizardClass);

        QMetaObject::connectSlotsByName(TplWizardClass);
    } // setupUi

    void retranslateUi(QWidget *TplWizardClass)
    {
        TplWizardClass->setWindowTitle(QApplication::translate("TplWizardClass", "TplWizard", 0));
    } // retranslateUi

};

namespace Ui {
    class TplWizardClass: public Ui_TplWizardClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TPLWIZARD_H
