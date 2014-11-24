#include "tplwindow.h"
#include <io.h>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include "newtpldialog.h"
#include "newappdialog.h"
#include "newappcopydialog.h"
#include "util.h"

TplWindow::TplWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	tplFolder = "C:\\Users\\LingFeng\\Documents\\Visual Studio 2010\\Projects\\CVTool2\\tpls";

	QStandardItemModel *model1 = new QStandardItemModel(this); 
	QStandardItemModel *model2 = new QStandardItemModel(this); 
	QStandardItemModel *model3 = new QStandardItemModel(this); 
	QStandardItemModel *model4 = new QStandardItemModel(this); 
	ui.listTpl->setModel(model1);
	ui.listApp->setModel(model2);
	ui.ListRow->setModel(model3);
	ui.listIcon->setModel(model4);

	connect(ui.listTpl, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tplSelected(const QModelIndex &)));
	connect(ui.listApp, SIGNAL(clicked(const QModelIndex &)), this, SLOT(appSelected(const QModelIndex &)));
	connect(ui.ListRow, SIGNAL(clicked(const QModelIndex &)), this, SLOT(rowSelected(const QModelIndex &)));
	connect(ui.listIcon, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemSelected(const QModelIndex &)));

	connect(ui.actionNew_Template,SIGNAL(triggered(bool)), this, SLOT(newTemplate()));
	connect(ui.actionNew_Template_Copy,SIGNAL(triggered(bool)), this, SLOT(newTemplateCopy()));
	connect(ui.actionDelete_Template, SIGNAL(triggered(bool)), this, SLOT(deleteTemplate()));
	connect(ui.actionNew_Application, SIGNAL(triggered(bool)), this, SLOT(newApplication()));
	connect(ui.actionNewApplicationCopy, SIGNAL(triggered(bool)),this,SLOT(newApplicationCopy()));
	connect(ui.actionAddBefore, SIGNAL(triggered(bool)), this, SLOT(addRowBefore()));
	connect(ui.actionDelete_A_row, SIGNAL(triggered(bool)), this, SLOT(deleteARow()));
	connect(ui.actionAdd_After,SIGNAL(triggered(bool)), this,SLOT(addRowAfter()));
	connect(ui.actionAddIcon, SIGNAL(triggered(bool)), this, SLOT(addItem()));

	loadTpls();

	paintWindow();
}

void TplWindow::paintWindow()
{
	QStandardItemModel *itemModel = (QStandardItemModel*)ui.listTpl->model();
	itemModel->clear();

	for(int i=0; i<tpls.size(); i++)
	{
		QString tmp = QString::fromStdString(tpls[i].name);
		QStandardItem *item = new QStandardItem(tmp); 
		itemModel->appendRow(item);
	}

	ui.listTpl->setCurrentIndex(itemModel->index(0,0));
	tplSelected(ui.listTpl->currentIndex());
}

void TplWindow::paintListRow()
{
	int tplIndex = ui.listTpl->currentIndex().row();
	int appIndex = ui.listApp->currentIndex().row();
	if(tplIndex<0 || appIndex<0)
	{
		return;
	}

	QStandardItemModel *itemModel = (QStandardItemModel*)ui.ListRow->model();
	itemModel->clear();
	ClsApp tpl = tpls[tplIndex].apps[appIndex];
	vector<ClsRow> rows = tpl.rows;
	for(int i=0; i<rows.size(); i++)
	{
		QString tmp = "ROW_" + QString::number(rows[i].index);
		QStandardItem *item = new QStandardItem(tmp); 
		itemModel->appendRow(item);
	}
}

TplWindow::~TplWindow()
{

}

