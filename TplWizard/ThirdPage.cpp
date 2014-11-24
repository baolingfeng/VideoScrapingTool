#include "ThirdPage.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <io.h>
#include <direct.h>

ThirdPage::ThirdPage(QWidget *parent)
{
	ui.setupUi(this);

	current_row = -1;
	current_item = -1;
	tw = new ToolWindow;

	//initialize application type
	_finddata_t fileDir;
	char* dir="C:/Users/LingFeng/Documents/Visual Studio 2010/Projects/CVTool2/data/*";
	long lfDir;

	if((lfDir=_findfirst(dir,&fileDir)) == -1)
	{
		 QMessageBox::information(this, tr("No file"), tr(dir));
	}
	else
	{
		do{
			if(strcmp(fileDir.name,".") != 0 && strcmp(fileDir.name,"..") != 0)
			{
				ui.combox_apptype->addItem(fileDir.name);
			}
        }while( _findnext( lfDir, &fileDir ) == 0 );
	}

	QStandardItemModel *standardItemModel = new QStandardItemModel(this); 
	QStandardItemModel *standardItemModel2 = new QStandardItemModel(this); 
	ui.RowList->setModel(standardItemModel);
	ui.ItemList->setModel(standardItemModel2);

	//ui.RowList->setSelectionModel
	connect(ui.btnAddRow,SIGNAL(clicked()),this,SLOT(addRow()));
	connect(ui.btnDelRow,SIGNAL(clicked()),this,SLOT(delRow()));
	connect(ui.btnAddItem,SIGNAL(clicked()),this,SLOT(addItem()));
	connect(ui.btnDelItem,SIGNAL(clicked()),this,SLOT(delItem()));
	connect(ui.btnTool,SIGNAL(clicked()),this,SLOT(showTool()));
	connect(ui.btnAddMsk,SIGNAL(clicked()),this,SLOT(addItemMask()));

	ui.combox_apptype->setCurrentIndex(-1);
	connect(ui.combox_apptype,SIGNAL(currentTextChanged(QString)),this,SLOT(appTypeSelected(QString)));

	connect(ui.RowList,SIGNAL(clicked(const QModelIndex &)),
		this,SLOT(rowSelected(const QModelIndex &)));
	connect(ui.ItemList,SIGNAL(clicked(const QModelIndex &)),
		this,SLOT(itemSelected(const QModelIndex &)));

	connect(tw,SIGNAL(quit()),this,SLOT(toolWindowClose()));
	connect(tw,SIGNAL(saveItem()), this,SLOT(addItemFromTool()));
	
}

void ThirdPage::initializePage()
{
	bool flag = field("create").toBool();

	if(!flag)
	{
		QString folder = field("folder").toString();
		qDebug()<<folder;

		QStringList list = folder.split("/");
		if(list.size() < 2)
		{
			qDebug()<<"wrong folder";
			return;
		}
		QString appname = list[list.size()-1];
		QString apptype= list[list.size()-2];

		if(appname.indexOf("APP_")>=0)
			appname = appname.mid(4);

		qDebug()<<appname<<"/"<<apptype;

		FILE* rowConfig = fopen((folder+"/config.txt").toStdString().c_str(),"r");
		if(rowConfig == NULL)
		{
			qDebug()<<"row config file doesn't exist";
			return;
		}

		int rownum,content_row,url_row;
		int value;
		char tmp[256];
		while(!feof(rowConfig))
		{
			fscanf(rowConfig,"%s %d",tmp,&value);

			if(strcmp(tmp,"ROWS") == 0)
			{
				rownum = value;
			}
			else if(strcmp(tmp,"CONTENT") == 0)
			{
				content_row = value;
			}
			else if(strcmp(tmp,"URL") == 0)
			{
				url_row = value;
			}
		}
		fclose(rowConfig);

		ui.combox_apptype->setCurrentText(apptype);
		ui.edit_appname->setText(appname);
		ui.spinBox_content->setValue(content_row);
		ui.spinBox_url->setValue(url_row);

		QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.RowList->model(); 
		for(int i=0; i<rownum;i++)
		{
			MyRow row;
			FILE* itemConfig = fopen((folder+"/ROW_"+QString::number(i+1)+"/config.txt").toStdString().c_str(),"r");
			
			int itemnum;
			char type[256];
			vector<int> masks;
			while(!feof(itemConfig))
			{
				fscanf(itemConfig,"%s",type);
				if(strcmp(type,"ITEMS") == 0)
				{
					fscanf(itemConfig,"%d",&itemnum);
				}
				else if(strcmp(type,"MASK") == 0)
				{
					int temp;
					fscanf(itemConfig,"%d",&temp);
					while(temp != -1)
					{
						masks.push_back(temp);
						fscanf(itemConfig,"%d",&temp);
					}
				}
			}

			for(int j=0; j<itemnum; j++)
			{
				MyItem myitem;
				QImage img; //= new QImage;
				img.load(folder+"/ROW_"+QString::number(i+1)+"/ITEM_"+QString::number(j+1)+".png");

				if(std::find(masks.begin(),masks.end(),j+1) != masks.end())
				{
					QImage img_mask;
					img_mask.load(folder+"/ROW_"+QString::number(i+1)+"/ITEM_"+QString::number(j+1)+"_mask.png");
					myitem.item_mask = img_mask;
				}

				myitem.item = img;
				row.items.push_back(myitem);
			}

			rows.push_back(row);

			QString tmp = "Row " + QString::number(i+1);
			QStandardItem *item = new QStandardItem(tmp); 
			standardItemModel->appendRow(item);
		}	
	}
}

