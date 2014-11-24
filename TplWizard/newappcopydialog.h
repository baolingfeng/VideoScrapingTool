#ifndef NEWAPPCOPYDIALOG_H
#define NEWAPPCOPYDIALOG_H

#include <QDialog>
#include "tplwindow.h"
#include "ui_NewApplicationCopyDialog.h"
#include <vector>

class NewAppCopyDialog : public QDialog
{
	Q_OBJECT

public:
	NewAppCopyDialog(TplWindow *parent);
	~NewAppCopyDialog();

	void accept();

	QString getAppName();
	int getCopyTplIndex();
	int getCopyAppIndex();
public slots:
	void getAppList(int tplIndex);

private:
	Ui::NewApplicationCopy ui;

	vector<ClsTpl> tpls;
};

#endif // NEWAPPCOPYDIALOG_H
