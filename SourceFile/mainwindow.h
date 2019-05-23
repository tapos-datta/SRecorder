#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include <iostream>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QShortcut>
#include <windows.h>
#include <QMessageBox>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv/cv.hpp>
#include <opencv2/videoio.hpp>
#include "captureinbackground.h"
#include <QFile>
#include <QDir>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendStopAction();


private slots:
    void on_pushButton_clicked();
    cv::Mat hwnd2mat(HWND hwnd);
    void getRectanglePoints(cv::Point& p1, cv::Point& p2);
    void returnToMainWindow();
    void ProcessForGetROI();



    void on_screemComboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();
    void shortcutFilter();
    void capturedFinished();

    void on_screenShot_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *newWindow;

    int scrHeight;
    int scrWidth;
    cv::Point topLeftPoint, bottomRightPoint;
    cv::Mat sourceImg;
    bool selectedROI,isRecording;
    QStringList scrnList;
    int *widthResolationOfScreens;
    int startPositionAtWidth;
    QShortcut *stopRecording;
    CaptureInBackGround *thread;
    QMessageBox msgBox;
};

#endif // MAINWINDOW_H
