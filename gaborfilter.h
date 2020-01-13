#ifndef GABORFILTER_H
#define GABORFILTER_H


#include <QDialog>
#include <QLabel>
#include <QCloseEvent>
#include <QGraphicsView>
#include <opencv2/opencv.hpp>


namespace Ui {
class GaborFilter;
}

class GaborFilter : public QDialog
{
    Q_OBJECT

public:
    explicit GaborFilter(QWidget *parent = 0);
    ~GaborFilter();

public slots:
    void setIMG(cv::Mat inputIMG); // od kturych tomczak i żułkowski
    void setMask(cv::Mat Mask);
    void setMaskFactor(int MaskFactor);

signals:
    void valueGaborData(cv::Mat GaborReal, cv::Mat GaborImag, cv::Mat GaborMask);

private slots:
    void on_InputMaskFactorHorizontalSlider_sliderMoved(int position);

    void on_KernalSizeSpinBox_valueChanged(int arg1);

    void on_SigmaDoubleSpinBox_valueChanged(double arg1);

    void on_LambdaDoubleSpinBox_valueChanged(double arg1);

    void on_ThetaDoubleSpinBox_valueChanged(double arg1);

    void on_psiDoubleSpinBox_valueChanged(double arg1);

    void on_GammaDoubleSpinBox_valueChanged(const QString &arg1);

    void on_GammaDoubleSpinBox_valueChanged(double arg1);

    void on_KernelSizeWidthSpinBox_valueChanged(int arg1);

    void on_KernelSizeHeightSpinBox_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_InputMaskFactorSpinBox_valueChanged(int arg1);

    void on_GaborIMGCheckBox_stateChanged(int arg1);

    void reject();

private:
    void displayIMG();
    void displayIMG(QLabel* display,cv::Mat img,int cnvertType=CV_GRAY2RGB);
    void displayMask();
    void displayGaborIMG();
    void setMask();
    void doWork();
    void rePrint();
    void initControls();


private:
    Ui::GaborFilter *ui;
    cv::Mat inputIMG;
    cv::Mat inputMask;
    cv::Mat GaborIMG;
    cv::Mat GaborIMGReal;
    cv::Mat GaborIMGImag;
    cv::Mat GaborReal;
    cv::Mat GaborImag;
    cv::Mat GaborKernelReal;
    cv::Mat GaborKernelImag;


    int GaborFormat = CV_32F;
    int IMGFormat = CV_8U;

    int MaskFactor=1;


    int  posKernelSizeWidth = 21;
    int  posKernelSizeHeight = 11;

    // żrudło http://answers.opencv.org/question/50975/gabor-kernel-parameters-in-opencv/
//    int pos_kernel_size=3;
    double pos_sigma= 1.2;
    double pos_lm = 50;
    double pos_th = 45;
    double pos_gamma= 0;
    double pos_psi =0; //90;






};

#endif // GABORFILTER_H
