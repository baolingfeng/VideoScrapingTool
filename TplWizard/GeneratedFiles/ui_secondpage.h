/********************************************************************************
** Form generated from reading UI file 'secondpage.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDPAGE_H
#define UI_SECONDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_UI_SecondPage
{
public:
    QLabel *label;
    QLabel *label_3;
    QComboBox *comboBox;
    QSpinBox *spinBox_rownum;
    QLabel *label_2;
    QLineEdit *appName;
    QLabel *label_4;
    QSpinBox *spinBox_content;
    QLabel *label_5;
    QSpinBox *spinBox_url;

    void setupUi(QWizardPage *UI_SecondPage)
    {
        if (UI_SecondPage->objectName().isEmpty())
            UI_SecondPage->setObjectName(QStringLiteral("UI_SecondPage"));
        UI_SecondPage->resize(400, 390);
        label = new QLabel(UI_SecondPage);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 40, 81, 16));
        label_3 = new QLabel(UI_SecondPage);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 120, 81, 16));
        comboBox = new QComboBox(UI_SecondPage);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(140, 40, 171, 22));
        spinBox_rownum = new QSpinBox(UI_SecondPage);
        spinBox_rownum->setObjectName(QStringLiteral("spinBox_rownum"));
        spinBox_rownum->setGeometry(QRect(140, 120, 171, 22));
        label_2 = new QLabel(UI_SecondPage);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 80, 91, 16));
        appName = new QLineEdit(UI_SecondPage);
        appName->setObjectName(QStringLiteral("appName"));
        appName->setGeometry(QRect(140, 80, 171, 20));
        label_4 = new QLabel(UI_SecondPage);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 160, 81, 16));
        spinBox_content = new QSpinBox(UI_SecondPage);
        spinBox_content->setObjectName(QStringLiteral("spinBox_content"));
        spinBox_content->setEnabled(false);
        spinBox_content->setGeometry(QRect(140, 160, 171, 22));
        label_5 = new QLabel(UI_SecondPage);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 200, 46, 13));
        spinBox_url = new QSpinBox(UI_SecondPage);
        spinBox_url->setObjectName(QStringLiteral("spinBox_url"));
        spinBox_url->setEnabled(false);
        spinBox_url->setGeometry(QRect(140, 200, 171, 22));

        retranslateUi(UI_SecondPage);

        QMetaObject::connectSlotsByName(UI_SecondPage);
    } // setupUi

    void retranslateUi(QWizardPage *UI_SecondPage)
    {
        UI_SecondPage->setWindowTitle(QApplication::translate("UI_SecondPage", "WizardPage", 0));
        label->setText(QApplication::translate("UI_SecondPage", "Application Type", 0));
        label_3->setText(QApplication::translate("UI_SecondPage", "Row Number", 0));
        label_2->setText(QApplication::translate("UI_SecondPage", "Application Name", 0));
        label_4->setText(QApplication::translate("UI_SecondPage", "Content Row", 0));
        label_5->setText(QApplication::translate("UI_SecondPage", "URL Row", 0));
    } // retranslateUi

};

namespace Ui {
    class UI_SecondPage: public Ui_UI_SecondPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDPAGE_H
