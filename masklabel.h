#ifndef MASKLABEL_H
#define MASKLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QTimer>

class MaskLabel : public QLabel
{
public:
    MaskLabel(QWidget *parent=0);

public slots:
    void startSpinning();
    void stopSpinning();

protected:
    void paintEvent(QPaintEvent *);
private:
    int angle;
    QTimer *timer;
    bool spinning;
    QPixmap spinImage;
    int padding;
};

#endif // MASKLABEL_H
