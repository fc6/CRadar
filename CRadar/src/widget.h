#ifndef WIDGET_H
#define WIDGET_H
/***************************************************************************************
** File:        widget.cpp
** Author:      fuhuayang
** ------------------------------------------------------------------------------------
** version    author         date        note
** v1.0.0    fuhuayang    2021-03-12    create
** v1.0.1    fuhuayang    2021-03-13    修改大小，目的是为了雷达显示可改变大小
****************************************************************************************/

#include <QWidget>
class CRadar;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    CRadar*             m_pRadar =NULL;
};

#endif // WIDGET_H
