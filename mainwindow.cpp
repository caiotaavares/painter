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

//            // Get the dimensions of the QLabel widget
//            int labelWidth = ui->mat_Display->width();
//            int labelHeight = ui->mat_Display->height();

//            // Resize the image to match the dimensions of the QLabel while maintaining aspect ratio
//            QImage scaledImage = image.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio);

//            // Convert the resized image to QPixmap and set it to the QLabel
//            ui->mat_Display->setPixmap(QPixmap::fromImage(scaledImage));
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
    // Seta o modo de ediçã como linha
    ui->mat_Display->setDrawPixelMode(2);
    QMessageBox msg;
    msg.setText("Line draw pressed!");
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

