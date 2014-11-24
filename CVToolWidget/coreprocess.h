#ifndef COREPROCESS_H
#define COREPROCESS_H

#include <QThread>

class CoreProcess : public QThread
{
	Q_OBJECT

public:
	CoreProcess(QObject *parent=0);
	~CoreProcess();

	void init(QString tplFolder, QString imgFolder, QString outputFolder, int numOfFrame);

	void run();

public slots:
	
signals:
	void frameStart(int frameNum);
	void edgeFinish(int frameNum);
	void vcFinish(int frameNum);
	void allFinish(int frameNum);

private:
	QString tplFolder;
	QString imgFolder;
	QString outputFolder;
	int numOfFrame;

};

#endif // COREPROCESS_H
