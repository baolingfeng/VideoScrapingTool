/********************************************************************************
** Form generated from reading UI file 'NewTemplateDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWTEMPLATEDIALOG_H
#define UI_NEWTEMPLATEDIALOG_H

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

class Ui_NewTemplateDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *NewTemplateDialog)
    {
        if (NewTemplateDialog->objectName().isEmpty())
            NewTemplateDialog->setObjectName(QStringLiteral("NewTemplateDialog"));
        NewTemplateDialog->resize(400, 104);
        layoutWidget = new QWidget(NewTemplateDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 60, 351, 33));
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

        label = new QLabel(NewTemplateDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 121, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);
        lineEdit = new QLineEdit(NewTemplateDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(140, 20, 241, 20));
        layoutWidget->raise();
        label->raise();
        lineEdit->raise();

        retranslateUi(NewTemplateDialog);
        QObject::connect(okButton, SIGNAL(clicked()), NewTemplateDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), NewTemplateDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewTemplateDialog);
    } // setupUi

    void retranslateUi(QDialog *NewTemplateDialog)
    {
        NewTemplateDialog->setWindowTitle(QApplication::translate("NewTemplateDialog", "New Template", 0));
        okButton->setText(QApplication::translate("NewTemplateDialog", "OK", 0));
        cancelButton->setText(QApplication::translate("NewTemplateDialog", "Cancel", 0));
        label->setText(QApplication::translate("NewTemplateDialog", "Template Name", 0));
    } // retranslateUi

};

namespace Ui {
    class NewTemplateDialog: public Ui_NewTemplateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWTEMPLATEDIALOG_H
