/********************************************************************************
** Form generated from reading UI file 'videodiff.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEODIFF_H
#define UI_VIDEODIFF_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_vddialog
{
public:
    QToolButton *toolButton;
    QLineEdit *vpedit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *outedit;
    QPushButton *startbtn;
    QPushButton *cancelbtn;
    QFrame *frame;
    QLabel *label_3;
    QLabel *fpslabel;
    QLabel *label_4;
    QLabel *ttlabel;
    QProgressBar *progressBar;
    QLabel *label_5;
    QDoubleSpinBox *threbox;

    void setupUi(QDialog *vddialog)
    {
        if (vddialog->objectName().isEmpty())
            vddialog->setObjectName(QStringLiteral("vddialog"));
        vddialog->resize(509, 148);
        toolButton = new QToolButton(vddialog);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(430, 10, 25, 19));
        vpedit = new QLineEdit(vddialog);
        vpedit->setObjectName(QStringLiteral("vpedit"));
        vpedit->setEnabled(false);
        vpedit->setGeometry(QRect(90, 10, 331, 20));
        label = new QLabel(vddialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 61, 20));
        label_2 = new QLabel(vddialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 71, 16));
        outedit = new QLineEdit(vddialog);
        outedit->setObjectName(QStringLiteral("outedit"));
        outedit->setGeometry(QRect(90, 50, 331, 20));
        startbtn = new QPushButton(vddialog);
        startbtn->setObjectName(QStringLiteral("startbtn"));
        startbtn->setGeometry(QRect(320, 120, 75, 23));
        cancelbtn = new QPushButton(vddialog);
        cancelbtn->setObjectName(QStringLiteral("cancelbtn"));
        cancelbtn->setGeometry(QRect(410, 120, 75, 23));
        frame = new QFrame(vddialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 160, 481, 31));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 10, 46, 13));
        fpslabel = new QLabel(frame);
        fpslabel->setObjectName(QStringLiteral("fpslabel"));
        fpslabel->setGeometry(QRect(50, 10, 81, 13));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(150, 10, 46, 13));
        ttlabel = new QLabel(frame);
        ttlabel->setObjectName(QStringLiteral("ttlabel"));
        ttlabel->setGeometry(QRect(210, 10, 171, 13));
        progressBar = new QProgressBar(vddialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 200, 491, 23));
        progressBar->setValue(0);
        label_5 = new QLabel(vddialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 90, 71, 16));
        threbox = new QDoubleSpinBox(vddialog);
        threbox->setObjectName(QStringLiteral("threbox"));
        threbox->setGeometry(QRect(90, 90, 201, 22));
        threbox->setDecimals(1);
        threbox->setMaximum(1);
        threbox->setValue(0.7);

        retranslateUi(vddialog);

        QMetaObject::connectSlotsByName(vddialog);
    } // setupUi

    void retranslateUi(QDialog *vddialog)
    {
        vddialog->setWindowTitle(QApplication::translate("vddialog", "Video Differencing", 0));
        toolButton->setText(QApplication::translate("vddialog", "...", 0));
        label->setText(QApplication::translate("vddialog", "Video Path:", 0));
        label_2->setText(QApplication::translate("vddialog", "Output Folder:", 0));
        startbtn->setText(QApplication::translate("vddialog", "Start", 0));
        cancelbtn->setText(QApplication::translate("vddialog", "Cancel", 0));
        label_3->setText(QApplication::translate("vddialog", "FPS:", 0));
        fpslabel->setText(QString());
        label_4->setText(QApplication::translate("vddialog", "Time:", 0));
        ttlabel->setText(QString());
        label_5->setText(QApplication::translate("vddialog", "Threshold:", 0));
    } // retranslateUi

};

namespace Ui {
    class vddialog: public Ui_vddialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEODIFF_H
