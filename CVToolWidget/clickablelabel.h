#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
	Q_OBJECT

public:
	ClickableLabel(QWidget *parent=0);
	//ClickableLabel(const QString& text, QWidget * parent);
	~ClickableLabel();

signals:
    void clicked();
 
protected:
    void mousePressEvent ( QMouseEvent * event ) ;
private:
	
};

#endif // CLICKABLELABEL_H
