# SRecorder

SRecorder is a desktop application (only for Windows),built in **C++** with **OpenCV** that can able to capture screen with the custom region of interest. 

## Usability
This application can be used for taking screenshot alongside with captureing screen.

## Requirements
- Qt5 with C++
- OpenCV (3.X)

### How it Works

When the program is executed, a interface will be like the following picture. 
Here, the number of display will be listed. If one selects a display to get capture, all of other displays will be 
inactive except selected one. The **Get ROI** button works for to select the region of interest. When the button being
pressed, the interface will be hidden and one can draw a rectangle to select ROI using left mouse. After that clicking 
right mouse, the ROI has been selected and then the interface will be shown again to either capture screen or save as screenshot.  

![Interface of SRecorder](/uploads/5e13032d4d770a835d2e3cc6a61aa1f1/screenshot.PNG)