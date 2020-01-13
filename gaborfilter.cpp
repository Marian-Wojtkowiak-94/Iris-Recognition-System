#include "gaborfilter.h"
#include "ui_gaborfilter.h"
#include <QtDebug>
#include <math.h>

GaborFilter::GaborFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaborFilter)
{
    ui->setupUi(this);

    this->rePrint();

}

GaborFilter::~GaborFilter()
{
    cv::destroyAllWindows();
    delete ui;
}

void GaborFilter::setIMG(cv::Mat inputIMG)
{

    if(inputIMG.channels()!=1){
        qDebug()<<inputIMG.channels();
        try{
            cv::cvtColor(inputIMG,this->inputIMG,CV_BGR2GRAY);
        }
        catch(...){

        }
//        this->inputIMG=inputIMG;
    }
    else{
        this->inputIMG=inputIMG;
    }
    qDebug()<<this->inputIMG.channels();
    this->setMask();
    this->rePrint();
    this->initControls();
    this->doWork();
}

void GaborFilter::setMask(cv::Mat Mask)
{
    if(Mask.channels()!=1){
        qDebug()<<Mask.channels();
        try{
            cv::cvtColor(Mask,this->inputMask,CV_BGR2GRAY);
        }
        catch(...){

        }
//        this->inputIMG=inputIMG;
    }
    else{
        this->inputMask=Mask;
    }
    qDebug()<<this->inputMask.channels();
    this->displayMask();
    this->doWork();
}





void GaborFilter::setMaskFactor(int MaskFactor)
{
    this->MaskFactor=MaskFactor;
    this->setMask();
}




void GaborFilter::displayIMG()
{
//    QSize rozmiar = ui->InputIMG->size();
//    cv::Mat frame;
//    cv::resize(this->inputIMG,frame, cv::Size(rozmiar.width(),rozmiar.height()),0,0,cv::INTER_CUBIC);
//    cv::cvtColor(frame,frame,CV_BGR2RGB);
//    QImage image((uchar*)frame.data, frame.cols, frame.rows,frame.step, QImage::Format_RGB888);
//    ui->InputIMG->setPixmap(QPixmap::fromImage(image));
    this->displayIMG(ui->InputIMG,this->inputIMG);
}

void GaborFilter::displayIMG(QLabel * display, cv::Mat img,int cnvertType)
{
    try{
    QSize rozmiar = display->size();
    cv::Mat frame,frame2;
    cv::resize(img,frame2, cv::Size(rozmiar.width(),rozmiar.height()),0,0,cv::INTER_CUBIC);
//    cv::cvtColor(frame,frame,CV_BGR2RGB);
//    cv::cvtColor(frame2,frame,CV_GRAY2RGB);
    cv::cvtColor(frame2,frame,cnvertType);
    QImage image((uchar*)frame.data, frame.cols, frame.rows,frame.step, QImage::Format_RGB888);
    display->setPixmap(QPixmap::fromImage(image));
    }
    catch(...){
      qDebug()<<"displayIMG(QLabel * display, cv::Mat img,int cnvertType) error"  ;
      qDebug() << display->objectName();
    }
}

void GaborFilter::displayMask()
{
    this->displayIMG(ui->InputMask,this->inputMask);
}

void GaborFilter::displayGaborIMG()// na poctavie https://stackoverflow.com/questions/14582082/merging-channels-in-opencv
{
    try{
        double min, max; // http://answers.opencv.org/question/27248/max-and-min-values-in-a-mat/
        cv::minMaxLoc(this->GaborImag, &min, &max);
        this->GaborImag.convertTo(this->GaborIMGImag,this->IMGFormat,255.0/(max-min),-255.0*min/(max-min));
        cv::minMaxLoc(this->GaborReal, &min, &max);
        this->GaborReal.convertTo(this->GaborIMGReal,this->IMGFormat,255.0/(max-min),-255.0*min/(max-min));
//        qDebug()<<this->GaborIMGImag.type();
//        qDebug()<<this->GaborImag.type();
//        this->GaborIMGImag=this->GaborImag;
//        this->GaborIMGReal=this->GaborReal;

        //this->GaborIMG.release();

        cv::Mat g, fin_img;
    //    img = imread("Lenna.png",CV_LOAD_IMAGE_GRAYSCALE);
        std::vector<cv::Mat> channels;

        g = cv::Mat::zeros(cv::Size(this->GaborIMGReal.cols, this->GaborIMGReal.rows), this->IMGFormat);
//        g = cv::Mat::zeros(cv::Size(this->GaborIMGReal.cols, this->GaborIMGReal.rows), this->GaborFormat);

        channels.push_back(this->GaborIMGReal);
        channels.push_back(this->GaborIMGImag);
        channels.push_back(g);

        cv::merge(channels, fin_img);
    //    cv::imshow("img", fin_img);

//        qDebug()<<fin_img.rows;
        this->GaborIMG=fin_img;
//        fin_img.convertTo(this->GaborIMG,this->IMGFormat);
        this->displayIMG(ui->GaborIMG,this->GaborIMG,CV_BGR2RGB);
        if(ui->GaborIMGCheckBox->isChecked()){
            cv::imshow("Gabor", this->GaborIMG);
            cv::imshow("Gabor Real", this->GaborIMGReal);
            cv::imshow("Gabor Kernel Real", this->GaborKernelReal);
            cv::imshow("Gabor Imag", this->GaborIMGImag);
            cv::imshow("Gabor Kernel Imag", this->GaborKernelImag);

        }
        else{
            cv::destroyAllWindows();
        }
    }
    catch(...){
      qDebug()<<"displayGaborIMG() error"  ;
    }
}

