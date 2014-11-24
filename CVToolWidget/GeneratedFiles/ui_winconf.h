/********************************************************************************
** Form generated from reading UI file 'winconf.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINCONF_H
#define UI_WINCONF_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_winconf
{
public:
    QTreeView *treeView;
    QLabel *label_2;
    QTableView *tableView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *winconf)
    {
        if (winconf->objectName().isEmpty())
            winconf->setObjectName(QStringLiteral("winconf"));
        winconf->resize(768, 559);
        treeView = new QTreeView(winconf);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(10, 30, 421, 491));
        treeView->setAnimated(true);
        treeView->setHeaderHidden(true);
        treeView->header()->setVisible(false);
        label_2 = new QLabel(winconf);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 10, 91, 16));
        tableView = new QTableView(winconf);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(450, 30, 301, 411));
        tableView->verticalHeader()->setVisible(false);
        pushButton = new QPushButton(winconf);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 530, 181, 23));
        pushButton_2 = new QPushButton(winconf);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 530, 101, 23));
        pushButton_3 = new QPushButton(winconf);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(450, 450, 101, 23));
        pushButton_4 = new QPushButton(winconf);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(570, 450, 101, 23));

        retranslateUi(winconf);

        QMetaObject::connectSlotsByName(winconf);
    } // setupUi

    void retranslateUi(QWidget *winconf)
    {
        winconf->setWindowTitle(QApplication::translate("winconf", "Window Definition Configuration", 0));
        label_2->setText(QApplication::translate("winconf", "Window Defintion", 0));
        pushButton->setText(QApplication::translate("winconf", "Add Application Window Definition", 0));
        pushButton_2->setText(QApplication::translate("winconf", "Modify", 0));
        pushButton_3->setText(QApplication::translate("winconf", "Add Visual Cue", 0));
        pushButton_4->setText(QApplication::translate("winconf", "Delete Visual Cue", 0));
    } // retranslateUi

};

namespace Ui {
    class winconf: public Ui_winconf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINCONF_H