void TplWindow::loadTpls()
{
	tpls.clear();
	string pattern = tplFolder + "/*";

	_finddata_t fileDir;
	long lfDir;

	if((lfDir=_findfirst(pattern.c_str(),&fileDir)) == -1)
	{
		 QMessageBox::information(this, tr("No file"), tr("No File"));
	}
	else
	{
		do
		{
			string tempStr(fileDir.name);
			if(tempStr == "." || tempStr == "..") continue;

			ClsTpl tpl;
			tpl.loadApp(tplFolder, tempStr);

			tpls.push_back(tpl);
		}while( _findnext( lfDir, &fileDir ) == 0 );	
	}

	_findclose(lfDir);
}

void TplWindow::tplSelected(const QModelIndex &selected)
{
	qDebug()<<selected.data().toString()<<' '<<selected.row();

	int index = selected.row();
	if(index < 0) 
	{
		return;
	}

	QStandardItemModel *itemModel = (QStandardItemModel*)ui.listApp->model();
	itemModel->clear();
	vector<ClsApp> apps = tpls[index].apps;
	for(int i=0; i<apps.size(); i++)
	{
		QString tmp = QString::fromStdString(apps[i].name);
		QStandardItem *item = new QStandardItem(tmp); 
		itemModel->appendRow(item);
	}

	ui.listApp->setCurrentIndex(itemModel->index(0,0));
	appSelected(ui.listApp->currentIndex());
}

void TplWindow::appSelected(const QModelIndex &selected)
{
	qDebug()<<selected.data().toString()<<' '<<selected.row();

	int index = selected.row();
	if(index < 0) 
	{
		QStandardItemModel *listModel = (QStandardItemModel*)ui.ListRow->model();
		QStandardItemModel *itemModel = (QStandardItemModel*)ui.listIcon->model();
		listModel->clear();
		itemModel->clear();
		ui.labelIcon->clear();
		ui.maskLabel->clear();
		return;
	}

	QModelIndex tplSelected = ui.listTpl->currentIndex();
	int currentTpl = tplSelected.row();

	QStandardItemModel *itemModel = (QStandardItemModel*)ui.ListRow->model();
	itemModel->clear();
	ClsApp tpl = tpls[currentTpl].apps[index];
	vector<ClsRow> rows = tpl.rows;
	for(int i=0; i<rows.size(); i++)
	{
		QString tmp = "ROW_" + QString::number(rows[i].index);
		QStandardItem *item = new QStandardItem(tmp); 
		itemModel->appendRow(item);
	}

	ui.editContentRow->setText(QString::number(tpl.contentRow));
	ui.editUrlRow->setText(QString::number(tpl.urlRow));

	ui.ListRow->setCurrentIndex(itemModel->index(0,0));
	rowSelected(ui.ListRow->currentIndex());
}

void TplWindow::rowSelected(const QModelIndex &selected)
{
	int index = selected.row();
	if(index < 0)
	{
		QStandardItemModel *itemModel = (QStandardItemModel*)ui.listIcon->model();
		itemModel->clear();
		ui.labelIcon->clear();
		ui.maskLabel->clear();
		return;
	}

	QModelIndex tplSelected = ui.listTpl->currentIndex();
	QModelIndex appSelected = ui.listApp->currentIndex();

	int currentTpl = tplSelected.row();
	int currentApp = appSelected.row();

	QStandardItemModel *itemModel = (QStandardItemModel*)ui.listIcon->model();
	itemModel->clear();
	vector<ClsItem> items = tpls[currentTpl].apps[currentApp].rows[index].items;
	for(int i=0; i<items.size(); i++)
	{
		QString tmp = QString::fromStdString(items[i].name);
		QStandardItem *item = new QStandardItem(tmp); 
		itemModel->appendRow(item);
	}

	ui.listIcon->setCurrentIndex(itemModel->index(0,0));
	itemSelected(ui.listIcon->currentIndex());
}

QImage convertFromMat(cv::Mat img)
{
	QImage::Format format;
	switch(img.type())
	{
		case CV_8UC4:
			format = QImage::Format_RGB32; break;
		case CV_8UC3:
			format = QImage::Format_RGB888; break;
		default:
			qDebug()<<"image format not supported";
			//QMessageBox::information(NULL, tr("Error"), tr("Image format not supported")); 
	}
		
	QImage temp(img.data, img.cols, img.rows, img.step, format);
	return temp.rgbSwapped();
}

