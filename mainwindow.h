#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <QMessageBox>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showMousePosition(QPoint &pos);
    void handleMouseColor(QPoint &pos);

private slots:
    void RGBToHSL(int R, int G, int B, double &H, double &S, double &L);

    void HSLToRGB(double H, double S, double L, int &R, int &G, int &B);

    void on_buton_load_image_clicked();

    void on_actionPixel_triggered();

    void on_actionReta_triggered();

    void on_actionAlgoritmo_de_Bresenham_triggered();

    void on_actionC_rculo_y_sqrt_R_R_x_x_triggered();

    void on_actionCirculo_Equa_o_param_trica_triggered();

    void on_pushButtonRGBCalc_clicked();

    void on_actionReta_Param_trica_triggered();

    void on_actionCircunfer_ncia_Brasenham_triggered();

    void on_spinBoxR_valueChanged(int arg1);

    void on_spinBoxG_valueChanged(int arg1);

    void on_spinBoxB_valueChanged(int arg1);

    void on_doubleSpinBoxH_valueChanged(double arg1);

    void on_doubleSpinBoxS_valueChanged(double arg1);

    void on_doubleSpinBoxL_valueChanged(double arg1);

    void on_pushButtonCalcHSL_clicked();

    void on_pushButtonCalcRGB_clicked();

    void on_actionPreto_e_Branco_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
