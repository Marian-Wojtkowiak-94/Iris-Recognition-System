#ifndef GABORCODING_H
#define GABORCODING_H

#include <QDialog>
#include <QLabel>
#include <QCloseEvent>
#include <QGraphicsView>
#include <opencv2/opencv.hpp>
#include <math.h>

namespace Ui {
class GaborCoding;
}

class GaborCoding : public QDialog
{
    Q_OBJECT

public:
    explicit GaborCoding(QWidget *parent = 0);
    ~GaborCoding();
public slots:
    void setGaborData(cv::Mat GaborReal, cv::Mat GaborImag, cv::Mat GaborMask );

private slots:
    void on_HashAutoScaleCheckBox_stateChanged(int arg1);

private:
    void doWork();
    void doWorkHashAutoScale();
    void doEnabledSeries();
    void initControls();
    QString coding(double real,double imag);
    QString codingMask(char data);
    bool cvMat2stdVector(cv::Mat & src,std::vector<float> & dest);
    bool cvMat2stdVector(cv::Mat & src,std::vector<uchar> & dest);


private:
    Ui::GaborCoding *ui;
    cv::Mat GaborReal;
    cv::Mat GaborImag;
    cv::Mat GaborMask;
    bool HashAutoScale=true;
    int64_t HashLength=2048;

//    std::string Hash;
//    std::string HashMask;
    std::vector<float> GaborRealArray;
    std::vector<float> GaborImagArray;
    std::vector<uchar> GaborMaskArray;

    QString Hash;
    QString HashMask;


};

#endif // GABORCODING_H
