#include "cvtoolwidget.h"
#include <io.h>
#include <iostream>
#include <QFileDialog>
#include <QProgressDialog>
#include <qdebug.h>
#include <time.h>
#include "ObjMan.h"
#include "WinTpl.h"
#include "AppTpl.h"
#include "EdgeInfo.h"
#include "LogUtil.h"
#include "FrameProcesser.h"
#include "MyFunctions.h"
#include <vector>
#include "configdialog.h"
#include "CProperties.h"
#include "waitdialog.h"

using namespace std;

CVToolWidget::CVToolWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	numOfFrame = 0;
	minIndex = 2e15;
	maxIndex = 0;
	frameStart = 0;
	frameEnd = 0;

	LogUtil::init();

	ui.spinFrom->setMaximum(10000);
	ui.spinTo->setMaximum(10000);

	connect(ui.btnImgFolder,SIGNAL(clicked()),this,SLOT(openImageFolder()));
	connect(ui.btnRun,SIGNAL(clicked()), this, SLOT(run()));
	//connect(ui.btnConfig,SIGNAL(clicked()),this,SLOT(openConfig()));
	connect(ui.btnTplFolder,SIGNAL(clicked()),this,SLOT(openTpl()));
	//createConsole();

	initConfigParam();

	ui.editTplName->setText("standard");
	//AppTpl::test();
	//FrameProcesser::test();
	//EdgeInfo::test();

	LogUtil::info("Welcome ");

}

CVToolWidget::~CVToolWidget()
{
	
}


void CVToolWidget::openConfig()
{
	ConfigDialog* configDialog = new ConfigDialog(this);
	
	if(configDialog->exec() == QDialog::Accepted)
	{
		config.FAST_CORNER_THRESH = configDialog->fastCornerThreshold;
	}
}

void CVToolWidget::openTpl()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Template Folder")); 
	fileDialog->setFileMode(QFileDialog::Directory);
	if(fileDialog->exec() == QDialog::Accepted) 
	{
		QString folder = fileDialog->selectedFiles()[0];
		ui.editTplFolder->setText(folder);
		LogUtil::info("Open Template Folder: " + folder.toStdString());
	}
}

void CVToolWidget::openImageFolder()
{
	
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Image Folder")); 
	fileDialog->setFileMode(QFileDialog::Directory);

	if(fileDialog->exec() == QDialog::Accepted) 
	{ 
		QString folder = fileDialog->selectedFiles()[0];
		ui.editImgFolder->setText(folder);
		LogUtil::info("Open Image Folder: " + folder.toStdString());

		QString temp = folder + "/img-*.png";

		numOfFrame = 0;
		_finddata_t fileInfo;
		long lfDir;
		if((lfDir=_findfirst(temp.toStdString().c_str(),&fileInfo)) == -1)
		{
			LogUtil::info("not a fold");
		}
		else
		{
			do{
				QString fileName(fileInfo.name);
				int idx = fileName.indexOf("img-");

				int frameIndex = fileName.mid(4,5).toInt();
				if(frameIndex < minIndex)
				{
					minIndex = frameIndex;
				}

				if(frameIndex > maxIndex)
				{
					maxIndex = frameIndex;
				}

				numOfFrame++;
			}while( _findnext( lfDir, &fileInfo ) == 0 );
		}

		ui.editImgNum->setText(QString::number(numOfFrame));
		ui.spinFrom->setValue(minIndex);
		ui.spinFrom->setEnabled(true);
		ui.spinTo->setValue(maxIndex);
		ui.spinTo->setEnabled(true);
	}
}

void CVToolWidget::run()
{
	this->setEnabled(false);

	LogUtil::debug("begin to run....");
	QString outputFolder = ui.editOutputFolder->text() + "/";
	LogUtil::debug("output folder: " + outputFolder.toStdString());

	QString tplFolder = ui.editTplFolder->text();
	AppTpl::BASE_FOLDER = tplFolder.toStdString();

	QString tplName = ui.editTplName->text();
	LogUtil::debug("template name: " + tplName.toStdString());

	QString imgFolder = ui.editImgFolder->text() + "/";
	LogUtil::debug("image folder: " + imgFolder.toStdString());

	vector<AppTpl> tpls;
	AppTpl::loadTemplates(tpls,tplName.toStdString());

	int frame_seq_begin = ui.spinFrom->value();
	int frame_num = ui.spinTo->value() - ui.spinFrom->value() + 1;

	char fileName[255];
	ostringstream ss;
	
	int taskbar = -1;
	time_t stime, etime;
	time(&stime);

	for(int cur_frame=frame_seq_begin; cur_frame<frame_seq_begin+frame_num; cur_frame++)
	{
		ss<<"begin to process "<<cur_frame<<"-th frame\n"; LogUtil::info(ss);
		sprintf(fileName,"img-%05d", cur_frame);

		FrameProcesser processer;
		if(FrameProcesser::TASKBAR<=0 && taskbar>0)
		{
			processer.upper_taskbar = taskbar;
		}

		processer.setOutputFolder(outputFolder.toStdString());

		if(!processer.feedFrame(imgFolder.toStdString(),fileName,cur_frame))
		{
			ss<<cur_frame<<"-th frame doens't exist"; LogUtil::info(ss);
			continue;
		}

		processer.run(tpls);

		if(processer.upper_taskbar > 0 && FrameProcesser::TASKBAR>0 &&
			(processer.upper_taskbar == taskbar || taskbar == -1))
		{
			FrameProcesser::TASKBAR--;
			taskbar = processer.upper_taskbar;
		}
	}
	
	time(&etime);
	ss<<"cost time: "<<etime-stime; LogUtil::info(ss);

	LogUtil::debug("-------- end ------");
	this->setEnabled(true);
}
