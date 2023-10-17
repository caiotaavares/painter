#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

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
 * COLOR PARAMS
 * @brief MainWindow::handleMouseColor
 * @param pos
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
    // Ensure H is within the range [0, 360]
    while (H < 0.0) {
        H += 360.0;
    }
    while (H >= 360.0) {
        H -= 360.0;
    }

    // Normalize S and L to [0, 1]
    S /= 100.0;
    L /= 100.0;

    double C = (1.0 - std::abs(2.0 * L - 1.0)) * S;
    double X = C * (1.0 - std::abs(std::fmod(H / 60.0, 2) - 1.0));
    double m = L - 0.5 * C;

    double R1, G1, B1;

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

    R = static_cast<int>((R1 + m) * 255);
    G = static_cast<int>((G1 + m) * 255);
    B = static_cast<int>((B1 + m) * 255);
}

/**
 * @brief MainWindow::handleMouseColor
 * @param pos
 */

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
            // Image displayed in the original resolution
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
    QMessageBox msg;
    msg.setText("Pixel draw pressed!");
    msg.exec();
}


void MainWindow::on_actionReta_triggered()
{
    // Seta o modo de ediçã como reta paramétrica
    ui->mat_Display->setDrawPixelMode(2);
    QMessageBox msg;
    msg.setText("Desenhando reta paramétrica!");
    msg.exec();
}


void MainWindow::on_actionAlgoritmo_de_Bresenham_triggered()
{
    // Seta o modo de edição como Brasenhan
    ui->mat_Display->setDrawPixelMode(3);
    QMessageBox msg;
    msg.setText("Brasenham draw pressed!");
    msg.exec();
}


void MainWindow::on_actionC_rculo_y_sqrt_R_R_x_x_triggered()
{
    // Seta o modo de edição como Círcunferência (y = sqrt(R*R – x *x))
    ui->mat_Display->setDrawPixelMode(4);
    QMessageBox msg;
    msg.setText("Círculo (y = sqrt(R*R – x *x))");
    msg.exec();
}


void MainWindow::on_actionCirculo_Equa_o_param_trica_triggered()
{
    ui->mat_Display->setDrawPixelMode(5);
    QMessageBox msg;
    msg.setText("Cícunferência (Paramétrica)");
    msg.exec();
}


void MainWindow::on_pushButtonRGBCalc_clicked()
{
    int r = ui->spinBoxR->value();
    int g = ui->spinBoxG->value();
    int b = ui->spinBoxB->value();
}


void MainWindow::on_actionReta_Param_trica_triggered()
{
    ui->mat_Display->setDrawPixelMode(6);
    QMessageBox msg;
    msg.setText("Reta (Paramétrica).");
    msg.exec();
}


void MainWindow::on_actionCircunfer_ncia_Brasenham_triggered()
{
    ui->mat_Display->setDrawPixelMode(7);
    QMessageBox msg;
    msg.setText("Circunferência (Brasenham).");
    msg.exec();
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

