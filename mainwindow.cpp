#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpen.h"

#include <QMessageBox>
#include <QPainter>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // matdisplay --> mainwindow
    connect(ui->mat_Display, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));
    connect(ui->mat_Display, SIGNAL(mousePressed(QPoint&)), this, SLOT(handleMouseColor(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_position_label->setText("x: " + QString::number(pos.x()) + ",y: " + QString::number(pos.y()));
}

/**
 * MANIPULAÇÃO DE CORES
 * @brief MainWindow::RGBToHSL
 * @param R
 * @param G
 * @param B
 * @param H
 * @param S
 * @param L
 */
void MainWindow::RGBToHSL(int R, int G, int B, double &H, double &S, double &L) {
    // Normaliza os valores de RGB para [0, 1]
    double r = R / 255.0;
    double g = G / 255.0;
    double b = B / 255.0;

    double maxVal = std::max({r, g, b});
    double minVal = std::min({r, g, b});

    // Calcula o L em %
    L = ((maxVal + minVal) / 2.0) * 100.0;

    if (maxVal == minVal) {
        // Escala de cinza
        H = S = 0.0;
    } else {
        double d = maxVal - minVal;

        // Calcula a saturação em %
        S = L > 50.0 ? (d / (2.0 - maxVal - minVal)) * 100.0 : (d / (maxVal + minVal)) * 100.0;

        // Calcula o H em graus
        if (maxVal == r) {
            H = (g - b) / d + (g < b ? 6.0 : 0.0);
        } else if (maxVal == g) {
            H = (b - r) / d + 2.0;
        } else {
            H = (r - g) / d + 4.0;
        }

        H *= 60.0;
        if (H < 0) {
            H += 360.0;
        }
    }
}

void MainWindow::HSLToRGB(double H, double S, double L, int &R, int &G, int &B) {
    // Garante que o H está entre [0, 360]
    while (H < 0.0) {
        H += 360.0;
    }
    while (H >= 360.0) {
        H -= 360.0;
    }

    // Normaliza o L e o S para [0, 1]
    S /= 100.0;
    L /= 100.0;

    double C = (1.0 - std::abs(2.0 * L - 1.0)) * S;
    double X = C * (1.0 - std::abs(std::fmod(H / 60.0, 2) - 1.0));
    double m = L - 0.5 * C;

    double R1, G1, B1;

    // Cálculo
    if (H >= 0 && H < 60) {
        R1 = C;
        G1 = X;
        B1 = 0.0;
    } else if (H >= 60 && H < 120) {
        R1 = X;
        G1 = C;
        B1 = 0.0;
    } else if (H >= 120 && H < 180) {
        R1 = 0.0;
        G1 = C;
        B1 = X;
    } else if (H >= 180 && H < 240) {
        R1 = 0.0;
        G1 = X;
        B1 = C;
    } else if (H >= 240 && H < 300) {
        R1 = X;
        G1 = 0.0;
        B1 = C;
    } else {
        R1 = C;
        G1 = 0.0;
        B1 = X;
    }

    // Definição das cores
    R = static_cast<int>((R1 + m) * 255);
    G = static_cast<int>((G1 + m) * 255);
    B = static_cast<int>((B1 + m) * 255);
}

void MainWindow::handleMouseColor(QPoint &pos)
{
    QImage displayedImage = ui->mat_Display->pixmap().toImage();
    if (!displayedImage.isNull() && displayedImage.valid(pos))
    {
        QColor pixelColor = QColor(displayedImage.pixel(pos));
        int red = pixelColor.red();
        int green = pixelColor.green();
        int blue = pixelColor.blue();

        // Mostra os valores de RGB coletados
        ui->pixel_color_label->setText("R:" + QString::number(red) + ",G:" + QString::number(green) + ",B:" + QString::number(blue));

        // Pinta o label com as cores do pixel
        QString style = QString("background-color: rgb(%1, %2, %3);").arg(red).arg(green).arg(blue);
        ui->colored_label->setStyleSheet(style);
    }
}

void MainWindow::on_buton_load_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg)"));

    if (QString::compare(filename, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(filename);

        if (valid)
        {
            // Image na resolução original
            ui->mat_Display->setPixmap(QPixmap::fromImage(image));
        }
        else
        {
            QMessageBox msg;
            msg.setText("Error!");
            msg.exec();
        }
    }
}


void MainWindow::on_actionPixel_triggered()
{
    // Seta o modo de edição como Pixel
    ui->mat_Display->setDrawPixelMode(1);
//    QMessageBox msg;
//    msg.setText("Pixel draw pressed!");
//    msg.exec();
}


void MainWindow::on_actionReta_triggered()
{
    // Seta o modo de edição como reta paramétrica
    ui->mat_Display->setDrawPixelMode(2);
//    QMessageBox msg;
//    msg.setText("Desenhando reta paramétrica!");
//    msg.exec();
}


void MainWindow::on_actionAlgoritmo_de_Bresenham_triggered()
{
    // Seta o modo de edição como Reta de Brasenhan
    ui->mat_Display->setDrawPixelMode(3);
//    QMessageBox msg;
//    msg.setText("Brasenham draw pressed!");
//    msg.exec();
}


void MainWindow::on_actionC_rculo_y_sqrt_R_R_x_x_triggered()
{
    // Seta o modo de edição como Círcunferência (y = sqrt(R*R – x *x))
    ui->mat_Display->setDrawPixelMode(4);
//    QMessageBox msg;
//    msg.setText("Círculo (y = sqrt(R*R – x *x))");
//    msg.exec();
}


void MainWindow::on_actionCirculo_Equa_o_param_trica_triggered()
{
    // Seta o modo de edição como Círcunferência (Paramétrica)
    ui->mat_Display->setDrawPixelMode(5);
//    QMessageBox msg;
//    msg.setText("Cícunferência (Paramétrica)");
//    msg.exec();
}


void MainWindow::on_pushButtonRGBCalc_clicked()
{

}


void MainWindow::on_actionReta_Param_trica_triggered()
{
    ui->mat_Display->setDrawPixelMode(6);
//    QMessageBox msg;
//    msg.setText("Reta (Paramétrica).");
//    msg.exec();
}


void MainWindow::on_actionCircunfer_ncia_Brasenham_triggered()
{
    ui->mat_Display->setDrawPixelMode(7);
//    QMessageBox msg;
//    msg.setText("Circunferência (Brasenham).");
//    msg.exec();
}

void MainWindow::on_pushButtonCalcHSL_clicked()
{
    double H, S, L;
    RGBToHSL(ui->spinBoxR->value(), ui->spinBoxG->value(), ui->spinBoxB->value(), H, S, L);
    ui->doubleSpinBoxH->setValue(H);
    ui->doubleSpinBoxS->setValue(S);
    ui->doubleSpinBoxL->setValue(L);
}


void MainWindow::on_pushButtonCalcRGB_clicked()
{
    int R, G, B;
    HSLToRGB(ui->doubleSpinBoxH->value(), ui->doubleSpinBoxS->value(), ui->doubleSpinBoxL->value(), R, G, B);
    ui->spinBoxR->setValue(R);
    ui->spinBoxG->setValue(G);
    ui->spinBoxB->setValue(B);
}

void MainWindow::on_spinBoxG_valueChanged(int) {}
void MainWindow::on_spinBoxB_valueChanged(int) {}
void MainWindow::on_spinBoxR_valueChanged(int) {}
void MainWindow::on_doubleSpinBoxH_valueChanged(double) {}
void MainWindow::on_doubleSpinBoxS_valueChanged(double) {}
void MainWindow::on_doubleSpinBoxL_valueChanged(double) {}


void MainWindow::on_actionPreto_e_Branco_triggered()
{
    QPixmap pixmap = ui->mat_Display->pixmap();
    QImage image = pixmap.toImage();

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QRgb pixelColor = image.pixel(x, y);
            int grayValue = qGray(pixelColor);
            image.setPixelColor(x, y, QColor(grayValue, grayValue, grayValue));
        }
    }

    ui->mat_Display->setPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_actionNegativa_triggered()
{
    int R, G, B;
    QPixmap pixmap = ui->mat_Display->pixmap();
    QImage image = pixmap.toImage();

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QRgb pixel = image.pixel(x, y);
            R = 255 - qRed(pixel);
            G = 255 - qGray(pixel);
            B = 255 - qBlue(pixel);
            QRgb newPixel = qRgb(R, G, B);
            image.setPixel(x, y, newPixel);
        }
    }

    ui->mat_Display->setPixmap(QPixmap::fromImage(image));
}

