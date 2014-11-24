/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Images;
    QAction *actionSet_Output_Folder;
    QAction *actionTemplate_Folder;
    QAction *actionTemplate_Configuration;
    QAction *actionRun;
    QAction *actionVideo_Differencing;
    QAction *actionSetting;
    QWidget *centralwidget;
    QListView *imagesview;
    QFrame *line;
    QFrame *line_2;
    QMenuBar *menubar;
    QMenu *menuVideo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1021, 756);
        actionLoad_Images = new QAction(MainWindow);
        actionLoad_Images->setObjectName(QStringLiteral("actionLoad_Images"));
        actionSet_Output_Folder = new QAction(MainWindow);
        actionSet_Output_Folder->setObjectName(QStringLiteral("actionSet_Output_Folder"));
        actionTemplate_Folder = new QAction(MainWindow);
        actionTemplate_Folder->setObjectName(QStringLiteral("actionTemplate_Folder"));
        actionTemplate_Configuration = new QAction(MainWindow);
        actionTemplate_Configuration->setObjectName(QStringLiteral("actionTemplate_Configuration"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        actionVideo_Differencing = new QAction(MainWindow);
        actionVideo_Differencing->setObjectName(QStringLiteral("actionVideo_Differencing"));
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName(QStringLiteral("actionSetting"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        imagesview = new QListView(centralwidget);
        imagesview->setObjectName(QStringLiteral("imagesview"));
        imagesview->setGeometry(QRect(0, 0, 161, 711));
        line = new QFrame(centralwidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(170, 330, 841, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(580, 0, 20, 731));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1021, 21));
        menuVideo = new QMenu(menubar);
        menuVideo->setObjectName(QStringLiteral("menuVideo"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuVideo->menuAction());
        menuVideo->addAction(actionVideo_Differencing);
        menuVideo->addAction(actionLoad_Images);
        menuVideo->addAction(actionSetting);
        menuVideo->addAction(actionTemplate_Configuration);
        menuVideo->addAction(actionRun);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "scvRipper", 0));
        actionLoad_Images->setText(QApplication::translate("MainWindow", "Load Images", 0));
        actionSet_Output_Folder->setText(QApplication::translate("MainWindow", "Set Output Folder", 0));
        actionTemplate_Folder->setText(QApplication::translate("MainWindow", "Template Folder", 0));
        actionTemplate_Configuration->setText(QApplication::translate("MainWindow", "Template Configuration", 0));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0));
        actionVideo_Differencing->setText(QApplication::translate("MainWindow", "Video Differencing", 0));
        actionSetting->setText(QApplication::translate("MainWindow", "Setting", 0));
        menuVideo->setTitle(QApplication::translate("MainWindow", "scvRipper", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
