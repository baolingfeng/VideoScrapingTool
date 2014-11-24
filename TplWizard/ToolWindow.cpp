#include "ToolWindow.h"
#include <QFileDialog>
#include <QListView>
#include <QMessageBox>
#include <QDebug>
#include <io.h>

ToolWindow::ToolWindow(QWidget *parent)
{
	ui.setupUi(this);

	//img = new QImage;
	
	subLabel = new QLabel(ui.centralwidget);
	subLabel->setGeometry(QRect(800, 100, 200, 200));

	label = new MyLabel;
	label->setGeometry(QRect(0, 40, 800, 600));
	label->setParent(ui.centralwidget);

	//imageLabel->setPixmap(QPixmap::fromImage(*img));
	label->setImage(img);
	label->setText("Image Area");

	//ui.subLabel->setText("");

	type = 0;
	imageCount = 0;
	ui.labelImgCount->setText(QString::number(imageCount));

	ui.subContainer->setVisible(false);

	//ui.actionOpen_Image->trigger()
	connect(ui.actionOpen_Image, SIGNAL(triggered(bool)),this,SLOT(openImage()));
	connect(ui.actionOpen_Video,SIGNAL(triggered(bool)),this,SLOT(openVideo()));
	connect(ui.actionOpen_Image_Folder, SIGNAL(triggered(bool)), this, SLOT(openImageFolder()));

	connect(ui.actionQuit, SIGNAL(triggered(bool)),this,SLOT(closeWindow(bool)));
	connect(ui.actionMatch, SIGNAL(triggered(bool)),this,SLOT(match()));
	connect(ui.btnSave,SIGNAL(clicked()),this,SLOT(saveIcon()));

	connect(ui.btnRight,SIGNAL(clicked()),this,SLOT(rightSubImg()));
	connect(ui.btnLeft,SIGNAL(clicked()),this,SLOT(leftSubImg()));
	connect(ui.btnUp,SIGNAL(clicked()),this,SLOT(upSubImg()));
	connect(ui.btnDown,SIGNAL(clicked()),this,SLOT(downSubImg()));

	connect(label,SIGNAL(rectSelected(QRect,int)),this,SLOT(getSelectedRect(QRect,int)));
	connect(ui.hsliderImgCount,SIGNAL(valueChanged(int)), this, SLOT(getFrame(int)));
	connect(ui.btnNextFrame, SIGNAL(clicked()), this, SLOT(nextFrame()));
	connect(ui.btnLastFrame,SIGNAL(clicked()), this, SLOT(lastFrame()));
	connect(ui.btnGo,SIGNAL(clicked()), this, SLOT(gotoFrame()));
}


ToolWindow::~ToolWindow(void)
{
	if(cap.isOpened())
	{
		cap.release();
	}
}

QImage ToolWindow::getItem()
{
	return item;
}

void ToolWindow::getSelectedRect(QRect rect, int subIndex)
{
	int left,up;
	if(subIndex == 1 )
	{
		left = 0;
		up = 0;
	}
	else if(subIndex == 2)
	{
		left = img.width()/2;
		up = 0;
	}
	else if(subIndex == 3)
	{
		left = 0;
		up = img.height()/2;
	}
	else if(subIndex == 4)
	{
		left = img.width()/2;
		up = img.height()/2;
	}
	QImage subImg = img.copy(left,up,img.width()/2,img.height()/2);

	item = subImg.copy(rect);

	subLabel->setPixmap(QPixmap::fromImage(item));

}

void ToolWindow::showSubWidgets()
{
	QRect geo;
	geo.setX(ui.subContainer->geometry().x());
	int tempy = label->geometry().y() + label->geometry().height();
	geo.setY(tempy + 10);

	geo.setWidth(ui.subContainer->geometry().width());
	geo.setHeight(ui.subContainer->geometry().height());
	ui.subContainer->setGeometry(geo);
	ui.subContainer->setVisible(true);

	ui.editImgWidth->setText(QString::number(img.width()));
	ui.editImgHeight->setText(QString::number(img.height()));
}

void ToolWindow::openImage()
{
	QFileDialog *fileDialog = new QFileDialog(this); 
	
	QListView *listView = fileDialog->findChild<QListView*>("listView");
	if (listView)
		listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    fileDialog->setWindowTitle(tr("Open Image")); 
    fileDialog->setDirectory("."); 
	fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)")); 
    if(fileDialog->exec() == QDialog::Accepted) { 
			type = 1;

            QString path = fileDialog->selectedFiles()[0]; 
			img.load(path);
			subLabel->setPixmap(QPixmap::fromImage(img));

			label->setGeometry(QRect(10,40,img.width()/2, img.height()/2));
			label->setImage(img);
			label->update();

			ui.hsliderImgCount->setEnabled(false);
			ui.btnGo->setEnabled(false);
			ui.btnNextFrame->setEnabled(false);
			ui.btnLastFrame->setEnabled(false);
			ui.spinGo->setEnabled(false);

			showSubWidgets();
           // QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path); 
    } else { 
            //QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files.")); 
    }
}

