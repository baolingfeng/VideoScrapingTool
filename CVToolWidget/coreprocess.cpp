#include "coreprocess.h"
#include "FrameProcesser.h"
#include "AppTpl.h"
#include <QDebug>


CoreProcess::CoreProcess(QObject *parent)
	: QThread(parent)
{

}

CoreProcess::~CoreProcess()
{

}

void CoreProcess::init(QString tplFolder, QString imgFolder, QString outputFolder, int numOfFrame)
{
	this->tplFolder = tplFolder;
	this->imgFolder = imgFolder;
	this->outputFolder = outputFolder;
	this->numOfFrame = numOfFrame;
}

void CoreProcess::run()
{
	vector<AppTpl> tpls;

	AppTpl::loadTemplatesFromFS(tpls, tplFolder.toStdString());

	char fileName[255];
	int taskbar = -1;
	int frame_seq_begin = 1;
	for(int cur_frame=frame_seq_begin; cur_frame<frame_seq_begin+numOfFrame; cur_frame++)
	{
		sprintf(fileName,"img-%05d", cur_frame);
		FrameProcesser processer;
		if(FrameProcesser::TASKBAR<=0 && taskbar>0)
		{
			processer.upper_taskbar = taskbar;
		}

		processer.setOutputFolder(outputFolder.toStdString());

		emit frameStart(cur_frame);

		/*
		if(!processer.feedFrame(imgFolder.toStdString(),fileName,cur_frame))
		{
			qDebug()<<cur_frame<<"-th frame doens't exist"; 
			continue;
		}
		*/

		this->sleep(2);
		//processer.collectEdgeInfo();
		emit edgeFinish(cur_frame);

		//processer.matchTplCompsToFrame(tpls);
		this->sleep(2);
		emit vcFinish(cur_frame);

		//processer.groupInstance(tpls);
		//processer.getBoundary();
		//processer.crop();
		//QString path = outputFolder + QString::number(cur_frame) + "_frame.png";
		//imwrite(path.toStdString(),processer.frame_paint);
		this->sleep(2);
		emit allFinish(cur_frame);

		this->sleep(1);
		/*
		processer.run(tpls);

		if(processer.upper_taskbar > 0 && FrameProcesser::TASKBAR>0 &&
			(processer.upper_taskbar == taskbar || taskbar == -1))
		{
			FrameProcesser::TASKBAR--;
			taskbar = processer.upper_taskbar;
		}
		*/
	}
}