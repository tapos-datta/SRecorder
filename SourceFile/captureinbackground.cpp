#include "captureinbackground.h"

CaptureInBackGround::CaptureInBackGround()
{

}

void CaptureInBackGround::run(){

    double fps = 10.0;
    DWORD sleepTime = (1000/fps); //calculated in millisec
    isRecording = true;
    vector<cv::Mat> imgFrameBUffer;
    imgFrameBUffer.resize(0);
    int szW =-1,szH =-1;
    HWND hwndDesktop = GetDesktopWindow();

    cv::Rect myROI(topLeftPoint,bottomRightPoint);

    cv::String fileName = saveFileName.toStdString();
    cv::VideoWriter writer;
    writer.open(fileName,CV_FOURCC('M','J','P','G'),fps,cv::Size(myROI.width,myROI.height),true);

    while(isRecording == true)
    {

        qDebug()<<isRecording<<endl;
            cv::Mat sourceImg = hwnd2mat(hwndDesktop);


            // Crop the full image to that image contained by the rectangle myROI
            // Note that this doesn't copy the data
            cv::Mat croppedRef(sourceImg, myROI);
            cv::Mat cropped;
            // Copy the data into new matrix
            croppedRef.copyTo(cropped);

            if(szW==-1 && szH==-1)
            {
                szW = cropped.cols;
                szH = cropped.rows;
            }
            writer.write(cropped.clone());
            //imgFrameBUffer.push_back(cropped.clone());
        //msleep(sleepTime);
    }
    writer.release();

//        for(int i=0;i<imgFrameBUffer.size();i++)
//        {


//        }


    qDebug()<<isRecording<<endl;
    sendCompletedNotification();
}


void CaptureInBackGround::stopAction()
{
    isRecording = false;
}

cv::Mat CaptureInBackGround::hwnd2mat(HWND hwnd)
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
