#ifndef MATTOIMAGECONVERSION_H
#define MATTOIMAGECONVERSION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMainWindow>
#include <opencv/cv.hpp>



class MatToImageConversion
{
public:
    MatToImageConversion(const cv::Mat &inMat);
    ~MatToImageConversion();
    QPixmap retPix;
    QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    QImage cvMatToQImage( const cv::Mat &inMat);
};

#endif // MATTOIMAGECONVERSION_H