void GaborFilter::on_InputMaskFactorHorizontalSlider_sliderMoved(int position)
{
    this->setMaskFactor(position);
    this->rePrint();
    if(ui->InputMaskFactorSpinBox->value()!=position)
        ui->InputMaskFactorSpinBox->setValue(position);
}

void GaborFilter::setMask()
{
//    this->inputMask= this->inputIMG;
    try{
        cv::threshold(this->inputIMG,this->inputMask,this->MaskFactor,256,0);
    }
    catch(...){

    }
    this->displayMask();
    this->doWork();
}

void GaborFilter::doWork() // żrudło http://answers.opencv.org/question/50975/gabor-kernel-parameters-in-opencv/
//i https://stackoverflow.com/questions/33781502/how-to-get-the-real-and-imaginary-parts-of-a-gabor-kernel-matrix-in-opencv
{
//    cv::Size KernalSize(this->posKernelSizeHeight,this->posKernelSizeWidth);
    cv::Size KernalSize(this->posKernelSizeWidth,this->posKernelSizeHeight);

    double Sigma = this->pos_sigma;
    double Lambda = 0.5+this->pos_lm/100.0;
    double Theta = this->pos_th*CV_PI/180;
    double psi = this->pos_psi*CV_PI/180;;
    double Gamma = this->pos_gamma;

//  cv::Mat KernalReal=cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi);

//    cv::Mat KernelReal = cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi+ 0, this->GaborFormat);
//    cv::Mat KernelImag = cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi+ CV_PI/2, this->GaborFormat);
//  cv::Mat KernelReal = cv::getGaborKernel(KernalSize,Sigma,Theta,Lambda,Gamma,psi,this->GaborFormat);
//  cv::Mat KernelReal = cv::getGaborKernel(KernalSize,Sigma,Theta,Lambda,Gamma,psi,this->GaborFormat);
//    cv::Mat ResultReal;
//    cv::Mat ResultImag;
//    cv::Mat src_f;
//    this->inputIMG.convertTo(src_f, this->GaborFormat, 1.0/255, 0);
//    cv::filter2D(src_f, ResultReal, this->GaborFormat, KernelReal);
//    cv::filter2D(src_f, ResultImag, this->GaborFormat, KernelImag);

//    this->GaborReal=ResultReal;
//    this->GaborImag=ResultImag;

    this->GaborKernelReal = cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi+ 0, this->GaborFormat);
    this->GaborKernelImag = cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi+ CV_PI/2, this->GaborFormat);

    cv::Mat src_f;
    this->inputIMG.convertTo(src_f, this->GaborFormat, 1.0/255, 0);
//    this->inputIMG.convertTo(src_f, this->GaborFormat, 2.0/255, -1.0/255);

    cv::filter2D(src_f,this->GaborReal, this->GaborFormat, this->GaborKernelReal);
    cv::filter2D(src_f, this->GaborImag, this->GaborFormat, this->GaborKernelImag);


    this->rePrint();
    this->valueGaborData(this->GaborReal,this->GaborImag,this->inputMask);
}

//void GaborFilter::doWork()// żrudło http://answers.opencv.org/question/50975/gabor-kernel-parameters-in-opencv/
//{
////    int kernel_size=(pos_kernel_size-1)/2;

////     cv::Size KernalSize(kernel_size,kernel_size);
//     cv::Size KernalSize(this->posKernelSizeHeight,this->posKernelSizeWidth);
//     double Sigma = pos_sigma;
//     double Lambda = 0.5+pos_lm/100.0;
//     double Theta = pos_th*CV_PI/180;
//     double psi = pos_psi*CV_PI/180;;
//     double Gamma = pos_gamma;