QImage ToolWindow::readFromVideo(int index)
{
	if(!cap.isOpened())
	{
		qDebug()<<"video isn't opened";
	}

	cap.set(CV_CAP_PROP_POS_FRAMES,index);

	cv::Mat frame;
	cap>>frame;

	QImage::Format format;
	switch(frame.type())
	{
		case CV_8UC4:
			format = QImage::Format_RGB32; break;
		case CV_8UC3:
			format = QImage::Format_RGB888; break;
		default:
			qDebug()<<"image format not supported";
			//QMessageBox::information(NULL, tr("Error"), tr("Image format not supported")); 
	}
		
	QImage temp(frame.data, frame.cols, frame.rows, frame.step, format);
	return temp.rgbSwapped();
}

void ToolWindow::openVideo()
{
	QFileDialog *fileDialog = new QFileDialog(this); 
	fileDialog->setWindowTitle(tr("Open Video")); 
    fileDialog->setDirectory("."); 
	fileDialog->setNameFilter(tr("Video Files(*.avi *.flv *.mov)")); 

	if(fileDialog->exec() == QDialog::Accepted) 
	{
		if(cap.isOpened())
		{
			cap.release();
		}

		type = 3;
		QString path = fileDialog->selectedFiles()[0]; 
		cap.open(path.toStdString());
		if(!cap.isOpened())
		{
			QMessageBox::information(NULL, tr("Error"), tr("Open Failed")); 
			return;
		}

		imageCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
		double fps = cap.get(CV_CAP_PROP_FPS);
		int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
		int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
		
		minIndex = 0;
		maxIndex = imageCount-1;

		ui.labelImageStart->setText("0");
		ui.labelImgCount->setText(QString::number(imageCount-1));
		ui.hsliderImgCount->setMaximum(imageCount-1);
		ui.hsliderImgCount->setEnabled(true);

		ui.spinGo->setValue(0);
		ui.spinGo->setMaximum(imageCount);
		ui.spinGo->setEnabled(true);

		ui.btnNextFrame->setEnabled(true);
		ui.btnGo->setEnabled(true);

		img = readFromVideo(0);

		label->setGeometry(QRect(10,40,img.width()/2, img.height()/2));
		label->setImage(img);
		label->update();
		showSubWidgets();
	}
	
}
 
QImage ToolWindow::readFromFolder(int index)
{
	char fileName[256];

	sprintf(fileName, "img-%05d.png", index);

	string imgPath = imageFolder.toStdString() + "/" +string(fileName);

	cv::Mat frame = cv::imread(imgPath);
	QImage temp;

	if(frame.empty())
	{
		qDebug()<<"image doen't exist";
		return temp;
	}

	QImage::Format format;
	switch(frame.type())
	{
		case CV_8UC4:
			format = QImage::Format_RGB32; break;
		case CV_8UC3:
			format = QImage::Format_RGB888; break;
		default:
			qDebug()<<"image format not supported";
			return temp;
	}
		
	temp = QImage(frame.data, frame.cols, frame.rows, frame.step, format);
	return temp.rgbSwapped();
}

void ToolWindow::openImageFolder()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Image Folder")); 
	fileDialog->setFileMode(QFileDialog::Directory);

	if(fileDialog->exec() == QDialog::Accepted) 
	{ 
		type = 2;
		imageFolder = fileDialog->selectedFiles()[0];

		QString temp = imageFolder + "/img-*.png";

		int numOfFrame = 0;
		_finddata_t fileInfo;
		long lfDir;
		minIndex = 99999; 
		maxIndex = 0;
		if((lfDir=_findfirst(temp.toStdString().c_str(),&fileInfo)) == -1)
		{
			qDebug()<<"not images found";
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

		imageCount = maxIndex - minIndex + 1;

		if(numOfFrame != imageCount)
		{
			QMessageBox::information(NULL, tr("Error"), tr("Image Folder has inconsist image number"));
			return;
		}

		ui.hsliderImgCount->setMaximum(imageCount-1);
		ui.labelImgCount->setText(QString::number(maxIndex));
		ui.labelImageStart->setText(QString::number(minIndex));
		ui.hsliderImgCount->setEnabled(true);

		ui.spinGo->setValue(minIndex);
		ui.spinGo->setMaximum(maxIndex);
		ui.spinGo->setEnabled(true);

		ui.btnNextFrame->setEnabled(true);
		ui.btnGo->setEnabled(true);

		img = readFromFolder(minIndex);
	
		label->setGeometry(QRect(10,40,img.width()/2, img.height()/2));
		label->setImage(img);
		label->update();

		showSubWidgets();
	}
}

