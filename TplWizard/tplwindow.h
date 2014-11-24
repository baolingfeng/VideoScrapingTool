#ifndef TPLWINDOW_H
#define TPLWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include "ui_TplTool.h"
#include "ClsTpl.h"

using namespace std;

class TplWindow : public QMainWindow
{
	Q_OBJECT

public:
	TplWindow(QWidget *parent=0);
	~TplWindow();

	void paintWindow();
	void paintListRow();
	void loadTpls();

	vector<ClsTpl> tpls;
public slots:
	void tplSelected(const QModelIndex &);
	void appSelected(const QModelIndex &);
	void rowSelected(const QModelIndex &);
	void itemSelected(const QModelIndex &);

	void newTemplate();
	void newTemplateCopy();
	void deleteTemplate();

	void newApplication();
	void newApplicationCopy();

	void addRowBefore();
	void addRowAfter();
	void deleteARow();

	void addItem();
private:
	Ui::TplWindow ui;
	string tplFolder;
};

#endif // TPLWINDOW_H
