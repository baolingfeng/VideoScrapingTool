#ifndef SCVRIPPER_H
#define SCVRIPPER_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "coreprocess.h"
#include <QStringList>
#include "clickablelabel.h"
#include "vddialog.h"
#include <hash_map>

using namespace std;

class FinishStore
{
public:
	FinishStore() { f1 = f2 = f3 = false;}
	~FinishStore() {}

	bool f1;
	bool f2;
	bool f3;
};

class ScvRipper : public QMainWindow
{
	Q_OBJECT

public:
	ScvRipper(QObject *parent=0);
	~ScvRipper();

public slots:
	void openImages();
	void selectImage(const QItemSelection&,  const QItemSelection&);
	void setting();
	void run();

	void frameStart(int frameNum);
	void edgeFinish(int frameNum);
	void vcFinish(int frameNum);
	void allFinish(int frameNum);

	void showOriginalImage();
	void showEdgeMap();
	void showVisualCues();
	void showExtracted();

	void showVDDialog();
	void showTplConfig();
private:
	int numOfFrame;
	QString imageFolder;
	QString tplFolder;
	QString outputFolder;

	QStringList imglist;
	hash_map<string, FinishStore> finish_map;

	CoreProcess* core;

	ClickableLabel* originalImgLabel;
	ClickableLabel* edgeLabel;
	ClickableLabel* vcLabel;
	ClickableLabel* extractedLabel;
	Ui::MainWindow ui;
	

};

#endif // SCVRIPPER_H
