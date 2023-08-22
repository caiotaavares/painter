#include "matdisplay.h"

#include <QMessageBox>
#include <QPixelFormat>
#include <QPainter>

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
    static QPoint firstClickPos;
    if(ev->button() == Qt::LeftButton)
    {
        QPoint mouse_pos = ev->pos();

        // Se esta no modo de edição "Pixel"
        if (mode == 1)
        {
            QImage image = this->pixmap().toImage();
            if (mouse_pos.x() >= 0 && mouse_pos.x() < image.width() &&
                mouse_pos.y() >= 0 && mouse_pos.y() < image.height())
            {
                // Adicionar espessura e colr variáveis
                QRgb color = qRgb(255, 0, 0);

                image.setPixel(mouse_pos.x(), mouse_pos.y(), color);
                this->setPixmap(QPixmap::fromImage(image));
            }
        }

        // Modo de edição "Linha"
        if (mode == 2) {
            if (mode == 2) {
                if (!firstClickPos.isNull()) {
                    // If firstClickPos is not null, we already have the first click position
                    QImage image = this->pixmap().toImage();
                    QPainter painter(&image);

                    // Draw a line between firstClickPos and mouse_pos
                    painter.setPen(QPen(QColor(0, 0, 255), 2));  // Set line color and width
                    painter.drawLine(firstClickPos, mouse_pos);

                    this->setPixmap(QPixmap::fromImage(image));

                    firstClickPos = QPoint();  // Reset firstClickPos for the next line
                } else {
                    // Store the position of the first click
                    firstClickPos = mouse_pos;
                }
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
