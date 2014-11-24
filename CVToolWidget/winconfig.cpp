#include "winconfig.h"
#include "util.h"
#include "qdebug.h"

WinConfig::WinConfig(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	string folder = "../windefinitions/example/";

	GUIComponent* eclipse = getWindowDefinitionFromPath(folder + "Eclipse/","Eclipse");
	GUIComponent* chrome = getWindowDefinitionFromPath(folder + "Chrome/","Chrome");
	GUIComponent* taskbar = getWindowDefinitionFromPath(folder + "TaskBar/","TaskBar");
	components.push_back(eclipse);
	components.push_back(chrome);
	components.push_back(taskbar);

	QStandardItemModel *model = new QStandardItemModel ;
	QList<QStandardItem*> row;
	QFont font;
	font.setBold(true);
	QStandardItem* s1 = new QStandardItem("GUIComponent");
	s1->setData(font, Qt::FontRole);
	row.push_back(s1);
	QStandardItem* s2 = new QStandardItem("Index");
	s2->setData(font, Qt::FontRole);
	row.push_back(s2);
	QStandardItem* s3 = new QStandardItem("To Be Scraped");
	s3->setData(font, Qt::FontRole);
	row.push_back(s3);
	 model->invisibleRootItem()->appendRow(row);

	for(int i=0; i<components.size(); i++)
	{
		setupModelData(components[i], model->invisibleRootItem());
	}

	QStringList header;
	header << "Component Name"<<"Index"<<"To Be Scraped";
	model->setHorizontalHeaderLabels(header);

     //register the model
	 ui.treeView->setModel(model);
     ui.treeView->expandAll();
	 ui.treeView->header()->resizeSection(0, 200);

	// ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	 //connect(ui.treeView,SIGNAL(customContextMenuRequested(const QPoint &)), 
	//	 this, SLOT(slotCustomContextMenu(const QPoint &)));  
	 
	 QStandardItemModel *tableModel = new QStandardItemModel ;
	 ui.tableView->setModel(tableModel);
	 ui.tableView->hide();
	 ui.tableView->setIconSize(QSize(100,100));
	 connect(ui.treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(componentSelected(const QModelIndex&)));
}

WinConfig::~WinConfig()
{

}

void WinConfig::componentSelected(const QModelIndex &index)
{
	if(index.row() <=0 && !index.parent().isValid()) return;

	QString name = index.data(Qt::DisplayRole).toString();
	QString tobescraped = index.sibling(index.row(), 1).data(Qt::DisplayRole).toString();
	qDebug()<<name<<","<<tobescraped<<endl;

	GUIComponent* comp = comp_map[name.toStdString()];

	if(comp->compType == "GUIItem")
	{
		QStandardItemModel* model = (QStandardItemModel*)(ui.tableView->model());
		model->clear();

		QStringList tableHeader;
		tableHeader << "Visual Cue"<<"Icons"<<"Is Unique";
		model->setHorizontalHeaderLabels(tableHeader);

		GUIItem* item = (GUIItem*)comp;
		for(int i=0; i<item->visualcues.size(); i++)
		{
			QList<QStandardItem*> row;

			QStandardItem *nameItem = new QStandardItem(QString::fromStdString(item->visualcues[i].name));
			row.push_back(nameItem);
			
			if(item->visualcues[i].icons.size() > 0)
			{
				int width = 0;
				int height = 0;
				for(int j=0; j<item->visualcues[i].icons.size(); j++)
				{
					cv::Mat icon =  item->visualcues[i].icons[j];
					
					width += icon.cols;
					height = height > icon.rows ? height : icon.rows;
				}
				
				cv::Mat combine = cv::Mat::zeros(height, width,  item->visualcues[i].icons[0].type());
				int left = 0;
				for(int j=0; j<item->visualcues[i].icons.size(); j++)
				{
					cv::Mat icon =  item->visualcues[i].icons[j];
					cv::Mat temp(combine, Rect(left, 0, icon.cols,  icon.rows));
					icon.copyTo(temp);

					left +=  icon.cols;
				}

				QImage qimg = Mat2QImage(combine);
				QIcon qicon(QPixmap::fromImage(qimg));
				QStandardItem *iconItem = new QStandardItem(qicon, "");
				row.push_back(iconItem);
			}
			else
			{
				QStandardItem *iconItem = new QStandardItem("");
				row.push_back(iconItem);
			}
				
			string isunique =item->visualcues[i].isunique ? "true" : "false";
			QStandardItem *isUniqueItem = new QStandardItem(QString::fromStdString(isunique));
			row.push_back(isUniqueItem);

			model->invisibleRootItem()->appendRow(row);
		}

		ui.tableView->show();
	}
	else
	{
		ui.tableView->hide();
	}
}

