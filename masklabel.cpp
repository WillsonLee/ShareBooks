#include "masklabel.h"
#include <QPainter>
#include <QBitmap>
#include <QDebug>

MaskLabel::MaskLabel(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(400,400);
    angle=0;
    timer=new QTimer(this);
    timer->setInterval(100);
    spinning=false;
    connect(timer,&QTimer::timeout,this,[&](){
       angle+=-10;
       angle=angle%360;
       update();
    });
    padding=2;
}

void MaskLabel::startSpinning()
{
    spinning=true;
    spinImage.load("../icons/wait.png");
    timer->start();
    update();
}

void MaskLabel::stopSpinning()
{
    spinning=false;
    timer->stop();
    update();
}

void MaskLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(pixmap()){
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
    if(spinning){
        painter.translate(width()/2,height()/2);
        painter.rotate(angle);
        painter.drawPixmap(-width()/2+padding,-height()/2+padding,width()-2*padding,height()-2*padding,spinImage);
    }
}