void TplWindow::itemSelected(const QModelIndex &selected)
{
	int index = selected.row();
	if(index < 0) 
	{
		ui.labelIcon->clear();
		ui.maskLabel->clear();
		return;
	}
	QModelIndex tplSelected = ui.listTpl->currentIndex();
	QModelIndex appSelected = ui.listApp->currentIndex();
	QModelIndex rowSelected = ui.ListRow->currentIndex();

	int currentTpl = tplSelected.row();
	int currentApp = appSelected.row();
	int currentRow = rowSelected.row();

	//QStandardItemModel *itemModel = (QStandardItemModel*)ui.listIcon->model();
	ClsItem item = tpls[currentTpl].apps[currentApp].rows[currentRow].items[index];
	
	QImage qimg = convertFromMat(item.img);
	QRect rect;
	rect.setX(ui.labelIcon->geometry().x());
	rect.setY(ui.labelIcon->geometry().y());
	rect.setWidth(qimg.width());
	rect.setHeight(qimg.height());
	ui.labelIcon->setGeometry(rect);

	ui.labelIcon->setPixmap(QPixmap::fromImage(qimg));

	if(item.hasMask())
	{
		QImage qmaskImg = convertFromMat(item.img_mask);
		ui.maskLabel->setPixmap(QPixmap::fromImage(qmaskImg));
	}
	else
	{
		ui.maskLabel->clear();
	}
}

void TplWindow::newTemplate()
{
	NewTplDialog* dialog = new NewTplDialog(this);
	
	if(dialog->exec() == QDialog::Accepted)
	{
		qDebug()<<"new template " + dialog->getTplName();
		ClsTpl newTpl(dialog->getTplName().toStdString());
		
		newTpl.write(tplFolder);

		//tpls.push_back(newTpl);
		loadTpls();
		paintWindow();
	}
}

void TplWindow::newTemplateCopy()
{
	int index = ui.listTpl->currentIndex().row();
	if(index<0)
	{
		QMessageBox::information(this, tr("No Seletced Template"), tr("Please Select a Template"));
		return;
	}

	ClsTpl tpl = tpls[index];

	NewTplDialog* dialog = new NewTplDialog(this);
	if(dialog->exec() == QDialog::Accepted)
	{
		ClsTpl newTpl = tpl;

		newTpl.name = dialog->getTplName().toStdString();

		newTpl.write(tplFolder);

		//tpls.push_back(newTpl);

		loadTpls();
		paintWindow();
	}
}

void TplWindow::deleteTemplate()
{
	int index = ui.listTpl->currentIndex().row();
	if(index<0)
	{
		QMessageBox::information(this, tr("No Seletced Template"), tr("Please Select a Template"));
		return;
	}

	ClsTpl tpl = tpls[index];

	QString info = QString::fromStdString("Are you sure to Delete Template" + tpl.name);
	//QMessageBox::StandardButton confirm = 

	if(QMessageBox::Yes == QMessageBox::question(this, tr("Delete Template"),info,QMessageBox::Yes, QMessageBox::No))
	{
		if(tpl.delTemplate(tplFolder))
		{
			loadTpls();
			paintWindow();
		}
		else
		{
			QMessageBox::information(this, tr("delete failed"), tr("delete failed"));
		}
	}
}

bool appExists(ClsTpl tpl, string appName)
{
	for(int i=0; i<tpl.apps.size(); i++)
	{
		if(tpl.apps[i].name == appName)
		{
			return true;
		}
	}
	return false;
}

