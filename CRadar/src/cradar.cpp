/***************************************************************************************
** File:        CRadar.h
** Author:      fuhuayang
** Describe:    仿雷达扫描效果
** ------------------------------------------------------------------------------------
** version    author         date        note
** v1.0.0    fuhuayang    2021-03-12    create
** v1.0.1    fuhuayang    2021-03-13    随机点控制在圆形范围内、添加一个逆时针转圈的飞机
** v1.0.2    fuhuayang    2021-03-13    转圈的飞机大一点，其它飞机随机转向，否则全是一个方向的;析构
** v1.0.3    fuhuayang    2021-03-15    加一个刻度盘，带数字度数（坐标系是正东方为0，雷达正北为0）
**                                      注意文字不要畸形
****************************************************************************************/

#include "cradar.h"
#include <QPainter>
#include <QTimerEvent>
#include <QConicalGradient>
#include <qDebug>
#include <QImage>

#define PI  3.14159265358979684626
#define ADJUST_LEN      50
#define SCALE_LINE_LEN  20

CRadar::CRadar(QWidget *parent) : QWidget(parent)
{
    //初始化
    m_pieRotate = 0;
    m_pieRotate_CCW =360;
    m_timerId = -1;
    m_nSpeed = 100;
    for(int i=0;i<15;++i)
    {
        m_points<<QPoint();
        m_pointsAlapha<<100;
    }
    m_point_CCW=QPoint(0,0);


    //启动定时器
    m_timerId = startTimer(m_nSpeed);
    m_pointTimerId = startTimer(1200);
    m_pointTimerId_CCW = startTimer(40);
    m_pPlane = new QImage("airplane.png");
    m_pPlane->load("airplane.png");
    m_pBK = new QImage();
    m_pBK->load("bk.jpg");

}

CRadar::~CRadar()
{
    killTimer(m_pointTimerId_CCW);
    killTimer(m_pointTimerId);
    killTimer(m_timerId);
}

void CRadar::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    //背景
    //painter.fillRect(rect(),QColor(15,15,15));
    painter.drawImage(rect(),*m_pBK,QRect());

    //边长
    int len = m_drawArea.width();
    //格子
    for(int i = 0; i < width(); i += 20)
        painter.drawLine(QPointF(i + 0.5, 0), QPointF(i + 0.5, height()));
    for(int j = 0; j < height(); j += 20)
        painter.drawLine(QPointF(0, j + 0.5), QPointF(width(), j + 0.5));

    //底盘(x轴、y轴和3个圆)
    painter.setPen(QPen(Qt::white,1));
    painter.drawLine(m_drawArea.topLeft() + QPoint(0,len/2),m_drawArea.topRight() + QPoint(0,len/2));
    painter.drawLine(m_drawArea.topLeft() + QPoint(len/2,0),m_drawArea.bottomLeft() + QPoint(len/2,0));

    painter.drawEllipse(m_drawArea.center(),len/2,len/2);
    painter.drawEllipse(m_drawArea.center(),len/4,len/4);
    painter.drawEllipse(m_drawArea.center(),len/8,len/8);

    drawScale(&painter);

    //转动部分
    //--线--//
    qreal x = m_drawArea.center().x() + (qreal)len/2 * cos(-m_pieRotate*PI/180);
    qreal y = m_drawArea.center().y() + (qreal)len/2 * sin(-m_pieRotate*PI/180);
    painter.setPen(QPen(Qt::white));
    painter.drawLine(m_drawArea.center(),QPointF(x,y));

    //--扇形--//
    QConicalGradient gradient;
    gradient.setCenter(m_drawArea.center());
    gradient.setAngle(m_pieRotate + 180); //渐变与旋转方向恰好相反，以扇形相反的边作为渐变角度。
    gradient.setColorAt(0.5,QColor(0,255,0,100)); //从渐变角度开始0.5 - 0.666...为扇形区域。
    gradient.setColorAt(0.667,QColor(0,255,0,0));
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawPie(m_drawArea,m_pieRotate*16,60*16);

    //随机点
    QRectF rect;
    for(int i = 0; i < m_points.count(); ++i)
    {
        int colorAlaph = m_pointsAlapha.at(i);
        painter.setPen(QPen(QColor(255,255,255,colorAlaph),3));
       // painter.drawPoint(m_points.at(i));
        rect =QRect(m_points.at(i).x()-8,m_points.at(i).y()-8,16,16);
        //painter.drawImage(rect,*m_pPlane);
        painter.translate (m_points.at(i));
        painter.rotate(colorAlaph);
        painter.drawImage(QRect(-8,-8,16,16),*m_pPlane);
        painter.resetTransform ();//恢复
    }
    //逆时针旋转的飞机
      painter.setPen(QPen(QColor(255,255,255,0),3));
      rect =QRect(m_point_CCW.x()-12,m_point_CCW.y()-12,24,24);
      painter.translate (rect.center());
      painter.rotate(m_pieRotate_CCW+180);
      QRectF ccwR(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());//这一步很关键
      painter.drawImage(ccwR,*m_pPlane);
      painter.resetTransform ();

}

