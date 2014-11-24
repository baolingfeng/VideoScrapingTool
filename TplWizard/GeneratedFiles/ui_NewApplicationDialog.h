/********************************************************************************
** Form generated from reading UI file 'NewApplicationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWAPPLICATIONDIALOG_H
#define UI_NEWAPPLICATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewApplication
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *editAppName;
    QLineEdit *editContentRow;
    QLineEdit *editUrlRow;
    QLabel *label_4;
    QLineEdit *editRowNum;

    void setupUi(QDialog *NewApplication)
    {
        if (NewApplication->objectName().isEmpty())
            NewApplication->setObjectName(QStringLiteral("NewApplication"));
        NewApplication->resize(400, 285);
        layoutWidget = new QWidget(NewApplication);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 190, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        label = new QLabel(NewApplication);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 101, 16));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(NewApplication);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 110, 91, 16));
        label_2->setFont(font);
        label_3 = new QLabel(NewApplication);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 150, 91, 16));
        label_3->setFont(font);
        editAppName = new QLineEdit(NewApplication);
        editAppName->setObjectName(QStringLiteral("editAppName"));
        editAppName->setGeometry(QRect(140, 30, 231, 20));
        editContentRow = new QLineEdit(NewApplication);
        editContentRow->setObjectName(QStringLiteral("editContentRow"));
        editContentRow->setGeometry(QRect(140, 110, 231, 20));
        editUrlRow = new QLineEdit(NewApplication);
        editUrlRow->setObjectName(QStringLiteral("editUrlRow"));
        editUrlRow->setGeometry(QRect(140, 150, 231, 20));
        label_4 = new QLabel(NewApplication);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 70, 91, 16));
        label_4->setFont(font);
        editRowNum = new QLineEdit(NewApplication);
        editRowNum->setObjectName(QStringLiteral("editRowNum"));
        editRowNum->setGeometry(QRect(140, 70, 231, 20));

        retranslateUi(NewApplication);
        QObject::connect(okButton, SIGNAL(clicked()), NewApplication, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), NewApplication, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewApplication);
    } // setupUi

    void retranslateUi(QDialog *NewApplication)
    {
        NewApplication->setWindowTitle(QApplication::translate("NewApplication", "New Application", 0));
        okButton->setText(QApplication::translate("NewApplication", "OK", 0));
        cancelButton->setText(QApplication::translate("NewApplication", "Cancel", 0));
        label->setText(QApplication::translate("NewApplication", "Application Name", 0));
        label_2->setText(QApplication::translate("NewApplication", "Content Row", 0));
        label_3->setText(QApplication::translate("NewApplication", "Url Row", 0));
        label_4->setText(QApplication::translate("NewApplication", "Row Number", 0));
    } // retranslateUi

};

namespace Ui {
    class NewApplication: public Ui_NewApplication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWAPPLICATIONDIALOG_H
