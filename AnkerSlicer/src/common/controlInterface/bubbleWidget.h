#ifndef BUBBLEWIDGET_H
#define BUBBLEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QToolButton>
#include <QGridLayout>
#include <QDebug>
namespace  control{


class BubbleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleWidget(QWidget *parent = nullptr,Qt::ArrowType  m_arrowType = Qt::NoArrow);
    //   Qt::NoArrow  Qt::UpArrow  Qt::DownArrow  Qt::LeftArrow  Qt::RightArrow
    void setArrowType(Qt::ArrowType  arrowType);

    qreal getMargins()  {return m_triangleHeight;};
    void setRadius(qreal radius) {m_radius = radius; setLayoutMargins(m_arrowType); update();};
    void setTriangleStartX(qreal triangleStartX) { m_triangleStartX = triangleStartX; setLayoutMargins(m_arrowType); update();};
    void setTriangleWidth(qreal triangleWidth){m_triangleWidth = triangleWidth; setLayoutMargins(m_arrowType); update();};
    void setTriangleHeight(qreal triangleHeight) {m_triangleHeight = triangleHeight; setLayoutMargins(m_arrowType); update();};
    void setBackgroundColor(const QColor &backgroundColor){m_backgroundColor = backgroundColor;  update();};

protected:
   void paintEvent(QPaintEvent *event) override;

private:
   QPainterPath addPathTo(Qt::ArrowType  arrowType);
   void setLayoutMargins(Qt::ArrowType arrowType);

private:
   Qt::ArrowType  m_arrowType;
   qreal m_radius;
   QColor m_backgroundColor;

   qreal m_triangleStartX;
   qreal m_triangleWidth;
   qreal m_triangleHeight;


//   Qt::NoArrow
//   Qt::UpArrow1
//   Qt::DownArrow2
//   Qt::LeftArrow3
//   Qt::RightArrow4


};


}
#endif // BUBBLEWIDGET_H