void ToolWindow::getFrame(int index)
{
	qDebug()<<"index "<<index;
	if(type == 3)
	{
		img = readFromVideo(index);
		label->setImage(img);
		label->update();
		ui.spinGo->setValue(index);
	}
	else if(type == 2)
	{
		img = readFromFolder(index + minIndex);
		label->setImage(img);
		label->update();
		ui.spinGo->setValue(index + minIndex);
	}

	if(index >= imageCount - 1)
	{
		ui.btnNextFrame->setEnabled(false);
	}
	else
	{
		ui.btnNextFrame->setEnabled(true);
	}

	if(index <= 0)
	{
		ui.btnLastFrame->setEnabled(false);
	}
	else
	{
		ui.btnLastFrame->setEnabled(true);
	}
}

void ToolWindow::nextFrame()
{
	int index = ui.hsliderImgCount->value() + 1;
	ui.hsliderImgCount->setValue(index);
	//getFrame(index);
}

void ToolWindow::lastFrame()
{
	int index = ui.hsliderImgCount->value() - 1;
	ui.hsliderImgCount->setValue(index);
	//getFrame(index);
}

void ToolWindow::gotoFrame()
{
	int index = ui.spinGo->value();
	ui.hsliderImgCount->setValue(index);
	getFrame(index);
}

void ToolWindow::saveIcon()
{
	if(!item.isNull())
	{
		emit saveItem();
	}
}

void ToolWindow::closeWindow(bool checked)
{
	emit quit();

	QMainWindow::close();
}

void ToolWindow::leftSubImg()
{
	if(img.isNull()) return;

	int step = ui.spinHSub->value();

	label->setLeft(-step);
	label->update();
}

void ToolWindow::rightSubImg()
{
	if(img.isNull()) return;

	int step = ui.spinHSub->value();

	label->setLeft(step);
	label->update();
}

void ToolWindow::upSubImg()
{
	if(img.isNull()) return;

	int step = ui.spinVSub->value();

	label->setUp(-step);
	label->update();
}

void ToolWindow::downSubImg()
{
	if(img.isNull()) return;

	int step = ui.spinVSub->value();

	label->setUp(step);
	label->update();
}

void ToolWindow::nextSubImg()
{
	if(img.isNull()) return;

	int index = label->getSubIndex() + 1 > 4 ? 1 : label->getSubIndex() + 1;
	label->setSubIndex(index);

	ui.spinHSub->setValue(index);

	label->update();
}

void ToolWindow::lastSubImg()
{
	if(img.isNull()) return;

	int index = label->getSubIndex() - 1 <= 0 ? 4 : label->getSubIndex() - 1;
	label->setSubIndex(index);

	ui.spinHSub->setValue(index);

	label->update();
}

cv::Mat convertImageToMat(QImage* img)
{
	cv::Mat tmp(img->height(), img->width(), CV_8UC4,(uchar*)img->bits(),img->bytesPerLine());
	cv::Mat mat = cv::Mat(tmp.rows, tmp.cols, CV_8UC3 ); 
	
	int from_to[] = { 0,0, 1,1, 2,2 }; 
	cv::mixChannels(&tmp, 1, &mat, 1, from_to, 3 );

	//cv::Mat mat_gray;

	//cvtColor(mat,mat_gray,CV_BGR2GRAY);

	return mat;
}

void detectImageFeature(cv::Mat img_gray,vector<cv::KeyPoint>& image_kps,cv::Mat& image_des)
{
	const int FAST_CORNER_THRESH = 65;
	const bool FAST_CORNER_THRESH_SUPRESS = false;
	const int HESSIAN_THRESHOLD = 400;
	const int OCTAVES = 4;
	const int OCTAVELAYERS = 4;
	const bool EXTENDED = false;
	const bool UPRIGHT = false;
 
	cv::SURF surf_inst;
	surf_inst.hessianThreshold = HESSIAN_THRESHOLD;
	surf_inst.nOctaves         = OCTAVES;
	surf_inst.nOctaveLayers    = OCTAVELAYERS;
	surf_inst.extended         = EXTENDED;
	surf_inst.upright          = UPRIGHT;

	cv::FAST(img_gray, image_kps, FAST_CORNER_THRESH, FAST_CORNER_THRESH_SUPRESS ); //key point detector
	surf_inst(img_gray, 
		cv::Mat(), //frame_mask
		image_kps, image_des, true);
}

