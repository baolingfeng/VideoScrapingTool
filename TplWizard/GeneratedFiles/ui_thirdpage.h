/********************************************************************************
** Form generated from reading UI file 'thirdpage.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THIRDPAGE_H
#define UI_THIRDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_UI_ThirdPage
{
public:
    QListView *RowList;
    QLabel *label;
    QListView *ItemList;
    QPushButton *btnAddItem;
    QLabel *imgLabel;
    QPushButton *btnDelItem;
    QPushButton *btnTool;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *combox_apptype;
    QLineEdit *edit_appname;
    QSpinBox *spinBox_content;
    QLabel *label_5;
    QPushButton *btnAddRow;
    QPushButton *btnDelRow;
    QSpinBox *spinBox_url;
    QPushButton *btnAddMsk;

    void setupUi(QWizardPage *UI_ThirdPage)
    {
        if (UI_ThirdPage->objectName().isEmpty())
            UI_ThirdPage->setObjectName(QStringLiteral("UI_ThirdPage"));
        UI_ThirdPage->resize(724, 587);
        RowList = new QListView(UI_ThirdPage);
        RowList->setObjectName(QStringLiteral("RowList"));
        RowList->setGeometry(QRect(10, 190, 231, 301));
        label = new QLabel(UI_ThirdPage);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 170, 46, 13));
        ItemList = new QListView(UI_ThirdPage);
        ItemList->setObjectName(QStringLiteral("ItemList"));
        ItemList->setGeometry(QRect(280, 190, 211, 301));
        btnAddItem = new QPushButton(UI_ThirdPage);
        btnAddItem->setObjectName(QStringLiteral("btnAddItem"));
        btnAddItem->setGeometry(QRect(280, 500, 75, 23));
        imgLabel = new QLabel(UI_ThirdPage);
        imgLabel->setObjectName(QStringLiteral("imgLabel"));
        imgLabel->setGeometry(QRect(540, 190, 161, 301));
        btnDelItem = new QPushButton(UI_ThirdPage);
        btnDelItem->setObjectName(QStringLiteral("btnDelItem"));
        btnDelItem->setGeometry(QRect(370, 500, 75, 23));
        btnTool = new QPushButton(UI_ThirdPage);
        btnTool->setObjectName(QStringLiteral("btnTool"));
        btnTool->setGeometry(QRect(620, 10, 91, 51));
        label_2 = new QLabel(UI_ThirdPage);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 10, 101, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_3 = new QLabel(UI_ThirdPage);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 40, 101, 16));
        label_3->setFont(font);
        label_4 = new QLabel(UI_ThirdPage);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 70, 81, 16));
        combox_apptype = new QComboBox(UI_ThirdPage);
        combox_apptype->setObjectName(QStringLiteral("combox_apptype"));
        combox_apptype->setGeometry(QRect(140, 10, 131, 22));
        edit_appname = new QLineEdit(UI_ThirdPage);
        edit_appname->setObjectName(QStringLiteral("edit_appname"));
        edit_appname->setGeometry(QRect(140, 40, 131, 20));
        spinBox_content = new QSpinBox(UI_ThirdPage);
        spinBox_content->setObjectName(QStringLiteral("spinBox_content"));
        spinBox_content->setEnabled(false);
        spinBox_content->setGeometry(QRect(140, 70, 131, 22));
        label_5 = new QLabel(UI_ThirdPage);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 100, 46, 13));
        btnAddRow = new QPushButton(UI_ThirdPage);
        btnAddRow->setObjectName(QStringLiteral("btnAddRow"));
        btnAddRow->setGeometry(QRect(10, 500, 75, 23));
        btnDelRow = new QPushButton(UI_ThirdPage);
        btnDelRow->setObjectName(QStringLiteral("btnDelRow"));
        btnDelRow->setGeometry(QRect(90, 500, 75, 23));
        spinBox_url = new QSpinBox(UI_ThirdPage);
        spinBox_url->setObjectName(QStringLiteral("spinBox_url"));
        spinBox_url->setEnabled(false);
        spinBox_url->setGeometry(QRect(140, 100, 131, 22));
        btnAddMsk = new QPushButton(UI_ThirdPage);
        btnAddMsk->setObjectName(QStringLiteral("btnAddMsk"));
        btnAddMsk->setGeometry(QRect(460, 500, 81, 23));

        retranslateUi(UI_ThirdPage);

        QMetaObject::connectSlotsByName(UI_ThirdPage);
    } // setupUi

    void retranslateUi(QWizardPage *UI_ThirdPage)
    {
        UI_ThirdPage->setWindowTitle(QApplication::translate("UI_ThirdPage", "WizardPage", 0));
        label->setText(QApplication::translate("UI_ThirdPage", "Rows", 0));
        btnAddItem->setText(QApplication::translate("UI_ThirdPage", "Add Item", 0));
        imgLabel->setText(QString());
        btnDelItem->setText(QApplication::translate("UI_ThirdPage", "Delete Item", 0));
        btnTool->setText(QApplication::translate("UI_ThirdPage", "Tool", 0));
        label_2->setText(QApplication::translate("UI_ThirdPage", "Application Type", 0));
        label_3->setText(QApplication::translate("UI_ThirdPage", "Application Name", 0));
        label_4->setText(QApplication::translate("UI_ThirdPage", "Content Row", 0));
        label_5->setText(QApplication::translate("UI_ThirdPage", "URL Row", 0));
        btnAddRow->setText(QApplication::translate("UI_ThirdPage", "Add Row", 0));
        btnDelRow->setText(QApplication::translate("UI_ThirdPage", "Delete Row", 0));
        btnAddMsk->setText(QApplication::translate("UI_ThirdPage", "Add Item Mask", 0));
    } // retranslateUi

};

namespace Ui {
    class UI_ThirdPage: public Ui_UI_ThirdPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THIRDPAGE_H
