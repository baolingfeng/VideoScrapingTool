/********************************************************************************
** Form generated from reading UI file 'cvtoolwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVTOOLWIDGET_H
#define UI_CVTOOLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CVToolWidgetClass
{
public:
    QLabel *label;
    QLineEdit *editImgFolder;
    QPushButton *btnImgFolder;
    QLabel *label_2;
    QLineEdit *editTplFolder;
    QLabel *label_3;
    QLineEdit *editTplName;
    QLabel *label_4;
    QLineEdit *editOutputFolder;
    QPushButton *btnOutputFolder;
    QPushButton *btnRun;
    QLabel *label_5;
    QLineEdit *editImgNum;
    QLabel *label_6;
    QSpinBox *spinFrom;
    QLabel *label_7;
    QSpinBox *spinTo;
    QPushButton *btnTplFolder;

    void setupUi(QWidget *CVToolWidgetClass)
    {
        if (CVToolWidgetClass->objectName().isEmpty())
            CVToolWidgetClass->setObjectName(QStringLiteral("CVToolWidgetClass"));
        CVToolWidgetClass->resize(520, 332);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(149, 149, 223, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(250, 250, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(199, 199, 239, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(74, 74, 111, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(99, 99, 149, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush7(QColor(202, 202, 239, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        CVToolWidgetClass->setPalette(palette);
        label = new QLabel(CVToolWidgetClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 91, 20));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush9(QColor(0, 0, 255, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush9);
        QBrush brush10(QColor(127, 127, 255, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush10);
        QBrush brush11(QColor(63, 63, 255, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush11);
        QBrush brush12(QColor(0, 0, 127, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush12);
        QBrush brush13(QColor(0, 0, 170, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush11);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush12);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush11);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        label->setPalette(palette1);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        editImgFolder = new QLineEdit(CVToolWidgetClass);
        editImgFolder->setObjectName(QStringLiteral("editImgFolder"));
        editImgFolder->setEnabled(false);
        editImgFolder->setGeometry(QRect(130, 40, 271, 20));
        btnImgFolder = new QPushButton(CVToolWidgetClass);
        btnImgFolder->setObjectName(QStringLiteral("btnImgFolder"));
        btnImgFolder->setGeometry(QRect(420, 40, 75, 23));
        label_2 = new QLabel(CVToolWidgetClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 130, 111, 16));
        label_2->setFont(font);
        editTplFolder = new QLineEdit(CVToolWidgetClass);
        editTplFolder->setObjectName(QStringLiteral("editTplFolder"));
        editTplFolder->setEnabled(false);
        editTplFolder->setGeometry(QRect(130, 130, 271, 20));
        label_3 = new QLabel(CVToolWidgetClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 170, 101, 20));
        label_3->setFont(font);
        editTplName = new QLineEdit(CVToolWidgetClass);
        editTplName->setObjectName(QStringLiteral("editTplName"));
        editTplName->setGeometry(QRect(130, 170, 271, 20));
        label_4 = new QLabel(CVToolWidgetClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 210, 101, 20));
        label_4->setFont(font);
        editOutputFolder = new QLineEdit(CVToolWidgetClass);
        editOutputFolder->setObjectName(QStringLiteral("editOutputFolder"));
        editOutputFolder->setGeometry(QRect(130, 210, 271, 20));
        btnOutputFolder = new QPushButton(CVToolWidgetClass);
        btnOutputFolder->setObjectName(QStringLiteral("btnOutputFolder"));
        btnOutputFolder->setGeometry(QRect(420, 210, 75, 23));
        btnRun = new QPushButton(CVToolWidgetClass);
        btnRun->setObjectName(QStringLiteral("btnRun"));
        btnRun->setGeometry(QRect(120, 260, 151, 51));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(27);
        font1.setBold(true);
        font1.setWeight(75);
        btnRun->setFont(font1);
        label_5 = new QLabel(CVToolWidgetClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 80, 101, 20));
        editImgNum = new QLineEdit(CVToolWidgetClass);
        editImgNum->setObjectName(QStringLiteral("editImgNum"));
        editImgNum->setEnabled(false);
        editImgNum->setGeometry(QRect(140, 80, 61, 20));
        label_6 = new QLabel(CVToolWidgetClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(220, 80, 46, 13));
        spinFrom = new QSpinBox(CVToolWidgetClass);
        spinFrom->setObjectName(QStringLiteral("spinFrom"));
        spinFrom->setEnabled(false);
        spinFrom->setGeometry(QRect(250, 80, 42, 22));
        spinFrom->setValue(1);
        label_7 = new QLabel(CVToolWidgetClass);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(300, 80, 21, 16));
        spinTo = new QSpinBox(CVToolWidgetClass);
        spinTo->setObjectName(QStringLiteral("spinTo"));
        spinTo->setEnabled(false);
        spinTo->setGeometry(QRect(320, 80, 42, 22));
        btnTplFolder = new QPushButton(CVToolWidgetClass);
        btnTplFolder->setObjectName(QStringLiteral("btnTplFolder"));
        btnTplFolder->setGeometry(QRect(420, 130, 75, 23));

        retranslateUi(CVToolWidgetClass);

        QMetaObject::connectSlotsByName(CVToolWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *CVToolWidgetClass)
    {
        CVToolWidgetClass->setWindowTitle(QApplication::translate("CVToolWidgetClass", "CV Tool", 0));
        label->setText(QApplication::translate("CVToolWidgetClass", "Image Folder", 0));
        btnImgFolder->setText(QApplication::translate("CVToolWidgetClass", "Open", 0));
        label_2->setText(QApplication::translate("CVToolWidgetClass", "Template Folder", 0));
        editTplFolder->setText(QApplication::translate("CVToolWidgetClass", "../tpls/", 0));
        label_3->setText(QApplication::translate("CVToolWidgetClass", "Template Name", 0));
        label_4->setText(QApplication::translate("CVToolWidgetClass", "Output Folder", 0));
        editOutputFolder->setText(QApplication::translate("CVToolWidgetClass", "../CropOut/test", 0));
        btnOutputFolder->setText(QApplication::translate("CVToolWidgetClass", "Open", 0));
        btnRun->setText(QApplication::translate("CVToolWidgetClass", "Run", 0));
        label_5->setText(QApplication::translate("CVToolWidgetClass", "Image Total Number:", 0));
        editImgNum->setText(QApplication::translate("CVToolWidgetClass", "0", 0));
        label_6->setText(QApplication::translate("CVToolWidgetClass", "From", 0));
        label_7->setText(QApplication::translate("CVToolWidgetClass", "To", 0));
        btnTplFolder->setText(QApplication::translate("CVToolWidgetClass", "Open", 0));
    } // retranslateUi

};

namespace Ui {
    class CVToolWidgetClass: public Ui_CVToolWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVTOOLWIDGET_H
