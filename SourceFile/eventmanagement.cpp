#include "eventmanagement.h"

EventManagement::EventManagement(QWidget *parent) : QLabel(parent)
{

}


void EventManagement::mouseMoveEvent(QMouseEvent *ev)
{
    this->posX = ev->x();
    this->posY = ev->y();
//    if(ev->buttons() & Qt::LeftButton)
//     {
//            this->move(mapToParent(ev->pos() - offset));
//     }
    emit Mouse_Pos(ev);
}

void EventManagement::mousePressEvent(QMouseEvent *ev)
{
    this->offset = ev->pos();
    if(ev->button()==Qt::RightButton){
        emit RightMouse_Pressed(ev);
    }
    else if(ev->button()==Qt::LeftButton)
    {
        emit LeftMouse_Pressed(ev);
    }
}

void EventManagement::mouseReleaseEvent(QMouseEvent *ev)
{
    emit Mouse_Left(ev);
}
