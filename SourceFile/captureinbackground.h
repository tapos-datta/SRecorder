#ifndef CAPTUREINBACKGROUND_H
#define CAPTUREINBACKGROUND_H

#include <QObject>
#include <QThread>
#include <windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv/cv.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include <iostream>
#include <QDebug>
using namespace std;

class CaptureInBackGround : public QThread
{
    Q_OBJECT

public:
    CaptureInBackGround();
    cv::Mat hwnd2mat(HWND hwnd);
    void run();

    int  scrHeight,scrWidth,startPositionAtWidth;
    cv::Point topLeftPoint,bottomRightPoint;
    QString saveFileName;

signals:
    void sendCompletedNotification();

public slots:
   void stopAction();



private:

   bool isRecording;
};

#endif // CAPTUREINBACKGROUND_H
