#ifndef VDTHREAD_H
#define VDTHREAD_H

#include <QThread>
#include "ui_videodiff.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

class VDThread : public QThread
{
	Q_OBJECT

public:
	VDThread(QString videopath, QString outputFolder, double thre, QWidget *parent=0);
	~VDThread();

	void run();
signals:
	void indexOfFrame(int);
private:
	QString videopath;
	QString outputFolder;
	double thre;
};

#endif // VDTHREAD_H