GUIComponent* WinConfig::getWindowDefinitionFromPath(string folder, string name)
{
	string winConfFile = folder + "config.properties";
	ifstream ifConf(winConfFile);

	string line;
	string compType;
	GUIComponent* component;

	while(getline(ifConf, line))
	{
		int index = line.find("=");
		if(index<0) continue;

		string attr = line.substr(0,index); toLowcase_blf(attr);
		string value = line.substr(index+1);

		if(attr == "belong")
		{
			compType = value;
			if(compType == "window")
			{
				component = new GUIComponent();
			}
			else if(compType == "row")
			{
				component = new GUIRow();
			}
			else if(compType == "GUIItem")
			{
				component = new GUIItem();
			}
			component->compType = compType;
			component->name = name;
		}
		else if(attr == "components")
		{
			cout<<"Components: "<<value<<endl;
			vector<string> comps = split_blf(value, ", ");
			if(compType == "window" || compType == "row")
			{
				for(int i=0; i<comps.size(); i++)
				{
					GUIComponent* subcomp = getWindowDefinitionFromPath(folder + comps[i] + "/", comps[i]);
					component->components.push_back(subcomp);
				}
			}
		}
		else if(attr == "index")
		{
			if(component->compType == "row")
			{
				((GUIRow*)component)->index = stringToNumber_blf<int>(value);
			}
			else if(component->compType == "GUIItem")
			{
				((GUIItem*)component)->index = stringToNumber_blf<int>(value);
			}
		}
		else if(attr == "tobescraped")
		{
			toLowcase_blf(value);
			component->tobescraped = (value == "true");
		}
		else if(attr == "numberofvisualcue")
		{
			int num = stringToNumber_blf<int>(value);
			for(int i=0; i<num; i++)
			{
				VisualCue vc;
				for(int j=0; j<3; j++)
				{
					getline(ifConf, line);
					int index = line.find("=");
				    if(index<0) continue;

					attr = line.substr(0,index); toLowcase_blf(attr);
					value = line.substr(index+1);
					int index2= attr.find(".");
					attr = attr.substr(index2+1);
					if(attr == "name")
					{
						vc.name = value;
					}
					else if(attr == "icons")
					{
						vector<string> icons = split_blf(value, ", ");
						for(int k=0; k<icons.size(); k++)
						{
							string iconName = icons[k] + ".png";
							cv::Mat mat= cv::imread(folder + iconName);
							vc.icons.push_back(mat);
						}
					}
					else if(attr == "isunique")
					{
						toLowcase_blf(value);
						vc.isunique = (value == "true");
					}
				}
				((GUIItem*)component)->visualcues.push_back(vc);
			}
		}
	}

	return component;
}

void WinConfig::setupModelData(GUIComponent* comp, QStandardItem* parent)
{
	QList<QStandardItem*> row;
	QStandardItem *nameItem = new QStandardItem(QString::fromStdString(comp->name));
	string tobesracped = comp->tobescraped ? "true" : "false";
	QStandardItem *tobescrapedItem = new QStandardItem(QString::fromStdString(tobesracped));
	if(comp->tobescraped)
	{
		tobescrapedItem->setData(QColor("red"), Qt::ForegroundRole);
	}

	comp_map[comp->name] = comp;

	string compType = comp->compType;

	QStandardItem *indexItem;
	if(compType == "row")
	{
		nameItem->setIcon(QIcon("r.png"));
		indexItem = new QStandardItem(QString::number((int)((GUIRow*)comp)->index));
	}
	else if(compType == "window")
	{
		nameItem->setIcon(QIcon("w.png"));
		indexItem = new QStandardItem(QString::number((int)0));
	}
	else if(compType == "GUIItem")
	{
		nameItem->setIcon(QIcon("i.png"));
		indexItem = new QStandardItem(QString::number((int)((GUIItem*)comp)->index));
	}

	row.push_back(nameItem);
	row.push_back(indexItem);
	row.push_back(tobescrapedItem);
	parent->appendRow(row);

	for(int i=0; i<comp->components.size(); i++)
	{
		setupModelData(comp->components[i], nameItem);
	}
}