#include "scvripper.h"
#include <io.h>
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include "FrameProcesser.h"
#include "AppTpl.h"
#include "util.h"
#include "winconfig.h"
#include "SettingDialog.h"

ScvRipper::ScvRipper(QObject *parent)
	: QMainWindow()
{
	ui.setupUi(this);
	
	originalImgLabel = new ClickableLabel(this);
	originalImgLabel->setText("Distinct-Content Image");
	originalImgLabel->setGeometry(QRect(170, 25, 400, 320));
	edgeLabel = new ClickableLabel(this);
	edgeLabel->setText("Line Map");
	edgeLabel->setGeometry(QRect(590, 25, 400, 320));
	vcLabel = new ClickableLabel(this);
	vcLabel->setText("Detected Visual Cues");
	vcLabel->setGeometry(QRect(170, 380, 400, 320));
	extractedLabel = new ClickableLabel(this);
	extractedLabel->setText("Detected Window Boundaries");
	extractedLabel->setGeometry(QRect(590, 380, 400, 320));

	 QFont font;
     font.setPointSize(14);
     font.setBold(true);
     font.setWeight(75);
     originalImgLabel->setFont(font);
     originalImgLabel->setAlignment(Qt::AlignCenter);
	 edgeLabel->setFont(font);
     edgeLabel->setAlignment(Qt::AlignCenter);
	 vcLabel->setFont(font);
     vcLabel->setAlignment(Qt::AlignCenter);
	 extractedLabel->setFont(font);
     extractedLabel->setAlignment(Qt::AlignCenter);

	numOfFrame = 0;
	tplFolder = "../tpls/std_large";
	outputFolder = "D:/temp/output/";

	core = new CoreProcess;

	QStandardItemModel *model = new QStandardItemModel ;
	ui.imagesview->setModel(model);

	connect(ui.actionLoad_Images, SIGNAL(triggered()), this, SLOT(openImages())); 
	connect(ui.actionRun, SIGNAL(triggered()), this, SLOT(run())); 
	connect(ui.actionSetting, SIGNAL(triggered()), this, SLOT(setting()));
	connect(ui.imagesview->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,  const QItemSelection&)), this,  SLOT(selectImage(const QItemSelection&,  const QItemSelection&)));
	connect(core, SIGNAL(frameStart(int)), this, SLOT(frameStart(int)));
	connect(core, SIGNAL(edgeFinish(int)), this, SLOT(edgeFinish(int)));
	connect(core, SIGNAL(vcFinish(int)), this, SLOT(vcFinish(int)));
	connect(core, SIGNAL(allFinish(int)), this, SLOT(allFinish(int)));

	connect(originalImgLabel, SIGNAL(clicked()), this, SLOT(showOriginalImage()));
	connect(edgeLabel, SIGNAL(clicked()), this, SLOT(showEdgeMap()));
	connect(vcLabel, SIGNAL(clicked()), this, SLOT(showVisualCues()));
	connect(extractedLabel, SIGNAL(clicked()), this, SLOT(showExtracted()));
	
	connect(ui.actionVideo_Differencing, SIGNAL(triggered()), this, SLOT(showVDDialog()));
	connect(ui.actionTemplate_Configuration, SIGNAL(triggered()), this, SLOT(showTplConfig()));
}

ScvRipper::~ScvRipper()
{

}

void ScvRipper::openImages()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Load Image Sequence")); 
	fileDialog->setFileMode(QFileDialog::Directory);
	fileDialog->setDirectory("D:/baolingfeng/Dropbox/ICSE Scvipper Demo");

	if(fileDialog->exec() == QDialog::Accepted) 
	{
		imageFolder = fileDialog->selectedFiles()[0];

		QString temp = imageFolder + "/img-*.png";

		_finddata_t fileInfo;
		long lfDir;
		if((lfDir=_findfirst(temp.toStdString().c_str(),&fileInfo)) == -1)
		{
			qDebug()<<"Load Image Error";
		}
		else
		{
			do{
				numOfFrame++;
				QString fileName(fileInfo.name);
				imglist.push_back(fileName);
				FinishStore fs;
				finish_map[fileName.toStdString()] = fs;

				QStandardItemModel* model = (QStandardItemModel*)ui.imagesview->model();
				QStandardItem* item = new QStandardItem(fileName);
				model->appendRow(item);

			}while( _findnext( lfDir, &fileInfo ) == 0 );
		}
	}
}

