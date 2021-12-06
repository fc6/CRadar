#ifndef CRADAR_H
#define CRADAR_H

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

#include <QWidget>
class QImage;
class CRadar : public QWidget
{
    Q_OBJECT
public:
    explicit CRadar(QWidget *parent = nullptr);
    ~CRadar();
protected:
    void paintEvent(QPaintEvent *event);    //绘制事件
    void resizeEvent(QResizeEvent *event);  //大小重置事件
    void timerEvent(QTimerEvent *event);    //定时器事件

private:
    void drawScale(QPainter* pPainter);     //绘制刻度
private:
    QRect         m_drawArea;      //绘制区域
    int           m_pieRotate;     //扇形旋转区域
    int           m_timerId;       //定时器ID
    int           m_pointTimerId;  //变更点定时器
    int           m_pointTimerId_CCW;  //变更点定时器 逆时针旋转的点
    int           m_nSpeed;        //速度
    QList<QPoint> m_points;        //绘制点
    QPoint        m_point_CCW;
    int           m_pieRotate_CCW;//角度
    QList<int>    m_pointsAlapha;  //绘制点颜色alapha值
    QImage*       m_pPlane;
    QImage*       m_pBK;
};

#endif //! CRADAR_H
