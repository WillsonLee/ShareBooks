#include "masklabel.h"
#include <QPainter>
#include <QBitmap>
#include <QDebug>

MaskLabel::MaskLabel(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(400,400);
}

void MaskLabel::paintEvent(QPaintEvent *e)
{
    if(pixmap()){
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QPainterPath path;
        int round = qMin(width(), height())/2;
        path.addEllipse(width()/2-round, height()/2-round, 2*round, 2*round);//center 2r*2r area
        painter.setClipPath(path);
        float ratio=qMin((float)pixmap()->width()/round,(float)pixmap()->height()/round);
        QPixmap pix_scaled=pixmap()->scaled(QSize(2*pixmap()->width()/ratio,2*pixmap()->height()/ratio));
        pix_scaled=pix_scaled.copy(pix_scaled.width()/2-round,0,2*round,2*round);
        painter.drawPixmap((width()-pix_scaled.width())/2, 0, 2*round, 2*round, pix_scaled);
    }else{
        QLabel::paintEvent(e);
    }
}