bool ThirdPage::validatePage()
{
	QString apptype = ui.combox_apptype->currentText();
	QString appname = ui.edit_appname->text();
	int content_row = ui.spinBox_content->value();
	int url = ui.spinBox_url->value();

	QString baseDir = "D:/temp/";

	QString appDir = baseDir + apptype;
	if(_access(appDir.toStdString().c_str(),0) < 0)
	{
		mkdir(appDir.toStdString().c_str());
	}

	QString nameDir = appDir + "/APP_" + appname;
	if(_access(nameDir.toStdString().c_str(),0) < 0)
	{
		mkdir((nameDir).toStdString().c_str());
	}

	FILE* rowConfig = fopen((nameDir.toStdString()+"/config.txt").c_str(),"w");
	fprintf(rowConfig,"ROWS %d", rows.size());
	fclose(rowConfig);

	for(int i=0; i<rows.size(); i++)
	{
		QString tmp = nameDir + "/ROW_" + QString::number(i+1); 
		mkdir(tmp.toStdString().c_str());

		FILE* itemConfig= fopen((tmp.toStdString()+"/config.txt").c_str(),"w");
		fprintf(itemConfig, "ITEMS %d\n",rows[i].items.size());

		vector<int> masks;
		for(int j=0; j<rows[i].items.size(); j++)
		{
			rows[i].items[j].item.save(tmp+"/ITEM_"+QString::number(j+1)+".png","png");

			if(!rows[i].items[j].item_mask.isNull())
			{
				rows[i].items[j].item_mask.save(tmp+"/ITEM_"+QString::number(j+1)+"_mask.png","png");
				masks.push_back(j+1);
			}
		}

		if(masks.size()>0)
		{
			fprintf(itemConfig,"MASK ");
			for(int i=0; i<masks.size(); i++)
			{
				fprintf(itemConfig,"%d ",masks[i]);
			}
			fprintf(itemConfig,"-1");
		}

		fclose(itemConfig);
	}

	return true;
}

ThirdPage::~ThirdPage(void)
{
}


void ThirdPage::rowSelected(const QModelIndex &selected)
{
	qDebug()<<selected.data().toString()<<' '<<selected.row();
	
	current_row = selected.row();

	QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.ItemList->model(); 
	standardItemModel->clear();

	MyRow row = rows[current_row];
	for(int i=0; i<row.items.size(); i++)
	{
		QString tmp = "Item " + QString::number(i);
		QStandardItem *item = new QStandardItem(tmp); 
		standardItemModel->appendRow(item);
		qDebug()<<i;
	}
	//QMessageBox::information(this,tr("Message"),tr("selected change"));
}

