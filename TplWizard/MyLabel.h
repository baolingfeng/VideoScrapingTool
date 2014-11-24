#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>


class MyLabel : public QLabel
{
	Q_OBJECT
public:
	MyLabel();
	~MyLabel();
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *);

public:
	void setImage(QImage img_in, int subIndex_in=1);
	
	int getSubIndex();
	void setSubIndex(int subIndex_in);
	void setLeft(int step);
	void setUp(int step);

signals:
	// void explains();  
	void rectSelected(QRect,int);
private:
	QImage img;

	int subIndex;

	int left;
	int up;

	bool m_isDown;
	QPoint m_start;
	QPoint m_stop;
};