void TplWindow::newApplication()
{
	int index = ui.listTpl->currentIndex().row();
	if(index<0)
	{
		QMessageBox::information(this, tr("No Seletced Template"), tr("Please Select a Template"));
		return;
	}

	ClsTpl tpl = tpls[index];

	NewAppDialog *dialog = new NewAppDialog(this);

	if(dialog->exec() == QDialog::Accepted)
	{
		ClsApp app;
		app.name = dialog->getAppName().toStdString();
		app.contentRow = dialog->getContentRow();
		app.urlRow = dialog->getUrlRow();
		app.initRows(dialog->getRowNum());

		if(appExists(tpl,app.name))
		{
			QMessageBox::information(this, tr("App Has Existed"), tr("App Has Existed"));
			return;
		}

		app.write(tplFolder+"/"+tpl.name);

		loadTpls();
		paintWindow();
		this->update();
	}
}

void TplWindow::newApplicationCopy()
{
	int index = ui.listTpl->currentIndex().row();
	if(index<0)
	{
		QMessageBox::information(this, tr("No Seletced Template"), tr("Please Select a Template"));
		return;
	}

	ClsTpl tpl = tpls[index];

	NewAppCopyDialog *dialog = new NewAppCopyDialog(this);

	if(dialog->exec() == QDialog::Accepted)
	{
		QString appName = dialog->getAppName();
		int tplIndex = dialog->getCopyTplIndex();
		int appIndex = dialog->getCopyAppIndex();

		if(appExists(tpl,appName.toStdString()))
		{
			QMessageBox::information(this, tr("App Has Existed"), tr("App Has Existed"));
			return;
		}

		ClsApp app = tpls[tplIndex].apps[appIndex];

		app.write(tplFolder+"/"+tpl.name);
		loadTpls();
		paintWindow();
	}
}

void TplWindow::deleteARow()
{
	if(QMessageBox::Yes != QMessageBox::question(this, tr("Delete Row"),tr("Are you sure to Delete this Row"),QMessageBox::Yes, QMessageBox::No))
	{
		return;
	}

	int tplIndex = ui.listTpl->currentIndex().row();
	int appIndex = ui.listApp->currentIndex().row();
	int rowIndex = ui.ListRow->currentIndex().row();

	if(tplIndex < 0 || appIndex <0 || rowIndex<0)
	{
		QMessageBox::information(this,tr("No Selected"),tr("Please Selected a Row"));
		return;
	}
	
	string tplName = tpls[tplIndex].name;
	tpls[tplIndex].apps[appIndex].deleteARow(tplFolder+"/"+tplName, rowIndex);
	
	paintListRow();
}

void TplWindow::addRowBefore()
{
	int tplIndex = ui.listTpl->currentIndex().row();
	int appIndex = ui.listApp->currentIndex().row();
	int rowIndex = ui.ListRow->currentIndex().row();

	if(tplIndex < 0 || appIndex <0 || rowIndex<0)
	{
		QMessageBox::information(this,tr("No Selected"),tr("Please Selected a Row"));
		return;
	}

	if(QMessageBox::Yes != QMessageBox::question(this, tr("Add Row"),tr("Are you sure to Add a Row"),QMessageBox::Yes, QMessageBox::No))
	{
		return;
	}

	string tplName = tpls[tplIndex].name;
	tpls[tplIndex].apps[appIndex].addRow(tplFolder+"/"+tplName, rowIndex);

	paintListRow();
}

void TplWindow::addRowAfter()
{
	int tplIndex = ui.listTpl->currentIndex().row();
	int appIndex = ui.listApp->currentIndex().row();
	int rowIndex = ui.ListRow->currentIndex().row();

	if(tplIndex < 0 || appIndex <0 || rowIndex<0)
	{
		QMessageBox::information(this,tr("No Selected"),tr("Please Selected a Row"));
		return;
	}

	if(QMessageBox::Yes != QMessageBox::question(this, tr("Add Row"),tr("Are you sure to Add a Row"),QMessageBox::Yes, QMessageBox::No))
	{
		return;
	}

	string tplName = tpls[tplIndex].name;
	tpls[tplIndex].apps[appIndex].addRow(tplFolder+"/"+tplName, rowIndex+1);

	paintListRow();
}

void TplWindow::addItem()
{
	
}