void ThirdPage::addRow()
{
	if(QMessageBox::warning(this,tr("Add Row"),tr("Are you sure to add a row?"),
		QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		MyRow row;
		row.row = rows.size()+1;
		rows.push_back(row);

		QString tmp = "Row " + QString::number(row.row);
		QStandardItem *item = new QStandardItem(tmp); 
		QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.RowList->model(); 
		standardItemModel->appendRow(item);
	}
}

void ThirdPage::delRow()
{
	if(current_row<0)
	{
		return;
	}

	if(QMessageBox::warning(this,tr("Delete Row"),tr("Are you sure to delete this row?"),
		QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		rows.erase(rows.begin()+current_row);
		QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.RowList->model(); 
		standardItemModel->clear();
		for(int i=1; i<=rows.size(); i++)
		{
			QString tmp = "Row " + QString::number(i);
			QStandardItem *item = new QStandardItem(tmp); 
			standardItemModel->appendRow(item);
		}

		current_row = -1;
	}
}

void ThirdPage::itemSelected(const QModelIndex &selected)
{
	current_item = selected.row();
	MyRow row = rows[current_row];

	QImage item = row.items[current_item].item;
	QImage item_mask = row.items[current_item].item_mask;
	if(!item_mask.isNull())
	{
		int w = item.width() + item_mask.width();
		int h = max(item.height(),item_mask.height());

		QImage* mergeImg = new QImage(w,h,item.format());
		QPainter* painter = new QPainter(mergeImg);
		painter->drawImage(0,0,item);
		painter->drawImage(item.width(),0,item_mask);

		ui.imgLabel->setPixmap(QPixmap::fromImage(*mergeImg));
	}
	else
	{
		ui.imgLabel->setPixmap(QPixmap::fromImage(item));
	}
}

void ThirdPage::addItem()
{
	if(current_row<0)
	{
		QMessageBox::information(this,tr("message"),tr("please select a row"));
		return;
	}

	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setWindowTitle(tr("Open Image")); 
    fileDialog->setDirectory("."); 
    fileDialog->setNameFilter(tr("Image Files(*.png)")); 
	if(fileDialog->exec() == QDialog::Accepted) 
	{ 
		QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.ItemList->model(); 
        
		foreach(QString path,fileDialog->selectedFiles())
		{
			MyItem myitem;
			myitem.item.load(path);
			rows[current_row].items.push_back(myitem);

			int current_size = rows[current_row].items.size();
			QString tmp = "Item " + QString::number(current_size);
			QStandardItem *item = new QStandardItem(tmp); 

			standardItemModel->appendRow(item);
		}
    } 
}

void ThirdPage::delItem()
{
	qDebug()<<"delete item "<<current_row<<"/"<<current_item;
	if(current_row<0 || current_item<0)
	{
		return;
	}

	if(QMessageBox::warning(this,tr("Delete Item"),tr("Are you sure to delete this item?"),
		QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		rows[current_row].items.erase(rows[current_row].items.begin() + current_item);
		
		QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.ItemList->model(); 
		standardItemModel->clear();
		for(int i=1; i<=rows[current_row].items.size(); i++)
		{
			QString tmp = "Item " + QString::number(i);
			QStandardItem *item = new QStandardItem(tmp); 
			standardItemModel->appendRow(item);
		}

		current_item = -1;
		ui.imgLabel->clear();
	}
}

void ThirdPage::addItemMask()
{
	if(current_row<0 || current_item<0)
	{
		QMessageBox::information(this,tr("message"),tr("please select a row and item"));
		return;
	}

	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Mask Image")); 
    fileDialog->setDirectory("."); 
    fileDialog->setNameFilter(tr("Image Files(*.png)")); 
	if(fileDialog->exec() == QDialog::Accepted) 
	{
		QString path = fileDialog->selectedFiles()[0];
		rows[current_row].items[current_item].item_mask.load(path);
	}
}

void ThirdPage::showTool()
{
	if(tw->isHidden())
	{
		tw->show();
	}	
	else
	{
		qDebug()<<"tool window has already opened";
	}
}

void ThirdPage::toolWindowClose()
{
	qDebug()<<"tool window close";
}

void ThirdPage::appTypeSelected(QString index)
{
	//QMessageBox::information(this,"selected",ui.comboBox->currentText());
	if(ui.combox_apptype->currentText() != "TaskBar")
	{
		ui.spinBox_content->setEnabled(true);
	}
	else
	{
		ui.spinBox_content->setEnabled(false);
	}
	
	if(ui.combox_apptype->currentText() == "Chrome" || ui.combox_apptype->currentText() == "Firefox" || 
		ui.combox_apptype->currentText() == "360Browser")
	{
		ui.spinBox_url->setEnabled(true);
	}
	else
	{
		ui.spinBox_url->setEnabled(false);
	}
}

void ThirdPage::addItemFromTool()
{
	if(current_row<0)
	{
		qDebug()<<"no row selected";
		return;
	}

	MyItem myitem;
	myitem.item = tw->getItem();
	rows[current_row].items.push_back(myitem);

	int current_size = rows[current_row].items.size();
	QString tmp = "Item " + QString::number(current_size);
	QStandardItem *item = new QStandardItem(tmp); 

	QStandardItemModel *standardItemModel = (QStandardItemModel*)ui.ItemList->model(); 
	standardItemModel->appendRow(item);
}