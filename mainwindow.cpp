#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->gabor=new GaborFilter();
    this->gaborCoding=new GaborCoding();
    connect(this->gabor,SIGNAL(valueGaborData(cv::Mat,cv::Mat,cv::Mat)),this->gaborCoding,SLOT(setGaborData(cv::Mat,cv::Mat,cv::Mat)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->gabor;
    delete this->gaborCoding;
    cv::destroyAllWindows();
}

void MainWindow::on_Garbor_clicked()
{
//    cv::Mat frame2 = cv::imread("/home/marian/Studia/projekt_przejsciowy_2/przykład.png");
//    cv::Mat frame2 = cv::imread("/home/marian/Studia/projekt_przejsciowy_2/irisExample.jpg");
    //cv::Mat frame2 = cv::imread("/home/marian/Obrazy/a/Internet Explorer Wallpaper.jpg");
    cv::Mat frame2 = cv::imread("/home/marian/Studia/projekt_przejsciowy_2/normalizacja/wyjscie/norm0.png");

    this->gabor->setIMG(frame2);
    this->gabor->show();
    this->gaborCoding->show();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    delete this->gabor;
    delete this->gaborCoding;
    event->accept();
}

void MainWindow::on_MainWindow_destroyed()
{
    cv::destroyAllWindows();
    delete this->gabor;
}