//     cv::Mat src_f;
//     cv::Mat dest;

//     try{
//     this->inputIMG.convertTo(src_f, this->GaborFormat, 1.0/255, 0);

//     cv::Mat kernel = cv::getGaborKernel(KernalSize, Sigma, Theta, Lambda,Gamma,psi);
//     cv::filter2D(src_f, dest, this->GaborFormat, kernel);
//     cv::imshow("Process window", dest);
//     qDebug()<<"doWork dest lewel"<<dest.channels();
////     cv::Mat Lkernel(kernel_size*20, kernel_size*20, this->GaborFormat);
//     cv::Mat Lkernel(this->posKernelSizeHeight*20, this->posKernelSizeWidth*20, this->GaborFormat);
//     cv::resize(kernel, Lkernel, Lkernel.size());
//     Lkernel /= 2.;
//     Lkernel += 0.5;
//     cv::imshow("Kernel", Lkernel);
//     cv::Mat mag;
//     cv::pow(dest, 2.0, mag);
//     cv::imshow("Mag", mag);
//     }
//     catch(...){
//         qDebug()<<"error doWork";
//     }
//     this->rePrint();
//}

void GaborFilter::rePrint()
{
    this->displayIMG();
    this->displayMask();
    this->displayGaborIMG();
}

void GaborFilter::initControls()
{
//    MaskFactor=1;
    ui->InputMaskFactorSpinBox->setValue(this->MaskFactor);


//    posKernelSizeWidth = 1;
    ui->KernelSizeWidthSpinBox->setValue(this->posKernelSizeWidth);
//    posKernelSizeHeight = 1;
    ui->KernelSizeHeightSpinBox->setValue(this->posKernelSizeHeight);

    // żrudło http://answers.opencv.org/question/50975/gabor-kernel-parameters-in-opencv/
//    int pos_kernel_size=3;
//    pos_sigma= 5;
    ui->SigmaDoubleSpinBox->setValue(this->pos_sigma);

//    pos_lm = 50;
    ui->LambdaDoubleSpinBox->setValue(this->pos_lm);

//    pos_th = 0;
    ui->ThetaDoubleSpinBox->setValue(this->pos_th);

//    pos_gamma= 0;
    ui->GammaDoubleSpinBox->setValue(this->pos_gamma);

//    pos_psi =0; //90;
    ui->psiDoubleSpinBox->setValue(this->pos_psi);

//    this->doWork();
}

void GaborFilter::on_KernalSizeSpinBox_valueChanged(int arg1)
{
//    this->pos_kernel_size=arg1;
    this->doWork();
}

void GaborFilter::on_SigmaDoubleSpinBox_valueChanged(double arg1)
{
    this->pos_sigma=(double) arg1;
    this->doWork();
}

void GaborFilter::on_LambdaDoubleSpinBox_valueChanged(double arg1)
{
    this->pos_lm=(double) arg1;
    this->doWork();
}

void GaborFilter::on_ThetaDoubleSpinBox_valueChanged(double arg1)
{
    this->pos_th=(double) arg1;
    this->doWork();

}

void GaborFilter::on_psiDoubleSpinBox_valueChanged(double arg1)
{
    this->pos_psi=(double) arg1;
    this->doWork();

}

void GaborFilter::on_GammaDoubleSpinBox_valueChanged(double arg1)
{
    this->pos_gamma=(double) arg1;
    this->doWork();

}

void GaborFilter::on_GammaDoubleSpinBox_valueChanged(const QString &arg1)
{

}

void GaborFilter::on_KernelSizeWidthSpinBox_valueChanged(int arg1)
{
    this->posKernelSizeWidth = arg1;
    this->doWork();
}

void GaborFilter::on_KernelSizeHeightSpinBox_valueChanged(int arg1)
{
    this->posKernelSizeHeight=arg1;
    this->doWork();
}

void GaborFilter::on_spinBox_valueChanged(int arg1)
{

}

void GaborFilter::on_InputMaskFactorSpinBox_valueChanged(int arg1)
{
    this->setMaskFactor(arg1);
    this->rePrint();
    if(ui->InputMaskFactorHorizontalSlider->value()!=arg1)
        ui->InputMaskFactorHorizontalSlider->setValue(arg1);
}

void GaborFilter::on_GaborIMGCheckBox_stateChanged(int arg1)
{
    this->rePrint();
}

void GaborFilter::reject()
{
    cv::destroyAllWindows();
    QDialog::reject();
}