vector<cv::Rect> clusterDetects_hough_fixedscale(cv::Mat tar_img_gray, vector<cv::KeyPoint> tar_kps, cv::Mat tar_des, 
		cv::Mat item, vector<cv::KeyPoint> item_kps, cv::Mat item_des, vector<cv::DMatch > matches)
{
	//Cluster the detects similar to hough transformation meathod
	//This function deals with the case where template appears in the target image without scaling.
	const double SUPPORTSIGMA = 0.2;
	double revSUPPORTSIGMA2 = 0.5/(SUPPORTSIGMA*SUPPORTSIGMA);

	int i;
	int fr_i, to_i;
	int loc_x, loc_y;
	float cnt;
	cv::Mat houghmap;
	houghmap = cv::Mat::zeros(tar_img_gray.rows+item.rows, tar_img_gray.cols+item.cols, CV_32FC1);

	vector<cv::Rect> detects;
	for( i=0; i<matches.size(); i++)
	{
		fr_i = matches[i].queryIdx;
		to_i = matches[i].trainIdx;

		loc_x = tar_kps[to_i].pt.x - item_kps[fr_i].pt.x + item.cols;
		loc_y = tar_kps[to_i].pt.y - item_kps[fr_i].pt.y + item.rows;

		//add support to the location
		//houghmap.at<float>(loc_y, loc_x) += 1;
		houghmap.at<float>(loc_y, loc_x) += exp( -revSUPPORTSIGMA2*matches[i].distance*matches[i].distance );
	}

	printf("\nFunction clusterDetects_hough_fixedscale\n");
	for( i=0; i<matches.size(); i++)
	{
		fr_i = matches[i].queryIdx;
		to_i = matches[i].trainIdx;

		loc_x = tar_kps[to_i].pt.x - item_kps[fr_i].pt.x + item.cols;
		loc_y = tar_kps[to_i].pt.y - item_kps[fr_i].pt.y + item.rows;
		
		cnt = houghmap.at<float>(loc_y, loc_x);

		if( cnt > 0.2 * item_kps.size() )//-----------------------tuning term----------------
		{
			cv::Rect temp( loc_x-item.cols, loc_y-item.rows, item.cols, item.rows) ;
			if(temp.x<0) temp.x = 0;
			if(temp.y<0) temp.y = 0;
			if(temp.x+temp.width>tar_img_gray.cols) temp.width = tar_img_gray.cols - temp.x;
			if(temp.y+temp.height>tar_img_gray.rows) temp.width = tar_img_gray.rows - temp.y;

			detects.push_back( temp );

			houghmap.at<float>(loc_y, loc_x) = 0;
		}
		
	}
	return detects;
}

