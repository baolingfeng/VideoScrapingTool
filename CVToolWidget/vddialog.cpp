#include "vddialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QProgressBar>
#include "util.h"
#include <direct.h>  
#include <io.h>
#include "vdthread.h"

VDDialog::VDDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(openVideo()));
	connect(ui.startbtn, SIGNAL(clicked()), this, SLOT(startVideoDiff()));
	connect(ui.cancelbtn, SIGNAL(clicked()), this, SLOT(closeDialog()));
	//connect(this, SIGNAL(indexOfFrame(int)), this, SLOT(setProgressBar(int)));
}

VDDialog::~VDDialog()
{

}

void VDDialog::openVideo()
{
	QFileDialog *fileDialog = new QFileDialog(this, "Open Video", QDir::currentPath(), "videos (*.flv *.avi *.mov)");
	fileDialog->setDirectory("D:/baolingfeng/Dropbox/ICSE2015Demo - scvRipper");

	if(fileDialog->exec() == QDialog::Accepted) 
	{
		QString path = fileDialog->selectedFiles()[0];
		ui.vpedit->setText(path);

		int i = path.lastIndexOf("/");
		QString before = path.left(i);
		QString after = path.mid(i+1);

		int j = after.indexOf(".");
		QString name = after.left(j);

		ui.outedit->setText(before + "/" + name);

		this->setGeometry(QRect(x(), y(), width(), 200));

		cv::VideoCapture cap(path.toStdString());
		if(!cap.isOpened())
		{
			qDebug()<<"video open fail";
			return;
		}

		int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
		double fps = cap.get(CV_CAP_PROP_FPS);
		double totaltime = frameCount / fps;
		int hour = totaltime / 3600;
		int minute = totaltime  / 60 - 60 * hour;
		int second = totaltime - minute * 60 - 3600 * hour;
		cap.release();
		
		ui.progressBar->setRange(0, frameCount);
		ui.fpslabel->setText(QString::number(fps));
		ui.ttlabel->setText(QString::number(hour) + "h " + QString::number(minute) + "m " + QString::number(second) + "s");

		ui.startbtn->setEnabled(true);
	}
}

void VDDialog::startVideoDiff()
{
	QString path = ui.vpedit->text();
	QString outputFolder = ui.outedit->text();
	double thre = ui.threbox->value();

	VDThread* thread = new VDThread(path, outputFolder, thre);
	//this->moveToThread(thread);

	this->connect(thread, SIGNAL(indexOfFrame(int)), SLOT(setProgressBar(int)));

	thread->start();

	this->setGeometry(QRect(x(), y(), width(), 230));
}

void VDDialog::setProgressBar(int index)
{
	ui.progressBar->setValue(index);
}

void VDDialog::doDiff()
{
	
	
}

void VDDialog::closeDialog()
{
	this->close();
}