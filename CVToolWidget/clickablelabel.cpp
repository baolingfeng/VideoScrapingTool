#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent)
	: QLabel(parent)
{

}

/*
ClickableLabel::ClickableLabel(const QString& text="", QWidget * parent = 0) : QLabel(text, parent)
{
	//this->setText(text);
}
*/

ClickableLabel::~ClickableLabel()
{

}

 void ClickableLabel::mousePressEvent ( QMouseEvent * event ) 
 {
	const QPixmap* pm = this->pixmap();
	if(pm == NULL || pm->isNull())
	{
		return;
	}

	emit clicked();
 }