vector<cv::Rect> clusterDetects_hough(cv::Mat tar_img_gray, vector<cv::KeyPoint> tar_kps, cv::Mat tar_des, 
	cv::Mat item, vector<cv::KeyPoint> item_kps, cv::Mat item_des, vector<cv::DMatch> matches)
{
	const double MINSCALE = 0.5;
	const double MAXSCALE = 5;
	const double FINENESS = 0.1;
	const double SUPPORTSIGMA = 0.2;
	double ep = 1e-5;//EPSILON;
	double revFINENESS = 1.0/FINENESS;
	double revSUPPORTSIGMA2 = 0.5/(SUPPORTSIGMA*SUPPORTSIGMA);

	int padrows = MAXSCALE*item.rows;
	int padcols = MAXSCALE*item.cols;
	int sz[] = {tar_img_gray.rows+padrows, tar_img_gray.cols+padcols, (MAXSCALE-MINSCALE)/FINENESS+1  };
	cv::SparseMat_<float> houghmap(3, sz);

	int i,j;
	int fr[2];
	int to[2];
	cv::Point2f fr_pt[2];
	cv::Point2f to_pt[2];
	double fr_dx, fr_dy;
	double to_dx, to_dy;

	vector<cv::Rect> detects;
	int nDetects;
	
	int    loc_x, loc_y, loc_s;
	double org_x, org_y, org_s;

	for( i=0; i<matches.size(); i++ )
	{
		for( j=0; j<matches.size(); j++ )
		{
			if(i==j)
			{
				continue;
			}
			fr[0] = matches[i].queryIdx;
			to[0] = matches[i].trainIdx;

			fr_pt[0] = item_kps[fr[0]].pt;
			to_pt[0] =  tar_kps[to[0]].pt;

			fr[1] = matches[j].queryIdx;
			to[1] = matches[j].trainIdx;

			fr_pt[1] = item_kps[fr[1]].pt;
			to_pt[1] =  tar_kps[to[1]].pt;

			fr_dx = fr_pt[0].x - fr_pt[1].x;
			fr_dy = fr_pt[0].y - fr_pt[1].y;
			to_dx = to_pt[0].x - to_pt[1].x;
			to_dy = to_pt[0].y - to_pt[1].y;

			if( abs(fr_dx+fr_dy)<ep || abs(fr_dx)<ep || abs(fr_dy)<ep )
			{
				continue;
			}

			org_s = (to_dx+to_dy)/(fr_dx+fr_dy);
			if( org_s < MINSCALE || org_s > MAXSCALE )
			{
				continue;
			}
			org_x = 0.5*(to_pt[0].x+to_pt[1].x - org_s*(fr_pt[0].x+fr_pt[1].x));
			org_y = 0.5*(to_pt[0].y+to_pt[1].y - org_s*(fr_pt[0].y+fr_pt[1].y));

			loc_x = (int)floor(org_x+padcols+0.5);
			loc_y = (int)floor(org_y+padrows+0.5);
			loc_s = (int)floor((org_s-MINSCALE)*revFINENESS + 0.5);

			houghmap.ref(loc_y, loc_x, loc_s) += exp( -revSUPPORTSIGMA2*( matches[i].distance*matches[i].distance + 
																		 matches[j].distance*matches[j].distance ) );
		}
	}

	nDetects = 0;
	
	cv::SparseMatIterator_<float> it = houghmap.begin(),
                              it_end = houghmap.end();
    for(; it != it_end; ++it)
	{
		const cv::SparseMat_<float>::Node* nd = it.node();
		//float itnum = (*it);//debug
		//float itnum_mat = houghmap(nd->idx[0],nd->idx[1], nd->idx[2]);//debug
		if( (*it) > 0.01 * item_kps.size()*item_kps.size() )
		{
			loc_y = nd->idx[0] - padrows;
			loc_x = nd->idx[1] - padcols;
			org_s = (nd->idx[2])*FINENESS + MINSCALE;

			printf("Rect_%d (%4d,%4d,%2.1lf) contains %.2f matches.\n", nDetects, loc_x, loc_y, org_s, houghmap(nd->idx));

			cv::Rect temp( loc_x, loc_y, org_s*item.cols, org_s*item.rows);
			if(temp.x<0) temp.x = 0;
			if(temp.y<0) temp.y = 0;
			if(temp.x+temp.width>tar_img_gray.cols) temp.width = tar_img_gray.cols - temp.x;
			if(temp.y+temp.height>tar_img_gray.rows) temp.width = tar_img_gray.rows - temp.y;

			detects.push_back(temp);
			nDetects ++;
		}
	}

	return detects;
}


void ToolWindow::match()
{
	cv::Mat cvImg, cvItem;
	cv::Mat cvImg_gray, cvItem_gray;

	if(img.isNull() || item.isNull())
	{
		return;
	}

	cvImg = convertImageToMat(&img);
	cvItem = convertImageToMat(&item);
	cvtColor(cvImg,cvImg_gray,CV_BGR2GRAY);
	cvtColor(cvItem,cvItem_gray,CV_BGR2GRAY);

	vector<cv::KeyPoint> image_kps, item_kps;
	cv::Mat				 image_des, item_des;

	detectImageFeature(cvImg_gray,image_kps,image_des);
	detectImageFeature(cvItem_gray,item_kps,item_des);

	cv::FlannBasedMatcher		 matcher;
	vector<vector<cv::DMatch > > matches;
	vector<cv::DMatch >			 matches_good;
	cv::Mat						 matches_img;

	matcher.knnMatch(item_des, image_des, matches, 2);
	for(int i = 0; i < std::min(image_des.rows-1,(int) matches.size()); i++)
	{
		if((matches[i][0].distance < 0.98*(matches[i][1].distance)) && 
			((int) matches[i].size()<=2 && (int) matches[i].size()>0))
		{
			matches_good.push_back(matches[i][0]);
		}
	}

	vector<cv::Rect> result = clusterDetects_hough(cvImg_gray,image_kps,image_des, cvItem_gray,item_kps,item_des,matches_good);

	for(int i=0; i<result.size(); i++)
	{
		cv::rectangle(cvImg,result[i],cv::Scalar(0,255,0),1);
	}

	cv::imshow("image",cvImg);
	cv::waitKey(0);
}