void CRadar::resizeEvent(QResizeEvent * /*event*/)
{
    //以较短的边长作为绘制区域边长
    if(width() > height())
    {
        m_drawArea = QRect((width() - height())/2,0,height(),height());
    }
    else
    {
        m_drawArea = QRect(0,(height() - width())/2,width(),width());
    }

    m_drawArea.adjust(ADJUST_LEN,ADJUST_LEN,-ADJUST_LEN,-ADJUST_LEN);
}

void CRadar::timerEvent(QTimerEvent *event)
{
    if(m_timerId == event->timerId())
    {
        m_pieRotate -= 10;
        update();
    }
    else if(m_pointTimerId == event->timerId())
    {
        //随机更换的点
        int iD = m_drawArea.width() -2*10;
        int ir = iD/2;
        int iManhattanLength = ir;
        double dRotate =0;
        for(int i = 0; i < m_points.count(); ++i)
        {
            iManhattanLength = rand()%ir;
            dRotate = (rand()%360)*PI/180;
            int offsetX = iManhattanLength*cos(dRotate);
            int offsetY = iManhattanLength*sin(dRotate);
            int alapha = rand()%255;
            m_points.replace(i,QPoint(offsetX,offsetY) + m_drawArea.center());
            m_pointsAlapha.replace(i,alapha);
        }


        update();
    }

    else if(m_pointTimerId_CCW ==event->timerId())
    {
        int iManhattanLength = (m_drawArea.width())/4;
        m_pieRotate_CCW += 1;
        m_pieRotate_CCW = (m_pieRotate_CCW+720)%360;

        double dRotate = m_pieRotate_CCW*PI/180;
        m_point_CCW.rx() = iManhattanLength*cos(dRotate);
        m_point_CCW.ry() = iManhattanLength*sin(dRotate);
        m_point_CCW += m_drawArea.center();
        update();
    }
}

void CRadar::drawScale(QPainter *pPainter)
{
  int len = m_drawArea.width()/2+SCALE_LINE_LEN-5;
  QPointF centerPoint = m_drawArea.center();
  pPainter->drawEllipse(centerPoint,len,len);
  QPointF tempEndPoint,tempBeginPoint, tempTextPoint;
  int r2=len-SCALE_LINE_LEN/2;
  for(int i=0;i<360;++i)
  {
      tempEndPoint.rx()=cos(i*PI/180)*len;
      tempEndPoint.ry()=sin(i*PI/180)*len;
      tempEndPoint+=centerPoint;


      r2= len-SCALE_LINE_LEN/2;
      if(0==i%5)
      {
          tempTextPoint.rx()=cos(i*PI/180)*(len+16 );
          tempTextPoint.ry()=sin(i*PI/180)*(len+16 );
          tempTextPoint+=centerPoint;

          pPainter->drawText(QRectF(tempTextPoint.rx()-9,tempTextPoint.ry()-8,18,16),QString("%1").arg((i+90)%360) );
          r2 =  len-SCALE_LINE_LEN ;
      }

      tempBeginPoint.rx()=cos(i*PI/180)*(r2);
      tempBeginPoint.ry()=sin(i*PI/180)*(r2);
      tempBeginPoint+=centerPoint;
      pPainter->drawLine(tempBeginPoint,tempEndPoint);
  }
}


