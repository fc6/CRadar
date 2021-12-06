/***************************************************************************************
** File:        widget.cpp
** Author:      fuhuayang
** ------------------------------------------------------------------------------------
** version    author         date        note
** v1.0.0    fuhuayang    2021-03-12    create
** v1.0.1    fuhuayang    2021-03-13    修改大小，目的是为了雷达显示可改变大小
****************************************************************************************/

#include "widget.h"
#include <QVBoxLayout>
#include "cradar.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pRadar = new CRadar(this);
    QVBoxLayout * pLayout = new QVBoxLayout(this);
    m_pRadar->resize(1040,650);
    this->resize(1040,650);
    this->setMinimumSize(50,50);
    pLayout->addWidget(m_pRadar);
}

Widget::~Widget()
{
     delete m_pRadar;
}
