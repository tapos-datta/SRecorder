#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>


class EventManagement : public QLabel
{
    Q_OBJECT

public:
    explicit EventManagement(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    int posX,posY;
    QPoint offset;

signals:
    void RightMouse_Pressed(QMouseEvent *ev);
    void LeftMouse_Pressed(QMouseEvent *ev);
    void Mouse_Pos(QMouseEvent *ev);
    void Mouse_Left(QMouseEvent *ev);


public slots:
};

#endif // EVENTMANAGEMENT_H
