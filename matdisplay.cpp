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

/**
 * @brief matDisplay::mouseMoveEvent
 * @param ev
 * Evento de movimentação do mouse mostra a posição relativa
 * e o rgb de cada pixel.
 */
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

/**
 * @brief matDisplay::mousePressEvent
 * @param ev
 * Evento que armazena todas as operações gráficas e de desenho
 */
void matDisplay::mousePressEvent(QMouseEvent *ev)
{
    // Posição do primeiro clique
    static QPoint firstClickPos;
    if(ev->button() == Qt::LeftButton)
    {
        // armazena os pixels da imagem
        QImage image = this->pixmap().toImage();

        // armazena a imagem na classe QPainter
        QPainter painter(&image);

        // seta a cor e espessura a ser utilizada na classe painter
        painter.setPen(QPen(QColor(0, 0, 255), 2));


        // Posição do segundo clique
        QPoint mouse_pos = ev->pos();

        int cx = mouse_pos.x(); // Posição x do mouse no segundo clique
        int cy = mouse_pos.y(); // Posição y do mouse no segundo clique
        int fcx = firstClickPos.x(); // Posição x do mouse no primeiro clique
        int fcy = firstClickPos.y(); // Posição y do mouse no primeiro clique

        //
        // MODO DE DESENHO PIXEL
        //
        if (mode == 1) {
            if (fcx >= 0 && fcx < image.width() &&
                fcy >= 0 && fcy < image.height())
            {
                painter.drawPoint(fcx, fcy);
            }
        }

        // Se firstClickPost não é nulo, já existe uma coordenada do primeiro clique armazenada
        if (!firstClickPos.isNull()) {
            //
            // MODO DE DESENHO RETA
            //
            if (mode == 2) {
                // armazena os parâmetros a e b de "y = ax + b"
                double a = static_cast<double>(mouse_pos.y() - firstClickPos.y()) / (mouse_pos.x() - firstClickPos.x());
                double b = firstClickPos.y() - a * firstClickPos.x();

                // Desenha a linha usando y = ax + b
                for (int x = firstClickPos.x(); x <= mouse_pos.x(); ++x) {
                    int y = static_cast<int>(a * x + b);
                    // Usa drawPoint() APENAS PARA DESENHAR CADA PONTO DA RETA
                    // CALCULADA COM "y = ax + b".
                    painter.drawPoint(x, y);
                }
            }

            //
            // DESENHO DA RETA (ALGORITMO DE BRASENHAM)
            //
            if (mode == 3) {
                // Armazena os pontos x e y das extremidades da
                // abs() calcula o valor absoluto da operação passada
                int dx = abs(cx - fcx);
                int dy = abs(cy - fcy);

                // Determina a direção da linha sx e sy
                // com base na diferença de valores dos eixos
                // PERMITE REALIZAR OPERAÇÕES EM MULTIPLOS OCTANTES
                int sx = (cx < fcx) ? 1 : -1;
                int sy = (cy < fcy) ? 1 : -1;

                // erro
                int error = dx - dy;

                // Utiliza o algoritmo de Bresenham para desenhar os pixels
                while (true) {

                    // drawPoint() é usado apenas para desenhar cada pixel na coordenada
                    // edida
                    painter.drawPoint(cx, cy);

                    // Não desenha com extremidades no mesmo ponto
                    if (cx == fcx && cy == fcy) break;

                    int e2 = 2 * error;
                    if (e2 > -dy) {
                        error -= dy;
                        cx += sx;
                    }
                    if (e2 < dx) {
                        error += dx;
                        cy += sy;
                    }
                }
            }

            //
            // DESENHO DA CIRCUNFERÊNCIA (y = sqrt(R*R - x*x))
            //
            if (mode == 4) {
                int R = (cx - fcx) / 2; // Raio do círculo

                for (int x = -R; x <= R; x++) {
                    int y = static_cast<int>(sqrt(R * R - x * x));

                    // Desenha a cima do eixo X
                    if (cy + y >= 0 && cy + y < image.height()) {
                        painter.drawPoint(cx + x - R, cy + y);
                    }

                    // Desenha a baixo do eixo x
                    if (cy - y >= 0 && cy - y < image.height()) {
                        painter.drawPoint(cx + x - R, cy - y);
                    }
                }
            }

            //
            // DESENHO DA CIRCUNFERÊNCIA (EQUAÇÃO PARAMÉTRICA)
            //
            if (mode == 5) {
                int R = (cx - fcx) / 2; // Raio do círculo
                double a; // Ângulo

                // O ângulo itera até 2*pi em 0.001º
                for (a = 0; a <= 2 * M_PI; a += 0.001) {
                    int x = static_cast<int>(R * cos(a));
                    int y = static_cast<int>(R * sin(a));

                    if (cy + y >= 0 && cy + y < image.height()) {
                        painter.drawPoint(cx + x - R, cy + y);
                    }

                    if (cy - y >= 0 && cy - y < image.height()) {
                        painter.drawPoint(cx + x - R, cy - y);
                    }
                }
            }

            // Atualiza a imagem
            this->setPixmap(QPixmap::fromImage(image));

            // Reseta o clique
            firstClickPos = QPoint();

        } else {
            firstClickPos = mouse_pos;
        }
    }
    if (ev->button() == Qt::RightButton)
    {
        QMessageBox msg;
        msg.setText("Right Mouse Button Pressed!");
        msg.exec();
    }
}
