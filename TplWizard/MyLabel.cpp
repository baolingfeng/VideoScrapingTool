#include "MyLabel.h"

MyLabel::MyLabel()
{
	m_isDown = false;
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	setScaledContents(true); 
	left = up = 0;
	//setScaledContents(true);
}

MyLabel::~MyLabel()
{
	//delete img;
}

void MyLabel::setImage(QImage img_in, int subIndex_in)
{
	this->img = img_in;
	subIndex = subIndex_in;
	left = up = 0;
}

void MyLabel::setSubIndex(int subIndex_in)
{
	subIndex = subIndex_in;
}

void MyLabel::setLeft(int step)
{
	int w = img.width();
	if(left+step < w/2 && left+step>=0)
	{
		left = left + step;
	}
}

void MyLabel::setUp(int step)
{
	int h = img.height();
	if(up+step<h/2 && up+step>=0)
	{
		up = up + step;
	}
}

int MyLabel::getSubIndex()
{
	return subIndex;
}

void MyLabel::mousePressEvent(QMouseEvent *e)
{
	if(e->button() && Qt::LeftButton)
	{
		m_isDown = true;
		m_start = e->pos();
		m_stop = e->pos();
	}
}

void MyLabel::mouseMoveEvent(QMouseEvent *e)
{
	if(m_isDown)
	{
		m_stop = e->pos();
	}
	update();
}

void MyLabel::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() && Qt::LeftButton){
		m_isDown = false;
	}
}

void MyLabel::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	//this->setPixmap(QPixmap::fromImage(*img));

	if(!img.isNull())
	{
		QPainter p(this);

		/*
		int left, up;
		if(subIndex == 1 )
		{
			left = 0;
			up = 0;
		}
		else if(subIndex == 2)
		{
			left = img.width()/2;
			up = 0;
		}
		else if(subIndex == 3)
		{
			left = 0;
			up = img.height()/2;
		}
		else if(subIndex == 4)
		{
			left = img.width()/2;
			up = img.height()/2;
		}
		*/

		QImage subImg = img.copy(left,up,img.width()/2,img.height()/2);
		
		p.drawImage(0,0,subImg);

		if(!m_isDown)
		{
			return;
		}
		p.setPen(Qt::red);
		p.drawRect(QRect(m_start,m_stop));

		emit rectSelected(QRect(m_start,m_stop), subIndex);
	}
	else
	{
		QImage img;
		img.load("noimage.png");
		p.drawImage(0,0,img);
	}
	
}

