#include "gaborcoding.h"
#include "ui_gaborcoding.h"

GaborCoding::GaborCoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaborCoding)
{
    ui->setupUi(this);
    this->initControls();
    this->doEnabledSeries();
}

GaborCoding::~GaborCoding()
{
    delete ui;
}

void GaborCoding::setGaborData(cv::Mat GaborReal, cv::Mat GaborImag, cv::Mat GaborMask)
{
    this->GaborReal=GaborReal;
    this->GaborImag=GaborImag;
    this->GaborMask=GaborMask;
    this->doWork();
}

void GaborCoding::doWork()
{
    this->Hash.clear();
    this->HashMask.clear();
    if(this->HashAutoScale){
        this->doWorkHashAutoScale();
    }
    else {

    }
    ui->HashTextEdit->setText(this->Hash);
    ui->HashMaskTextEdit->setText(this->HashMask);
}

void GaborCoding::doWorkHashAutoScale()
{
    this->HashLength=this->GaborReal.rows*this->GaborReal.cols*2;
    ui->HashLengthSpinBox->setValue(this->HashLength);

    this->Hash.reserve(this->HashLength+1);
    this->HashMask.reserve(this->HashLength+1);
    for(int i=0;i<this->GaborReal.rows;i++){
        for(int j=0;j<this->GaborReal.cols;j++){
            this->Hash.append( this->coding(this->GaborReal.at<double>(i,j),this->GaborImag.at<double>(i,j)));
            this->HashMask.append(this->codingMask(this->GaborMask.at<char>(i,j)));
        }
    }

}

//void GaborCoding::doWorkHashAutoScale()
//{
//    this->HashLength=this->GaborReal.rows*this->GaborReal.cols*2;
//    ui->HashLengthSpinBox->setValue(this->HashLength);

//    this->Hash.reserve(this->HashLength+1);
//    this->HashMask.reserve(this->HashLength+1);

//    this->cvMat2stdVector(this->GaborReal,this->GaborRealArray);
//    this->cvMat2stdVector(this->GaborImag,this->GaborImagArray);
//    this->cvMat2stdVector(this->GaborMask,this->GaborMaskArray);

//    for (int i=0; i < this->GaborRealArray.size(); i++) {
//        this->Hash.append(this->coding(this->GaborRealArray[i],this->GaborImagArray[i]));
//        this->HashMask.append(this->codingMask(this->GaborMaskArray[i]));
//    }



//}

void GaborCoding::doEnabledSeries()
{
    ui->HashLengthSpinBox->setEnabled(!this->HashAutoScale);
}

void GaborCoding::initControls()
{
    ui->HashAutoScaleCheckBox->setChecked(this->HashAutoScale);
    ui->HashLengthSpinBox->setValue(this->HashLength);
    //    ui->HashTextEdit->setText("bdiufbdiu");
    ui->HashTextEdit->setReadOnly(true);
    ui->HashMaskTextEdit->setReadOnly(true);
}

QString GaborCoding::coding(double real, double imag)
{
    double pom=atan2(imag,real);
    pom=pom*180.0/M_PI;
    //  10|00
    //  --+--
    //  11|01
    if(pom<90){
        return "00";
    }
    else if (pom<180) {
       return "10";
    }
    else if (pom<270) {
       return "11";
    }
    else{
        return "01";
    }
}

QString GaborCoding::codingMask(char data)
{
    if (data){
        return "11";
    }
    else{
        return "00";
    }
}

bool GaborCoding::cvMat2stdVector(cv::Mat &src, std::vector<float> &dest) // surce https://stackoverflow.com/questions/26681713/convert-mat-to-array-vector-in-opencv
{
    if (src.isContinuous()) {
      dest.assign(src.datastart, src.dataend);
      return true;
    } else {
      for (int i = 0; i < src.rows; ++i) {
        dest.insert(dest.end(), src.ptr<uchar>(i), src.ptr<uchar>(i)+src.cols);
        return true;
      }
    }
    return false;

}

bool GaborCoding::cvMat2stdVector(cv::Mat &src, std::vector<uchar> &dest)
{
    if (src.isContinuous()) {
      dest.assign(src.datastart, src.dataend);
      return true;
    } else {
      for (int i = 0; i < src.rows; ++i) {
        dest.insert(dest.end(), src.ptr<uchar>(i), src.ptr<uchar>(i)+src.cols);
        return true;
      }
    }
    return false;

}

void GaborCoding::on_HashAutoScaleCheckBox_stateChanged(int arg1)
{
    this->HashAutoScale=arg1;
    this->doEnabledSeries();
}
