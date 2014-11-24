#ifndef WINCONFIG_H
#define WINCONFIG_H

#include <QWidget>
#include <QTreeView>
 #include <QStandardItemModel>
 #include <QItemSelectionModel>
#include <iostream>
#include <fstream>
#include <vector>
#include "ui_winconf.h"
#include "GUICompenent.h"
#include <hash_map>

using namespace std;

class WinConfig : public QWidget
{
	Q_OBJECT

public:
	WinConfig(QWidget *parent=0);
	~WinConfig();

	GUIComponent* getWindowDefinitionFromPath(string folder, string name);
	void setupModelData(GUIComponent* comp, QStandardItem* parent);

public slots:
	void componentSelected(const QModelIndex &index);

private:

	vector<GUIComponent*> components;
	hash_map<string, GUIComponent*> comp_map;
	Ui::winconf ui;
};

#endif // WINCONFIG_H
