#include "imagebutton.h"
#include <QPainter>

ImageButton::ImageButton(QWidget *parent):QPushButton(parent)
{

}

void ImageButton::setImage(QPixmap pix)
{
    this->icon=pix;
    this->update();
}

void ImageButton::setRadius(int r)
{
    this->radius=r;
    this->update();
}

void ImageButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(QRectF(0,0,this->width(),this->height()),icon,QRectF(0,0,icon.width(),icon.height()));
    int pen_width=1;
    painter.setPen(QPen(QBrush(Qt::black),pen_width));
    painter.drawRoundedRect(QRect(pen_width,pen_width,this->width()-pen_width*2,this->height()-pen_width*2),radius,radius);
}
