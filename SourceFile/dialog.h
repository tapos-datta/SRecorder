#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mattoimageconversion.h"
#include <QShortcut>
using namespace std;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{

    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

    ~Dialog();

    //variable
    int width;
    int height;

    void innitializeLabel(cv::Mat img);

signals:
    void SendRectanglePoints(cv::Point& p1,cv::Point& p2);
    void SendSignalToClose();


private slots:
    void rightMouse_pressed(QMouseEvent *ev);
    void leftMouse_pressed(QMouseEvent *ev);
    void mouse_left_pos(QMouseEvent *ev);
    void mouse_current_pos(QMouseEvent *ev);
    void drawRectangle(double centerX, double centerY, double radius);
    void displayImg(cv::Mat foregroundImg);
    cv::Mat  convertModifiedImage3Dto2D(cv::Mat imagefile);
    cv::Mat  modifyingShownImage(cv::Mat image1, cv::Mat image2);
    void endPointForRectangle(int endX,int endY);
    void shortcutFilter();





private:
    Ui::Dialog *ui;
    bool isPressed = false;
    int startX,startY;
    int endX,endY;
    cv::Mat backgroundImg,rectangleImg,foregroundImg;
    cv::Point startPoint,endpoint;
    bool isRelease,isSelectedArea;
    QShortcut *_exit;
};

#endif // DIALOG_H
