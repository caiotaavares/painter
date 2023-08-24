#ifndef MATDISPLAY_H
#define MATDISPLAY_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include <QMouseEvent>

class matDisplay : public QLabel
{
    Q_OBJECT

public:
    matDisplay(QWidget *parent = 0);

    void setDrawPixelMode(int newMode);
    int mode = 0;

//    QPoint firstClickPos;
//    QPoint mouse_pos;

protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
//    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void sendMousePosition(QPoint&);
    void mousePressed(QPoint&);
};

#endif // MATDISPLAY_H
