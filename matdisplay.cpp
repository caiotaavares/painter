#include "matdisplay.h"

#include <QMessageBox>
#include <QPixelFormat>

matDisplay::matDisplay(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

void matDisplay::setDrawPixelMode(int newMode)
{
    mode = newMode;
}

void matDisplay::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint mouse_pos = ev->pos();
    if(mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height())
    {
        if(mouse_pos.x() > 0 && mouse_pos.y() > 0)
        {
            emit sendMousePosition(mouse_pos);
        }
    }
    QPoint mouse_pos_color = ev->pos();
    emit mousePressed(mouse_pos_color);
}

void matDisplay::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        // Se esta no modo de edição "Pixel"
        if (mode == 1)
        {
            QPoint mouse_pos = ev->pos();
            QImage image = this->pixmap().toImage();
            if (mouse_pos.x() >= 0 && mouse_pos.x() < image.width() &&
                mouse_pos.y() >= 0 && mouse_pos.y() < image.height())
            {
                // Adicionar espessura e color variáveis
                QRgb color = qRgb(255, 0, 0);

                image.setPixel(mouse_pos.x(), mouse_pos.y(), color);
                this->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
    if (ev->button() == Qt::RightButton)
    {
        QMessageBox msg;
        msg.setText("Right Mouse Button Pressed!");
        msg.exec();
    }
}
