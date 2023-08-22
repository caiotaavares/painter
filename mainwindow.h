#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <QMessageBox>

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
    void on_buton_load_image_clicked();

    void on_actionPixel_triggered();

    void on_actionReta_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
