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

        // Modo de edição "Pixel"
        if (mode == 1)
        {
            QImage image = this->pixmap().toImage();
            if (mouse_pos.x() >= 0 && mouse_pos.x() < image.width() &&
                mouse_pos.y() >= 0 && mouse_pos.y() < image.height())
            {
                // Adicionar espessura e colr variáveis
                QRgb color = qRgb(255, 0, 0);

                // USANDO setPixel()
                image.setPixel(mouse_pos.x(), mouse_pos.y(), color);
                this->setPixmap(QPixmap::fromImage(image));
            }
        }

        // Modo de edição "Linha"
        if (mode == 2) {
            // Se firstClickPost não é nulo, já existe uma coordenada do primeiro clique armazenada
            if (!firstClickPos.isNull()) {

                // armazena os pixels da imagem
                QImage image = this->pixmap().toImage();
                QPainter painter(&image);

                // armazena os parâmetros a e b de "y = ax + b"
                double a = static_cast<double>(mouse_pos.y() - firstClickPos.y()) / (mouse_pos.x() - firstClickPos.x());
                double b = firstClickPos.y() - a * firstClickPos.x();

                // Desenha a linha usando y = ax + b
                painter.setPen(QPen(QColor(0, 0, 255), 2));  // Escolhe a cor da linha
                for (int x = firstClickPos.x(); x <= mouse_pos.x(); ++x) {
                    int y = static_cast<int>(a * x + b);
                    // Usa drawPoint() APENAS PARA DESENHAR CADA PONTO DA RETA
                    // CALCULADA COM "y = ax + b".
                    painter.drawPoint(x, y);
                }

                // Atualiza a imagem
                this->setPixmap(QPixmap::fromImage(image));

                // Reseta o clique
                firstClickPos = QPoint();
            } else {
                // Store the position of the first click
                firstClickPos = mouse_pos;
            }
        }

        // Modo de edição "Algoritmo de Bresenham"
        if (mode == 3) {
            if (!firstClickPos.isNull()) {
                QImage image = this->pixmap().toImage();
                QPainter painter(&image);

                int x1 = firstClickPos.x();
                int y1 = firstClickPos.y();
                int x2 = mouse_pos.x();
                int y2 = mouse_pos.y();

                // Calcula o valor de dx and dy
                int dx = abs(x2 - x1);
                int dy = abs(y2 - y1);

                // Determina a direção da linha sx e sy
                int sx = (x1 < x2) ? 1 : -1;
                int sy = (y1 < y2) ? 1 : -1;

                // erro
                int err = dx - dy;

                // Utiliza o algoritmo de Bresenham para desenhar os pixels
                while (true) {
                    /**
                     * drawPoint() é usado apenas para desenhar cada pixelna coordenada
                     * Pedida
                     **/
                    painter.drawPoint(x1, y1);

                    if (x1 == x2 && y1 == y2)
                        break;

                    int e2 = 2 * err;
                    if (e2 > -dy) {
                        err -= dy;
                        x1 += sx;
                    }
                    if (e2 < dx) {
                        err += dx;
                        y1 += sy;
                    }
                }

                this->setPixmap(QPixmap::fromImage(image));
                firstClickPos = QPoint();
            } else {
                firstClickPos = mouse_pos;
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
