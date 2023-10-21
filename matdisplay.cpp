#include "matdisplay.h"

#include <QMessageBox>
#include <QPixelFormat>
#include <QPainter>

matDisplay::matDisplay(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);

    // Configura o tamanho do QLabel e cria uma imagem branca
    QSize size(571, 561);
    QImage image(size, QImage::Format_RGB32);
    image.fill(Qt::white); // Preenche a imagem com branco

    // Define a imagem inicial no QLabel
    this->setPixmap(QPixmap::fromImage(image));
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

    // armazena os pixels da imagem
    QImage image = this->pixmap().toImage();

    // armazena a imagem na classe QPainter
    QPainter painter(&image);

    // seta a cor e espessura a ser utilizada na classe painter
    painter.setPen(QPen(QColor(0, 0, 255), 2));

    if (mode == 1) {
        //
        // MODO DE DESENHO PIXEL
        //
        if (ev->button() == Qt::LeftButton) {
            // MODO DE DESENHO PIXEL
            painter.drawPoint(ev->pos().x(), ev->pos().y());

            // Atualiza a imagem
            this->setPixmap(QPixmap::fromImage(image));
        }
    } else {
        if(ev->button() == Qt::LeftButton)
        {
            // Se firstClickPost não é nulo, já existe uma coordenada do primeiro clique armazenada
            if (!firstClickPos.isNull()) {

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
            // MODO DE DESENHO RETA
            //
            if (mode == 2) {
                double m = static_cast<double>(fcy - cy) / (fcx - cx);
                int x = cx; // Comece a partir da coordenada x inicial
                int y = cy;

                // Verifica se Dx é maior do que Dy
                if (fabs(fcx - cx) >= fabs(fcy - cy)) {
                    // Incrementa/decrementa x e calcula y
                    int inc = (fcx > cx) ? 1 : -1;

                    while (x != fcx) {
                        painter.drawPoint(x, y);
                        x += inc;
                        // Calcula a coordenada y com base na inclinação da reta.
                        y = static_cast<int>(m * (x - cx) + cy);
                    }
                } else {
                    // Incrementa/decrementa y e calcula x
                    int inc = (fcy > cy) ? 1 : -1;

                    while (y != fcy) {
                        painter.drawPoint(x, y);
                        y += inc;
                        // Calcula a coordenada x com base na inclinação da reta.
                        x = static_cast<int>((y - cy) / m + cx);
                    }
                }
            }

            //
            // DESENHO DA RETA PARAMÉTRICA
            //
            if (mode == 6) {
                int x1 = cx;
                int y1 = cy;
                int x2 = fcx;
                int y2 = fcy;

                int dx = x2 - x1;
                int dy = y2 - y1;

                int steps;
                if (abs(dx) > abs(dy)) {
                    steps = abs(dx);
                } else {
                    steps = abs(dy);
                }

                double xIncrement = static_cast<double>(dx) / steps;
                double yIncrement = static_cast<double>(dy) / steps;

                double x = x1;
                double y = y1;

                for (int i = 0; i <= steps; i++) {
                    painter.drawPoint(static_cast<int>(x), static_cast<int>(y));
                    x += xIncrement;
                    y += yIncrement;
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
                int R = static_cast<int>(sqrt(pow(cx - fcx, 2) + pow(cy - fcy, 2))); // Calcular o raio com base na distância entre os dois cliques

                for (int x = -R; x <= R; x++) {
                    int y = static_cast<int>(sqrt(R * R - x * x));

                    // Desenha acima do eixo X
                    if (cy + y >= 0 && cy + y < image.height()) {
                        painter.drawPoint(cx + x, cy + y);
                    }

                    // Desenha abaixo do eixo x
                    if (cy - y >= 0 && cy - y < image.height()) {
                        painter.drawPoint(cx + x, cy - y);
                    }
                }
            }

            //
            // DESENHO DA CIRCUNFERÊNCIA (EQUAÇÃO PARAMÉTRICA)
            //
            if (mode == 5) {
                int R = static_cast<int>(sqrt(pow(cx - fcx, 2) + pow(cy - fcy, 2))); // Calcular o raio com base na distância entre os dois cliques
                double a; // Ângulo

                // O ângulo itera até 2*pi em 0.001º
                for (a = 0; a <= 2 * M_PI; a += 0.001/R) {
                    int x = static_cast<int>(R * cos(a));
                    int y = static_cast<int>(R * sin(a));

                    if (cy + y >= 0 && cy + y < image.height()) {
                        painter.drawPoint(cx + x, cy + y);
                    }

                    if (cy - y >= 0 && cy - y < image.height()) {
                        painter.drawPoint(cx + x, cy - y);
                    }
                }
            }
            //
            // Circunferência (Brasenham)
            //
            if (mode == 7) {
                int R = static_cast<int>(sqrt(pow(cx - fcx, 2) + pow(cy - fcy, 2))); // Calcular o raio com base na distância entre os dois cliques
                int x = 0;
                int y = R;
                int h = 1 - R;
                int dE = 3;
                int dSE = -2 * R + 5;

                // Desenha o primeiro ponto em cada octante
                painter.drawPoint(cx + x, cy + y);
                painter.drawPoint(cx - x, cy + y);
                painter.drawPoint(cx + x, cy - y);
                painter.drawPoint(cx - x, cy - y);
                painter.drawPoint(cx + y, cy + x);
                painter.drawPoint(cx - y, cy + x);
                painter.drawPoint(cx + y, cy - x);
                painter.drawPoint(cx - y, cy - x);
                while (x < y) {
                    if (h < 0) { // Seleciona E
                        h = h + dE;
                        dE = dE + 2;
                        dSE = dSE + 2;
                    } else { // Seleciona SE
                        h = h + dSE;
                        dE = dE + 2;
                        dSE = dSE + 4;
                        y = y - 1;
                    }
                    x = x + 1;
                    // Desenha um ponto em cada octante
                    painter.drawPoint(cx + x, cy + y);
                    painter.drawPoint(cx - x, cy + y);
                    painter.drawPoint(cx + x, cy - y);
                    painter.drawPoint(cx - x, cy - y);
                    painter.drawPoint(cx + y, cy + x);
                    painter.drawPoint(cx - y, cy + x);
                    painter.drawPoint(cx + y, cy - x);
                    painter.drawPoint(cx - y, cy - x);
                }
            }

            // Atualiza a imagem
            this->setPixmap(QPixmap::fromImage(image));

            // Reseta o clique
            firstClickPos = QPoint();

            } else {
                firstClickPos = ev->pos();
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
