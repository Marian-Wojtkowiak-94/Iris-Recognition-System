#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gaborfilter.h"
#include "gaborcoding.h"
#include <opencv2/opencv.hpp>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Garbor_clicked();
    void closeEvent (QCloseEvent *event);
    void on_MainWindow_destroyed();

private:
    Ui::MainWindow *ui;
    GaborFilter* gabor;
    GaborCoding* gaborCoding;
};

#endif // MAINWINDOW_H
