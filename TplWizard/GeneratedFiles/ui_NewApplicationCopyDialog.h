/********************************************************************************
** Form generated from reading UI file 'NewApplicationCopyDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWAPPLICATIONCOPYDIALOG_H
#define UI_NEWAPPLICATIONCOPYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewApplicationCopy
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *editAppName;
    QLabel *label;
    QComboBox *comboBoxTpl;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *comboBoxApp;

    void setupUi(QDialog *NewApplicationCopy)
    {
        if (NewApplicationCopy->objectName().isEmpty())
            NewApplicationCopy->setObjectName(QStringLiteral("NewApplicationCopy"));
        NewApplicationCopy->resize(400, 300);
        layoutWidget = new QWidget(NewApplicationCopy);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 250, 351, 33));
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

        editAppName = new QLineEdit(NewApplicationCopy);
        editAppName->setObjectName(QStringLiteral("editAppName"));
        editAppName->setGeometry(QRect(130, 30, 231, 20));
        label = new QLabel(NewApplicationCopy);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 101, 16));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        comboBoxTpl = new QComboBox(NewApplicationCopy);
        comboBoxTpl->setObjectName(QStringLiteral("comboBoxTpl"));
        comboBoxTpl->setGeometry(QRect(130, 70, 231, 22));
        label_2 = new QLabel(NewApplicationCopy);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 70, 101, 16));
        label_2->setFont(font);
        label_3 = new QLabel(NewApplicationCopy);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 120, 101, 16));
        label_3->setFont(font);
        comboBoxApp = new QComboBox(NewApplicationCopy);
        comboBoxApp->setObjectName(QStringLiteral("comboBoxApp"));
        comboBoxApp->setGeometry(QRect(130, 120, 231, 22));

        retranslateUi(NewApplicationCopy);
        QObject::connect(okButton, SIGNAL(clicked()), NewApplicationCopy, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), NewApplicationCopy, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewApplicationCopy);
    } // setupUi

    void retranslateUi(QDialog *NewApplicationCopy)
    {
        NewApplicationCopy->setWindowTitle(QApplication::translate("NewApplicationCopy", "New Application Copy", 0));
        okButton->setText(QApplication::translate("NewApplicationCopy", "OK", 0));
        cancelButton->setText(QApplication::translate("NewApplicationCopy", "Cancel", 0));
        label->setText(QApplication::translate("NewApplicationCopy", "Application Name", 0));
        label_2->setText(QApplication::translate("NewApplicationCopy", "Copy Template", 0));
        label_3->setText(QApplication::translate("NewApplicationCopy", "Copy Application", 0));
    } // retranslateUi

};

namespace Ui {
    class NewApplicationCopy: public Ui_NewApplicationCopy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWAPPLICATIONCOPYDIALOG_H
