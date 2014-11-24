#ifndef VDDIALOG_H
#define VDDIALOG_H

#include <QDialog>
#include "ui_videodiff.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

class VDDialog : public QDialog
{
	Q_OBJECT

public:
	VDDialog(QWidget *parent=0);
	~VDDialog();

public slots:
	void openVideo();
	void startVideoDiff();
	void doDiff();
	void setProgressBar(int index);
	void closeDialog();
signals:
	void indexOfFrame(int index);
private:
	Ui::vddialog ui;
};

#endif // VDDIALOG_H
