/********************************************************************************
** Form generated from reading UI file 'TplTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TPLTOOL_H
#define UI_TPLTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TplWindow
{
public:
    QAction *actionNew_Template;
    QAction *actionNew_Application;
    QAction *actionNewApplicationCopy;
    QAction *actionAddBefore;
    QAction *actionDelete_A_row;
    QAction *actionNew_Template_Copy;
    QAction *actionDelete_Template;
    QAction *actionAdd_After;
    QAction *actionDeleteApp;
    QAction *actionAddIcon;
    QAction *actionDeleteIcon;
    QAction *actionAdd_Mask;
    QWidget *centralwidget;
    QListView *listTpl;
    QLabel *label;
    QListView *listApp;
    QLabel *label_2;
    QListView *ListRow;
    QLabel *label_3;
    QListView *listIcon;
    QLabel *label_4;
    QLabel *labelIcon;
    QLabel *maskLabel;
    QLabel *label_5;
    QLineEdit *editContentRow;
    QLabel *label_6;
    QLineEdit *editUrlRow;
    QPushButton *btnAddItem;
    QPushButton *btnDelItem;
    QMenuBar *menuBar;
    QMenu *menuTemplates;
    QMenu *menuTool;
    QMenu *menuRow;
    QMenu *menuIcon;

    void setupUi(QMainWindow *TplWindow)
    {
        if (TplWindow->objectName().isEmpty())
            TplWindow->setObjectName(QStringLiteral("TplWindow"));
        TplWindow->resize(782, 408);
        actionNew_Template = new QAction(TplWindow);
        actionNew_Template->setObjectName(QStringLiteral("actionNew_Template"));
        actionNew_Application = new QAction(TplWindow);
        actionNew_Application->setObjectName(QStringLiteral("actionNew_Application"));
        actionNewApplicationCopy = new QAction(TplWindow);
        actionNewApplicationCopy->setObjectName(QStringLiteral("actionNewApplicationCopy"));
        actionAddBefore = new QAction(TplWindow);
        actionAddBefore->setObjectName(QStringLiteral("actionAddBefore"));
        actionDelete_A_row = new QAction(TplWindow);
        actionDelete_A_row->setObjectName(QStringLiteral("actionDelete_A_row"));
        actionNew_Template_Copy = new QAction(TplWindow);
        actionNew_Template_Copy->setObjectName(QStringLiteral("actionNew_Template_Copy"));
        actionDelete_Template = new QAction(TplWindow);
        actionDelete_Template->setObjectName(QStringLiteral("actionDelete_Template"));
        actionAdd_After = new QAction(TplWindow);
        actionAdd_After->setObjectName(QStringLiteral("actionAdd_After"));
        actionDeleteApp = new QAction(TplWindow);
        actionDeleteApp->setObjectName(QStringLiteral("actionDeleteApp"));
        actionAddIcon = new QAction(TplWindow);
        actionAddIcon->setObjectName(QStringLiteral("actionAddIcon"));
        actionDeleteIcon = new QAction(TplWindow);
        actionDeleteIcon->setObjectName(QStringLiteral("actionDeleteIcon"));
        actionAdd_Mask = new QAction(TplWindow);
        actionAdd_Mask->setObjectName(QStringLiteral("actionAdd_Mask"));
        centralwidget = new QWidget(TplWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        listTpl = new QListView(centralwidget);
        listTpl->setObjectName(QStringLiteral("listTpl"));
        listTpl->setGeometry(QRect(20, 40, 151, 251));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 121, 16));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        listApp = new QListView(centralwidget);
        listApp->setObjectName(QStringLiteral("listApp"));
        listApp->setGeometry(QRect(190, 40, 151, 251));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(190, 20, 81, 16));
        label_2->setFont(font);
        ListRow = new QListView(centralwidget);
        ListRow->setObjectName(QStringLiteral("ListRow"));
        ListRow->setGeometry(QRect(370, 40, 151, 131));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(370, 20, 46, 13));
        label_3->setFont(font);
        listIcon = new QListView(centralwidget);
        listIcon->setObjectName(QStringLiteral("listIcon"));
        listIcon->setGeometry(QRect(540, 40, 141, 251));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(540, 20, 46, 13));
        label_4->setFont(font);
        labelIcon = new QLabel(centralwidget);
        labelIcon->setObjectName(QStringLiteral("labelIcon"));
        labelIcon->setGeometry(QRect(700, 40, 46, 41));
        maskLabel = new QLabel(centralwidget);
        maskLabel->setObjectName(QStringLiteral("maskLabel"));
        maskLabel->setGeometry(QRect(710, 130, 46, 41));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(370, 190, 71, 16));
        editContentRow = new QLineEdit(centralwidget);
        editContentRow->setObjectName(QStringLiteral("editContentRow"));
        editContentRow->setEnabled(false);
        editContentRow->setGeometry(QRect(450, 190, 71, 20));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(370, 230, 46, 13));
        editUrlRow = new QLineEdit(centralwidget);
        editUrlRow->setObjectName(QStringLiteral("editUrlRow"));
        editUrlRow->setEnabled(false);
        editUrlRow->setGeometry(QRect(450, 230, 71, 20));
        btnAddItem = new QPushButton(centralwidget);
        btnAddItem->setObjectName(QStringLiteral("btnAddItem"));
        btnAddItem->setGeometry(QRect(540, 300, 61, 23));
        btnDelItem = new QPushButton(centralwidget);
        btnDelItem->setObjectName(QStringLiteral("btnDelItem"));
        btnDelItem->setGeometry(QRect(610, 300, 61, 23));
        TplWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(TplWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 782, 21));
        menuTemplates = new QMenu(menuBar);
        menuTemplates->setObjectName(QStringLiteral("menuTemplates"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuRow = new QMenu(menuBar);
        menuRow->setObjectName(QStringLiteral("menuRow"));
        menuIcon = new QMenu(menuBar);
        menuIcon->setObjectName(QStringLiteral("menuIcon"));
        TplWindow->setMenuBar(menuBar);

        menuBar->addAction(menuTemplates->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuRow->menuAction());
        menuBar->addAction(menuIcon->menuAction());
        menuTemplates->addAction(actionNew_Template);
        menuTemplates->addAction(actionNew_Template_Copy);
        menuTemplates->addAction(actionDelete_Template);
        menuTool->addAction(actionNew_Application);
        menuTool->addAction(actionNewApplicationCopy);
        menuTool->addAction(actionDeleteApp);
        menuRow->addAction(actionAddBefore);
        menuRow->addAction(actionAdd_After);
        menuRow->addAction(actionDelete_A_row);
        menuIcon->addAction(actionAddIcon);
        menuIcon->addAction(actionDeleteIcon);
        menuIcon->addAction(actionAdd_Mask);

        retranslateUi(TplWindow);

        QMetaObject::connectSlotsByName(TplWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TplWindow)
    {
        TplWindow->setWindowTitle(QApplication::translate("TplWindow", "MainWindow", 0));
        actionNew_Template->setText(QApplication::translate("TplWindow", "New", 0));
        actionNew_Application->setText(QApplication::translate("TplWindow", "New", 0));
        actionNewApplicationCopy->setText(QApplication::translate("TplWindow", "New (Copy)", 0));
        actionAddBefore->setText(QApplication::translate("TplWindow", "Add Before", 0));
        actionDelete_A_row->setText(QApplication::translate("TplWindow", "Delete", 0));
        actionNew_Template_Copy->setText(QApplication::translate("TplWindow", "New (Copy)", 0));
        actionDelete_Template->setText(QApplication::translate("TplWindow", "Delete", 0));
        actionAdd_After->setText(QApplication::translate("TplWindow", "Add After", 0));
        actionDeleteApp->setText(QApplication::translate("TplWindow", "Delete", 0));
        actionAddIcon->setText(QApplication::translate("TplWindow", "Add", 0));
        actionDeleteIcon->setText(QApplication::translate("TplWindow", "Delete", 0));
        actionAdd_Mask->setText(QApplication::translate("TplWindow", "Add Mask", 0));
        label->setText(QApplication::translate("TplWindow", "Existing Templates", 0));
        label_2->setText(QApplication::translate("TplWindow", "Applications", 0));
        label_3->setText(QApplication::translate("TplWindow", "Rows", 0));
        label_4->setText(QApplication::translate("TplWindow", "Icons", 0));
        labelIcon->setText(QString());
        maskLabel->setText(QString());
        label_5->setText(QApplication::translate("TplWindow", "Content Row", 0));
        label_6->setText(QApplication::translate("TplWindow", "Url Row", 0));
        btnAddItem->setText(QApplication::translate("TplWindow", "Add", 0));
        btnDelItem->setText(QApplication::translate("TplWindow", "Delete", 0));
        menuTemplates->setTitle(QApplication::translate("TplWindow", "Templates", 0));
        menuTool->setTitle(QApplication::translate("TplWindow", "Application", 0));
        menuRow->setTitle(QApplication::translate("TplWindow", "Row", 0));
        menuIcon->setTitle(QApplication::translate("TplWindow", "Icon", 0));
    } // retranslateUi

};

namespace Ui {
    class TplWindow: public Ui_TplWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TPLTOOL_H
