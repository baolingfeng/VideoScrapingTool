/********************************************************************************
** Form generated from reading UI file 'toolwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLWINDOW_H
#define UI_TOOLWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UI_ToolWindow
{
public:
    QAction *actionOpen_Image;
    QAction *actionQuit;
    QAction *actionMatch;
    QAction *actionOpen_Image_Folder;
    QAction *actionOpen_Video;
    QWidget *centralwidget;
    QPushButton *btnSave;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *labelImageStart;
    QSlider *hsliderImgCount;
    QLabel *labelImgCount;
    QPushButton *btnLastFrame;
    QPushButton *btnNextFrame;
    QSpinBox *spinGo;
    QPushButton *btnGo;
    QWidget *subContainer;
    QPushButton *btnLeft;
    QPushButton *btnRight;
    QSpinBox *spinHSub;
    QLabel *label;
    QLineEdit *editImgWidth;
    QLabel *label_2;
    QLineEdit *editImgHeight;
    QPushButton *btnUp;
    QPushButton *btnDown;
    QSpinBox *spinVSub;
    QMenuBar *menubar;
    QMenu *menuOpen_Image;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *UI_ToolWindow)
    {
        if (UI_ToolWindow->objectName().isEmpty())
            UI_ToolWindow->setObjectName(QStringLiteral("UI_ToolWindow"));
        UI_ToolWindow->resize(1000, 640);
        actionOpen_Image = new QAction(UI_ToolWindow);
        actionOpen_Image->setObjectName(QStringLiteral("actionOpen_Image"));
        actionQuit = new QAction(UI_ToolWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionMatch = new QAction(UI_ToolWindow);
        actionMatch->setObjectName(QStringLiteral("actionMatch"));
        actionOpen_Image_Folder = new QAction(UI_ToolWindow);
        actionOpen_Image_Folder->setObjectName(QStringLiteral("actionOpen_Image_Folder"));
        actionOpen_Video = new QAction(UI_ToolWindow);
        actionOpen_Video->setObjectName(QStringLiteral("actionOpen_Video"));
        centralwidget = new QWidget(UI_ToolWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        btnSave = new QPushButton(centralwidget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setGeometry(QRect(780, 30, 75, 23));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 701, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelImageStart = new QLabel(horizontalLayoutWidget);
        labelImageStart->setObjectName(QStringLiteral("labelImageStart"));

        horizontalLayout->addWidget(labelImageStart);

        hsliderImgCount = new QSlider(horizontalLayoutWidget);
        hsliderImgCount->setObjectName(QStringLiteral("hsliderImgCount"));
        hsliderImgCount->setEnabled(false);
        hsliderImgCount->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(hsliderImgCount);

        labelImgCount = new QLabel(horizontalLayoutWidget);
        labelImgCount->setObjectName(QStringLiteral("labelImgCount"));

        horizontalLayout->addWidget(labelImgCount);

        btnLastFrame = new QPushButton(horizontalLayoutWidget);
        btnLastFrame->setObjectName(QStringLiteral("btnLastFrame"));
        btnLastFrame->setEnabled(false);

        horizontalLayout->addWidget(btnLastFrame);

        btnNextFrame = new QPushButton(horizontalLayoutWidget);
        btnNextFrame->setObjectName(QStringLiteral("btnNextFrame"));
        btnNextFrame->setEnabled(false);

        horizontalLayout->addWidget(btnNextFrame);

        spinGo = new QSpinBox(horizontalLayoutWidget);
        spinGo->setObjectName(QStringLiteral("spinGo"));
        spinGo->setEnabled(false);

        horizontalLayout->addWidget(spinGo);

        btnGo = new QPushButton(horizontalLayoutWidget);
        btnGo->setObjectName(QStringLiteral("btnGo"));
        btnGo->setEnabled(false);

        horizontalLayout->addWidget(btnGo);

        subContainer = new QWidget(centralwidget);
        subContainer->setObjectName(QStringLiteral("subContainer"));
        subContainer->setGeometry(QRect(60, 470, 381, 91));
        QPalette palette;
        QBrush brush(QColor(0, 0, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        subContainer->setPalette(palette);
        btnLeft = new QPushButton(subContainer);
        btnLeft->setObjectName(QStringLiteral("btnLeft"));
        btnLeft->setGeometry(QRect(200, 10, 41, 23));
        btnRight = new QPushButton(subContainer);
        btnRight->setObjectName(QStringLiteral("btnRight"));
        btnRight->setGeometry(QRect(300, 10, 51, 23));
        spinHSub = new QSpinBox(subContainer);
        spinHSub->setObjectName(QStringLiteral("spinHSub"));
        spinHSub->setGeometry(QRect(250, 10, 41, 21));
        spinHSub->setMinimum(1);
        spinHSub->setMaximum(9999);
        spinHSub->setValue(100);
        label = new QLabel(subContainer);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 71, 16));
        editImgWidth = new QLineEdit(subContainer);
        editImgWidth->setObjectName(QStringLiteral("editImgWidth"));
        editImgWidth->setEnabled(false);
        editImgWidth->setGeometry(QRect(90, 10, 51, 20));
        label_2 = new QLabel(subContainer);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 71, 16));
        editImgHeight = new QLineEdit(subContainer);
        editImgHeight->setObjectName(QStringLiteral("editImgHeight"));
        editImgHeight->setEnabled(false);
        editImgHeight->setGeometry(QRect(90, 40, 51, 20));
        btnUp = new QPushButton(subContainer);
        btnUp->setObjectName(QStringLiteral("btnUp"));
        btnUp->setGeometry(QRect(200, 40, 41, 23));
        btnDown = new QPushButton(subContainer);
        btnDown->setObjectName(QStringLiteral("btnDown"));
        btnDown->setGeometry(QRect(300, 40, 51, 23));
        spinVSub = new QSpinBox(subContainer);
        spinVSub->setObjectName(QStringLiteral("spinVSub"));
        spinVSub->setGeometry(QRect(250, 40, 41, 21));
        spinVSub->setMinimum(1);
        spinVSub->setMaximum(9999);
        spinVSub->setValue(100);
        UI_ToolWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(UI_ToolWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        menuOpen_Image = new QMenu(menubar);
        menuOpen_Image->setObjectName(QStringLiteral("menuOpen_Image"));
        UI_ToolWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(UI_ToolWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        UI_ToolWindow->setStatusBar(statusbar);

        menubar->addAction(menuOpen_Image->menuAction());
        menuOpen_Image->addSeparator();
        menuOpen_Image->addAction(actionOpen_Image_Folder);
        menuOpen_Image->addAction(actionOpen_Video);
        menuOpen_Image->addAction(actionOpen_Image);
        menuOpen_Image->addAction(actionMatch);
        menuOpen_Image->addAction(actionQuit);

        retranslateUi(UI_ToolWindow);

        QMetaObject::connectSlotsByName(UI_ToolWindow);
    } // setupUi

    void retranslateUi(QMainWindow *UI_ToolWindow)
    {
        UI_ToolWindow->setWindowTitle(QApplication::translate("UI_ToolWindow", "MainWindow", 0));
        actionOpen_Image->setText(QApplication::translate("UI_ToolWindow", "Open An Image", 0));
        actionQuit->setText(QApplication::translate("UI_ToolWindow", "Quit", 0));
        actionMatch->setText(QApplication::translate("UI_ToolWindow", "Match", 0));
        actionOpen_Image_Folder->setText(QApplication::translate("UI_ToolWindow", "Open Image Folder", 0));
        actionOpen_Video->setText(QApplication::translate("UI_ToolWindow", "Open Video", 0));
        btnSave->setText(QApplication::translate("UI_ToolWindow", "Save", 0));
        labelImageStart->setText(QApplication::translate("UI_ToolWindow", "0", 0));
        labelImgCount->setText(QApplication::translate("UI_ToolWindow", "99", 0));
        btnLastFrame->setText(QApplication::translate("UI_ToolWindow", "<<", 0));
        btnNextFrame->setText(QApplication::translate("UI_ToolWindow", ">>", 0));
        btnGo->setText(QApplication::translate("UI_ToolWindow", "Go", 0));
        btnLeft->setText(QApplication::translate("UI_ToolWindow", "left", 0));
        btnRight->setText(QApplication::translate("UI_ToolWindow", "right", 0));
        label->setText(QApplication::translate("UI_ToolWindow", "Image Width", 0));
        label_2->setText(QApplication::translate("UI_ToolWindow", "Image Height", 0));
        btnUp->setText(QApplication::translate("UI_ToolWindow", "up", 0));
        btnDown->setText(QApplication::translate("UI_ToolWindow", "down", 0));
        menuOpen_Image->setTitle(QApplication::translate("UI_ToolWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class UI_ToolWindow: public Ui_UI_ToolWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLWINDOW_H
