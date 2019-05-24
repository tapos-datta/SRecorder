#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->ROI,SIGNAL(RightMouse_Pressed(QMouseEvent*)),this,SLOT(rightMouse_pressed(QMouseEvent*)));
    connect(ui->ROI,SIGNAL(LeftMouse_Pressed(QMouseEvent*)),this,SLOT(leftMouse_pressed(QMouseEvent*)));
    connect(ui->ROI,SIGNAL(Mouse_Pos(QMouseEvent*)),this,SLOT(mouse_current_pos(QMouseEvent*)));
    connect(ui->ROI,SIGNAL(Mouse_Left(QMouseEvent*)),this,SLOT(mouse_left_pos(QMouseEvent*)));



}

Dialog::~Dialog()
{
    delete ui;
    delete _exit;
}

void Dialog::innitializeLabel(cv::Mat img)
{

     ui->ROI->setGeometry(0,0,width,height);
     isPressed = false;
     isRelease = false;
     isSelectedArea = false;
     cv::Mat img2 = cv::Mat::zeros(height,width,CV_8UC3);



     if(img.channels()>3){
         cv::cvtColor(img, img, CV_BGRA2BGR);
     }
//     qDebug()<<img.type()<<" "<<img.channels()<<endl<<width<<" "<<height<<" "<<img.rows<<" "<<img.cols;
//     imshow("img",img);
//     cv::waitKey(0);

     backgroundImg = img.clone();
     rectangleImg = img2.clone();

     displayImg(rectangleImg.clone());

     _exit = new QShortcut(QKeySequence("Esc"), this);
     connect(_exit,SIGNAL(activated()),this,SLOT(shortcutFilter()));



}

void Dialog::displayImg(cv::Mat foregroundImg)
{
     cv::Mat shownImg = modifyingShownImage(backgroundImg.clone(),foregroundImg.clone());

     if(isRelease && !isPressed)
     {
         cv::Mat selectedROI = foregroundImg.clone();
         cv::floodFill(selectedROI, cv::Point(0,0), cv::Scalar(255,255,255));
         cv::bitwise_not(selectedROI,selectedROI);
         cv::cvtColor(selectedROI,selectedROI,CV_BGR2GRAY);
         cv::Mat targetImg,new_image;
         shownImg.convertTo(targetImg,CV_BGR2BGRA);

         new_image = targetImg.clone();


         for( int y = 0; y < targetImg.rows; y++ )
         {
             for( int x = 0; x < targetImg.cols; x++ )
             {
                 if(selectedROI.at<uchar>(y,x)>0)
                 {
                     for( int c = 0; c < 3; c++ )
                     {
                         new_image.at<cv::Vec3b>(y,x)[c] =
                           cv::saturate_cast<uchar>( 0.5*( targetImg.at<cv::Vec3b>(y,x)[c] ) + 0 );
                     }
                 }

             }
         }
         shownImg = new_image.clone();

     }

    MatToImageConversion matCon(shownImg);
    QPixmap imPix = matCon.retPix;

    ui->ROI->setPixmap(imPix);

}


//right mouse pressed
void Dialog::rightMouse_pressed(QMouseEvent *ev)
{
    if(isSelectedArea)
    {
        SendRectanglePoints(startPoint,endpoint);
        SendSignalToClose();
    }

    isRelease = false;
    isPressed = false;

}
//left mouse pressed
void Dialog::leftMouse_pressed(QMouseEvent *ev)
{
    if(!isPressed)
    {
        isPressed = true;
        startX = ev->x();
        startY = ev->y();
        startPoint = cv::Point(startX,startY);
        isRelease = false;
        isSelectedArea = false;
    }
}

//Release mouse press
void Dialog::mouse_left_pos(QMouseEvent *ev)
{
    if(isPressed)
    {
        isPressed = false;
        endX = ev->x();
        endY = ev->y();

        //no rectangle
        if(endX == startX && endY== startY)
            return;

        isRelease = true;
        endPointForRectangle(endX,endY);
        //selected region successfully
        isSelectedArea = true;
    }

}

//Drag mouse cursor
void Dialog::mouse_current_pos(QMouseEvent *ev)
{
    if(isPressed)
    {
        endX = ev->x();
        endY = ev->y();
        endPointForRectangle(endX,endY);
    }
}


void Dialog::endPointForRectangle(int endX,int endY)
{
    //confined area
    if(endX<0)
        endX = 0;
    if(endX>=width)
        endX = width-1;
    if(endY<=0)
        endY = 0;
    if(endY>=height)
        endY = height-1;

    endpoint = cv::Point(endX,endY);

    foregroundImg.release();
    foregroundImg = rectangleImg.clone();

    rectangle(foregroundImg,startPoint,endpoint,cv::Scalar(0,0,255),2,8);
    displayImg(foregroundImg.clone());
}

cv::Mat Dialog::convertModifiedImage3Dto2D(cv::Mat imagefile)
{
    //converting 3D modified image to 2D modified image

    if(imagefile.channels()==1){
        return imagefile;
    }

    cv::Mat retImg = cv::Mat::zeros(imagefile.rows,imagefile.cols,CV_8UC1);
    cv::Mat splitedImage[3];
    split(imagefile,splitedImage);

    for(int i=0;i<imagefile.rows;i++)
    {
        for(int j=0;j<imagefile.cols;j++)
        {
            if(splitedImage[0].at<uchar>(i,j)==0 && splitedImage[1].at<uchar>(i,j)==0 && splitedImage[2].at<uchar>(i,j)==255 )
            {
                retImg.at<uchar>(i,j) = 255;
            }

        }
    }

    return retImg;
}

cv::Mat Dialog::modifyingShownImage(cv::Mat image1, cv::Mat image2)
{
    cv::Mat newImage2 = convertModifiedImage3Dto2D(image2);

    //create mask from image2
 //   cv::Mat mask = cv::Mat::ones(newImage2.rows,newImage2.cols,CV_8UC1);
    cv::Mat mask;
    mask = newImage2<255;
    mask = mask.mul(1/255.0);


    //creating reverse mask
    cv::Mat reverseMask,roi,maski,rmaski,result;
    bitwise_xor(mask,1,reverseMask);

    vector<cv::Mat>Mask3D;
    Mask3D.push_back(mask);
    Mask3D.push_back(mask);
    Mask3D.push_back(mask);

    merge(Mask3D,maski);

    roi = image1.mul(maski);

    vector<cv::Mat>reverseMask3D;

    reverseMask3D.push_back(reverseMask);
    reverseMask3D.push_back(reverseMask);
    reverseMask3D.push_back(reverseMask);

    for(int i=0;i<reverseMask.rows;i++)
    {
        for(int j=0;j<reverseMask.cols;j++)
        {
            if(reverseMask.at<uchar>(i,j)==1)
            {
                reverseMask3D[0].at<uchar>(i,j)=1;
                reverseMask3D[1].at<uchar>(i,j)=1;
                reverseMask3D[2].at<uchar>(i,j) =1;
            }
        }
    }

    merge(reverseMask3D,rmaski);


    cv::Mat modifiedRegion = image2.mul(rmaski);

    add(roi,modifiedRegion,result);


    return result;
}

void Dialog::shortcutFilter()
{
    QShortcut* shortcut = qobject_cast<QShortcut*>(sender());
    QKeySequence seq = shortcut->key();
    QString keySeq  = seq.toString();

    if(keySeq=="Esc")
    {
        SendSignalToClose();
    }
}



void Dialog::drawRectangle(double centerX, double centerY, double radius)
{

}

