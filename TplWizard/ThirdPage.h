#pragma once
#include <QWizardPage>
#include "ui_thirdpage.h"
#include <QImage>
#include <vector>

#include "ToolWindow.h"

class MyItem
{
public:
	MyItem()
	{
		//item = new QImage;
		//item_mask = new QImage;
	}

	QImage item;
	QImage item_mask;
};

class MyRow
{
public:
	int row;
	std::vector<MyItem> items;
};

class ThirdPage :
	public QWizardPage
{
	Q_OBJECT
public:
	ThirdPage(QWidget *parent=0);
	~ThirdPage(void);

	bool validatePage();
public slots:
	void rowSelected(const QModelIndex&  selected);
	void itemSelected(const QModelIndex&  selected);
	void addRow();
	void delRow();
	void addItem();
	void delItem();
	void showTool();
	void toolWindowClose();
	void appTypeSelected(QString index);
	void addItemMask();
	void addItemFromTool();

protected:
	void initializePage();
private:
	Ui::UI_ThirdPage ui;

	ToolWindow* tw;

	std::vector<MyRow> rows;
	int current_row;
	int current_item;
};