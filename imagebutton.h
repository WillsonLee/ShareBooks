#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QPixmap>

class ImageButton : public QPushButton
{
public:
    ImageButton(QWidget *parent=NULL);
    void setImage(QPixmap pix);
    void setRadius(int r);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap icon;
    int radius;
};

#endif // IMAGEBUTTON_H