// Função para calcular o centro do objeto
QVector3D MainWindow::CalculateObjectCenter(QVector3D* points, int numPoints) {
    QVector3D center(0, 0, 0);
    for (int i = 0; i < numPoints; i++) {
        center += points[i];
    }
    center /= numPoints;
    return center;
}

void MainWindow::on_pushButtonCreateHouse_clicked()
{
    QPixmap pixmap = ui->mat_Display->pixmap();
    QImage image = pixmap.toImage();
    QPainter painter(&image);

    // Fatores de escala global e local
    float globalScale = ui->doubleSpinBoxGlobalScale->value();  // Escala global
    QVector3D localScale(ui->doubleSpinBoxLocalScaleX->value(), ui->doubleSpinBoxLocalScaleY->value(), ui->doubleSpinBoxLocalScaleZ->value()); // Escala local

    // Defina os fatores de translação em X, Y e Z
    float translateX = ui->doubleSpinBoxTranslateX->value();
    float translateY = ui->doubleSpinBoxTranslateY->value();
    float translateZ = ui->doubleSpinBoxTranslateZ->value();

    // Variáveis para rotação
    float angle = ui->doubleSpinBoxAngle->value();  // Ângulo da rotação em graus
    char rotationAxis = ui->comboBoxRotation->currentText().toLower().toLatin1()[0]; // Eixo de rotação: 'x', 'y' ou 'z'
    bool origem = ui->checkBoxOrigem->isChecked(); // Verifique se a rotação deve ser em torno da origem ou do centro do objeto

    // Crie uma matriz de escala que combina as escalas local e global
    QMatrix4x4 scaleMatrix;
    scaleMatrix.scale(globalScale * localScale.x(), globalScale * localScale.y(), globalScale * localScale.z());

    // Crie uma matriz de translação
    QMatrix4x4 translationMatrix;
    translationMatrix.translate(translateX, translateY, translateZ);

    // Pontos 3D
    QVector3D points3D[11];

    // Defina as coordenadas dos pontos 3D
    points3D[0] = QVector3D(0, 200, 0);
    points3D[1] = QVector3D(100, 200, 0);
    points3D[2] = QVector3D(100, 100, 0);
    points3D[3] = QVector3D(0, 100, 0);
    points3D[4] = QVector3D(50, 50, 0);
    points3D[5] = QVector3D(120, 170, 100);
    points3D[6] = QVector3D(220, 170, 100);
    points3D[7] = QVector3D(220, 70, 100);
    points3D[8] = QVector3D(120, 70, 100);
    points3D[9] = QVector3D(170, 20, 100);
    points3D[10] = QVector3D(0, 0, 0); // Ponto de referência

    // Aplicar rotação em torno da origem ou do centro do objeto
    QVector3D center(0, 0, 0);  // Suponha que o centro do objeto está em (0, 0, 0)
    QMatrix4x4 rotationMatrix;

    if (origem) {
        // Rotação em torno da origem
        if (rotationAxis == 'x') {
            rotationMatrix.rotate(angle, 1, 0, 0);
        } else if (rotationAxis == 'y') {
            rotationMatrix.rotate(angle, 0, 1, 0);
        } else if (rotationAxis == 'z') {
            rotationMatrix.rotate(angle, 0, 0, 1);
        }
    } else {
        // Rotação em torno do centro do objeto
        center = CalculateObjectCenter(points3D, 11);  // Função para calcular o centro
        QMatrix4x4 translateToOriginMatrix;
        translateToOriginMatrix.translate(-center.x(), -center.y(), -center.z());

        QMatrix4x4 translateBackMatrix;
        translateBackMatrix.translate(center.x(), center.y(), center.z());

        if (rotationAxis == 'x') {
            rotationMatrix.rotate(angle, 1, 0, 0);
        } else if (rotationAxis == 'y') {
            rotationMatrix.rotate(angle, 0, 1, 0);
        } else if (rotationAxis == 'z') {
            rotationMatrix.rotate(angle, 0, 0, 1);
        }

        rotationMatrix = translateBackMatrix * rotationMatrix * translateToOriginMatrix;
    }

    // Combine todas as transformações em uma única matriz de modelo
    QMatrix4x4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    // Aplicar a matriz de modelo a todos os pontos 3D
    for (int i = 0; i < 11; i++) {
        points3D[i] = modelMatrix * points3D[i];
    }

    // Define a matriz de projeção (projeção ortográfica)
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();

    // Aplique a translação à matriz de projeção
    projectionMatrix = translationMatrix * projectionMatrix;

    // Projetar pontos 3D no plano 2D (z = 0) com escalas e translação
    QVector2D points2D[11];
    for (int i = 0; i < 11; i++) {
        QVector3D point3D = points3D[i];

        // Aplica a escala diretamente na matriz de projeção
        point3D = scaleMatrix * point3D;

        QVector4D point4D(point3D.x(), point3D.y(), point3D.z(), 1.0);
        point4D = projectionMatrix.map(point4D);
        points2D[i] = QVector2D(point4D.x(), point4D.y());
    }

    // Define a cor e espessura da linha
    painter.setPen(QPen(QColor(0, 0, 255), 2));

    // Desenha as linhas entre os pontos 2D
    painter.drawLine(points2D[0].toPoint(), points2D[1].toPoint());
    painter.drawLine(points2D[1].toPoint(), points2D[2].toPoint());
    painter.drawLine(points2D[3].toPoint(), points2D[0].toPoint());
    painter.drawLine(points2D[3].toPoint(), points2D[4].toPoint());
    painter.drawLine(points2D[4].toPoint(), points2D[2].toPoint());

    painter.drawLine(points2D[5].toPoint(), points2D[8].toPoint());
    painter.drawLine(points2D[8].toPoint(), points2D[9].toPoint());
    painter.drawLine(points2D[9].toPoint(), points2D[7].toPoint());
    painter.drawLine(points2D[7].toPoint(), points2D[6].toPoint());
    painter.drawLine(points2D[6].toPoint(), points2D[5].toPoint());

    painter.drawLine(points2D[0].toPoint(), points2D[5].toPoint());
    painter.drawLine(points2D[1].toPoint(), points2D[6].toPoint());
    painter.drawLine(points2D[3].toPoint(), points2D[8].toPoint());
    painter.drawLine(points2D[2].toPoint(), points2D[7].toPoint());
    painter.drawLine(points2D[4].toPoint(), points2D[9].toPoint());

    // Atualiza a imagem exibida no QLabel
    ui->mat_Display->setPixmap(QPixmap::fromImage(image));
}
