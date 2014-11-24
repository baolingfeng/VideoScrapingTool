#pragma once

#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include <QMainWindow>
#include "ui_toolwindow.h"
#include <QLabel>
#include "MyLabel.h"

using namespace std;

class ToolWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	ToolWindow(QWidget *parent=0);
	~ToolWindow(void);

	QImage getItem();
	QImage readFromVideo(int index);
	QImage readFromFolder(int index);
	void showSubWidgets();

public slots:
	void openImage();
	void closeWindow(bool);
	void nextSubImg();
	void lastSubImg();
	void leftSubImg();
	void rightSubImg();
	void upSubImg();
	void downSubImg();

	void getSelectedRect(QRect rect, int subIndex);
	void match();
	void saveIcon();
	void openVideo();
	void openImageFolder();
	void getFrame(int);
	void nextFrame();
	void lastFrame();
	void gotoFrame();

signals:
	void quit();
	void saveItem();
private:
	Ui::UI_ToolWindow ui;

	MyLabel* label;
	QLabel* subLabel;

	QImage img;
	QImage item;

	int type; //0, 1: one image, 2: image folder, 3: video
	int imageCount;
	int minIndex;
	int maxIndex;
	QString imageFolder;
	cv::VideoCapture cap;
};

