#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //load qss file for QMessageBox
    QString path = QCoreApplication::applicationDirPath();
    QFile File(":/stylesheet/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    msgBox.setStyleSheet(StyleSheet);

    ui->captureButton->setEnabled(false);
    ui->screenShot->setEnabled(false);

    selectedROI = false;
    startPositionAtWidth = 0;
    QList<QScreen*> screenList = QGuiApplication::screens();
    scrnList.clear();
    scrnList.append("");
    ui->numberOfScreen->setText(QString::number(screenList.length()));

    widthResolationOfScreens = new int[(screenList.length() + 1)];

    widthResolationOfScreens[0] = 0;

    for(int i=0;i<screenList.length();i++)
    {
        QRect  screenGeometry = screenList[i]->geometry();
        QString name = "Display-"+QString::number(i+1)+" ("+QString::number(screenGeometry.width())+" X "+QString::number(screenGeometry.height())+")";
        qDebug()<<name<<endl;
        scrnList.append(name);
        widthResolationOfScreens[i] = screenGeometry.width();

    }

    ui->screemComboBox->addItems(scrnList);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    qDebug()<<height<<" "<<width<<endl<<QGuiApplication::screens();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[] widthResolationOfScreens;
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    //Sleep(150);
    scrHeight = QApplication::desktop()->height();
    scrWidth = QApplication::desktop()->width();
    selectedROI = false;
    qDebug()<<scrWidth<<"  "<<scrHeight<<endl;
    ProcessForGetROI();

}

cv::Mat MainWindow::hwnd2mat(HWND hwnd)
{
    HDC hwindowDC,hwindowCompatibleDC;

    int height,width,srcheight,srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC=GetDC(hwnd);

    hwindowCompatibleDC=CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = scrHeight;
    srcwidth = scrWidth;
    //height = windowsize.bottom/1;  //change this to whatever size you want to resize to
    //width = windowsize.right/1;
    height = scrHeight;
    width = scrWidth;

    src.create(height,width,CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap( hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt( hwindowCompatibleDC, startPositionAtWidth,0, width, height, hwindowDC, startPositionAtWidth, 0,srcwidth,srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC,hbwindow,0,height,src.data,(BITMAPINFO *)&bi,DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject (hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);
    return src;
}

void MainWindow::getRectanglePoints(cv::Point& p1,cv::Point& p2)
{
    selectedROI = true;
    topLeftPoint = p1;
    bottomRightPoint = p2;
    cv::Rect myROI(p1,p2);
    // Crop the full image to that image contained by the rectangle myROI
    // Note that this doesn't copy the data
    cv::Mat croppedRef(sourceImg, myROI);
    cv::Mat cropped;
    // Copy the data into new matrix
    croppedRef.copyTo(cropped);
    ui->screenShot->setEnabled(true);
    ui->captureButton->setEnabled(false);


}

void MainWindow::ProcessForGetROI()
{

    newWindow = new Dialog();
    connect(newWindow,SIGNAL(SendRectanglePoints(cv::Point&,cv::Point&)),this,SLOT(getRectanglePoints(cv::Point&, cv::Point&)));
    connect(newWindow,SIGNAL(SendSignalToClose()),this,SLOT(returnToMainWindow()));
    newWindow->width = scrWidth;
    newWindow->height = scrHeight;
    newWindow->setGeometry(0,0,scrWidth,scrHeight);
    Sleep(170);

    HWND hwndDesktop = GetDesktopWindow();

    sourceImg = hwnd2mat(hwndDesktop);

    newWindow->innitializeLabel(sourceImg.clone());

    newWindow->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    //newWindow->setParent(0); // Create TopLevel-Widget
    newWindow->setAttribute(Qt::WA_NoSystemBackground, true);
    newWindow->setAttribute(Qt::WA_TranslucentBackground, true);
    newWindow->show();
}

void MainWindow::returnToMainWindow()
{
    newWindow->hide();
    this->show();
    newWindow->close();
    newWindow->~Dialog();
}


void MainWindow::on_screemComboBox_currentIndexChanged(int index)
{
    ui->screemComboBox->setCurrentIndex(index);
    startPositionAtWidth = 0;
    if(index==0)
    {

        return;
    }

    for(int i=1;i<index;i++)
    {
        startPositionAtWidth += widthResolationOfScreens[i];
    }

}

void MainWindow::on_pushButton_2_clicked()
{

    if(selectedROI)
    {
        qDebug()<<"Enter this section";

        QString path = QFileDialog::getSaveFileName(0,
        tr("Save Video File As"),
        ".","*.avi");

        if(!path.isEmpty())
        {
            msgBox.setText("Screen recording will be start now\n\nTo stop recording use 'Ctrl+T'");
            msgBox.exec();

            stopRecording = new QShortcut(QKeySequence("Ctrl+T"), this);
            connect(stopRecording,SIGNAL(activated()),this,SLOT(shortcutFilter()));

            thread = new CaptureInBackGround();

            connect(this,SIGNAL(sendStopAction()),thread,SLOT(stopAction()));
            connect(thread,SIGNAL(sendCompletedNotification()),this,SLOT(capturedFinished()));
            thread->scrHeight = scrHeight;
            thread->scrWidth = scrWidth;
            thread->startPositionAtWidth = startPositionAtWidth;
            thread->topLeftPoint = topLeftPoint;
            thread->bottomRightPoint = bottomRightPoint;
            thread->saveFileName = path;
            thread->start();

        }
    }

}

void MainWindow::shortcutFilter(){


    QShortcut* shortcut = qobject_cast<QShortcut*>(sender());
    QKeySequence seq = shortcut->key();
    QString keySeq  = seq.toString();

    if(keySeq=="Ctrl+T")
    {
        sendStopAction();
    }


}

void MainWindow::capturedFinished(){

    msgBox.setText("Successfully saved the captured screen.");
    msgBox.exec();

    delete thread;
}

void MainWindow::on_screenShot_clicked()
{
    try {
        QString path = QFileDialog::getSaveFileName(0,
        tr("Save File As"),
        ".","*.png");

        if(!path.isEmpty())
        {
            cv::Rect myROI(topLeftPoint,bottomRightPoint);
            cv::Mat croppedRef(sourceImg, myROI);
            cv::Mat cropped;
            // Copy the data into new matrix
            croppedRef.copyTo(cropped);

            cv::imwrite(path.toStdString(),cropped.clone());
            msgBox.setText("Successfully saved screenshot.");
            msgBox.exec();

        }


    } catch (cv::Exception& e) {

    }
    catch(exception& e)
    {

    }


}
