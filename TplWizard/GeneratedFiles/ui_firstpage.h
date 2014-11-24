/********************************************************************************
** Form generated from reading UI file 'firstpage.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRSTPAGE_H
#define UI_FIRSTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_UI_FirstPage
{
public:
    QRadioButton *btn_create;
    QRadioButton *btn_modify;
    QLineEdit *editFolder;
    QPushButton *btnOpen;

    void setupUi(QWizardPage *UI_FirstPage)
    {
        if (UI_FirstPage->objectName().isEmpty())
            UI_FirstPage->setObjectName(QStringLiteral("UI_FirstPage"));
        UI_FirstPage->resize(658, 415);
        btn_create = new QRadioButton(UI_FirstPage);
        btn_create->setObjectName(QStringLiteral("btn_create"));
        btn_create->setGeometry(QRect(30, 100, 201, 17));
        btn_modify = new QRadioButton(UI_FirstPage);
        btn_modify->setObjectName(QStringLiteral("btn_modify"));
        btn_modify->setGeometry(QRect(30, 130, 251, 17));
        editFolder = new QLineEdit(UI_FirstPage);
        editFolder->setObjectName(QStringLiteral("editFolder"));
        editFolder->setGeometry(QRect(50, 160, 441, 20));
        btnOpen = new QPushButton(UI_FirstPage);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));
        btnOpen->setGeometry(QRect(500, 160, 75, 23));

        retranslateUi(UI_FirstPage);

        QMetaObject::connectSlotsByName(UI_FirstPage);
    } // setupUi

    void retranslateUi(QWizardPage *UI_FirstPage)
    {
        UI_FirstPage->setWindowTitle(QApplication::translate("UI_FirstPage", "WizardPage", 0));
        btn_create->setText(QApplication::translate("UI_FirstPage", "Create a new applcation tamplate", 0));
        btn_modify->setText(QApplication::translate("UI_FirstPage", "Create from an existing application template", 0));
        btnOpen->setText(QApplication::translate("UI_FirstPage", "Open", 0));
    } // retranslateUi

};

namespace Ui {
    class UI_FirstPage: public Ui_UI_FirstPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRSTPAGE_H