void ScvRipper::selectImage(const QItemSelection& selected,  const QItemSelection& deselected)
{
	if(selected.indexes().isEmpty()) return;

	QModelIndex index = selected.indexes().first();
	QString name = index.data(Qt::DisplayRole).toString();

	int row = index.row();

	QString imagePath = this->imageFolder + "/" + name;
	QImage orignalImage;
	orignalImage.load(imagePath);

	QImage dispImg = orignalImage.scaled(originalImgLabel->width(), originalImgLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);
	originalImgLabel->setPixmap(QPixmap::fromImage(dispImg));

	QString edgePath = this->outputFolder + QString::number(row+1) + "_c_edge.png";
	QImage edgeImage(edgePath);
	if(!edgeImage.isNull() && finish_map[name.toStdString()].f1)
	{
		dispImg = edgeImage.scaled(edgeLabel->width(), edgeLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);
		edgeLabel->setPixmap(QPixmap::fromImage(dispImg));
	}
	else
	{
		edgeLabel->clear();
	}

	QString vcPath = this->outputFolder + QString::number(row+1) + "_visualcues.png";
	QImage vcImage(vcPath);
	if(!vcImage.isNull() && finish_map[name.toStdString()].f2)
	{
		dispImg = vcImage.scaled(vcLabel->width(), vcLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);
		vcLabel->setPixmap(QPixmap::fromImage(dispImg));
	}
	else
	{
		vcLabel->clear();
	}

	QString exPath = this->outputFolder + QString::number(row+1) + "_frame.png";
	QImage exImage(exPath);
	if(!exImage.isNull() && finish_map[name.toStdString()].f3)
	{
		dispImg = exImage.scaled(extractedLabel->width(), extractedLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);
		extractedLabel->setPixmap(QPixmap::fromImage(dispImg));
	}
	else
	{
		extractedLabel->clear();
	}
}

void ScvRipper::run()
{
	qDebug()<<"run....."<<endl;
	core->init(tplFolder, imageFolder+"/", outputFolder, numOfFrame);
	core->start();
	
}

void ScvRipper::frameStart(int frameNum)
{
	char temp[20];
	sprintf(temp,"img-%05d.png", frameNum);
	QString imgName(temp);

	QStandardItemModel* model = (QStandardItemModel*)ui.imagesview->model() ;

	if(imglist[frameNum-1] == imgName)
	{
		model->item(frameNum-1, 0)->setData(QColor("red"), Qt::ForegroundRole);
		ui.imagesview->setCurrentIndex(model->index(frameNum-1, 0));
	}

	
	/*
	for ( int i = 0 ; i < model->rowCount() ; ++i )
	{
	  QString label = model->item( i, 0 )->data( Qt::DisplayRole ).toString() ;

	  if(label == imgName)
	  {
		  model->item(i, 0)->setData(QColor("red"), Qt::ForegroundRole);
	  }
	}
	*/
}

void ScvRipper::edgeFinish(int frameNum)
{
	QString path = outputFolder + QString::number(frameNum) + "_c_edge.png"; 
	QImage edgeMap(path);

	QImage qimg = edgeMap.scaled(edgeLabel->width(), edgeLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);

	edgeLabel->setPixmap(QPixmap::fromImage(qimg));

	QString imgName = imglist[frameNum-1];
	finish_map[imgName.toStdString()].f1 = true;
}

void ScvRipper::vcFinish(int frameNum)
{
	QString path = outputFolder + QString::number(frameNum) + "_visualcues.png"; 
	QImage edgeMap(path);

	QImage qimg = edgeMap.scaled(vcLabel->width(), vcLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);

	vcLabel->setPixmap(QPixmap::fromImage(qimg));

	QString imgName = imglist[frameNum-1];
	finish_map[imgName.toStdString()].f2 = true;
}

void ScvRipper::allFinish(int frameNum)
{
	QString path = outputFolder + QString::number(frameNum) + "_frame.png"; 
	QImage result(path);

	QImage qimg = result.scaled(extractedLabel->width(), extractedLabel->height(), Qt::AspectRatioMode::KeepAspectRatio);

	extractedLabel->setPixmap(QPixmap::fromImage(qimg));

	QStandardItemModel* model = (QStandardItemModel*)ui.imagesview->model() ;
	model->item(frameNum-1, 0)->setData(QColor("blue"), Qt::ForegroundRole);

	QString imgName = imglist[frameNum-1];
	finish_map[imgName.toStdString()].f3 = true;
}

void ScvRipper::showOriginalImage()
{
	QStandardItemModel* model = (QStandardItemModel*)ui.imagesview->model() ;
	
	QString name = ui.imagesview->currentIndex().data(Qt::DisplayRole).toString();
	QString path = imageFolder + "/" + name;

	cv::Mat mat = cv::imread(path.toStdString());
	cv::imshow("img", mat);
	cv::waitKey(0);
}

void ScvRipper::showEdgeMap()
{
	int row = ui.imagesview->currentIndex().row();
	QString path = outputFolder + QString::number(row+1) + "_c_edge.png";

	cv::Mat mat = cv::imread(path.toStdString());
	cv::imshow("img", mat);
	cv::waitKey(0);
}

void ScvRipper::showVisualCues()
{
	int row = ui.imagesview->currentIndex().row();
	QString path = outputFolder + QString::number(row+1) + "_visualcues.png";

	cv::Mat mat = cv::imread(path.toStdString());
	cv::imshow("img", mat);
	cv::waitKey(0);
}

void ScvRipper::showExtracted()
{
	int row = ui.imagesview->currentIndex().row();
	QString path = outputFolder + QString::number(row+1) + "_frame.png";

	cv::Mat mat = cv::imread(path.toStdString());
	cv::imshow("img", mat);
	cv::waitKey(0);
}

void ScvRipper::showVDDialog()
{
	VDDialog* d = new VDDialog();
	d->show();
}

void ScvRipper::showTplConfig()
{
	WinConfig* c = new WinConfig;
	c->show();
}

void ScvRipper::setting()
{
	SettingDialog* s = new SettingDialog(this);
	s->show();
}