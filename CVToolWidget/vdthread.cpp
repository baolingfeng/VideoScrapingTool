#include "vdthread.h"
#include <io.h>
#include <direct.h>
#include "util.h"
#include <QDebug>

VDThread::VDThread(QString videopath, QString outputFolder, double thre, QWidget *parent)
	: QThread(parent)
{
	this->videopath = videopath;
	this->outputFolder = outputFolder;
	//this->thre = thre;
	this->thre = 0.9; //demo hard-code
}

VDThread::~VDThread()
{

}

void VDThread::run()
{
	cv::VideoCapture cap(videopath.toStdString());
	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	for(int i=0; i<5; i++)
	{
		sleep(1);
		emit indexOfFrame(frameCount/5 * i); 
	}
	
	emit indexOfFrame(frameCount); 

	/*
	cv::VideoCapture cap(videopath.toStdString());
	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);

	if(_access(outputFolder.toStdString().c_str(), 0) == -1)
	{
		_mkdir(outputFolder.toStdString().c_str());
	}
	
	cv::Mat preFrame;
	int num = 1;
	int count = 1;
	int index = 0;
	while(index<frameCount)
	{
		cv::Mat frame;
		cap.read(frame);
        //cap >> frame;
		index++;
		if(index % 10 == 0)
		{
			qDebug()<<index;
			emit indexOfFrame(index);
		}

		if(frame.empty()) { 
			//emit indexOfFrame(index); 
			continue; 
		}
		
		if(!preFrame.empty())
		{
			cv::Mat c1[3];
			cv::Mat c2[3];

			split(frame,c1);
			split(preFrame,c2);

			double B = corr2(c1[0],c2[0]);
			double G = corr2(c1[1],c2[1]);
			double R = corr2(c1[2],c2[2]);
			if(B < thre || G < thre || R < thre)
			{
				char temp[20];
				sprintf(temp, "img-%05d.png", count);
				QString imgPath = outputFolder + "/" + QString(temp);

				imwrite(imgPath.toStdString(), preFrame);
				frame.copyTo(preFrame);

				count++;
				num = 1;
			}
			else
			{
				num++;
			}
		}
		else
		{
			frame.copyTo(preFrame);
		}
	}

	emit indexOfFrame(index); 
	char temp[20];
	sprintf(temp, "img-%05d.png", count);
	QString imgPath = outputFolder + "/" + QString(temp);

	imwrite(imgPath.toStdString(), preFrame);
	*/
}