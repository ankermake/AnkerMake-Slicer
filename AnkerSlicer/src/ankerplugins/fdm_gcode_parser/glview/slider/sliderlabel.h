#ifndef SLIDERLABEL_H
#define SLIDERLABEL_H

#include <QLabel>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT SliderLabel : public QLabel
#else
class SliderLabel : public QLabel
#endif

{
    Q_OBJECT
    Q_ENUMS(ArrowPosition)
    Q_ENUMS(TrianglePosition)
    Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius)
    Q_PROPERTY(QColor background READ getBackground WRITE setBackground)
    Q_PROPERTY(QColor foreground READ getForeground WRITE setForeground)

    Q_PROPERTY(bool showArrow READ getShowArrow WRITE setShowArrow)
    Q_PROPERTY(int arrowSize READ getArrowSize WRITE setArrowSize)
    Q_PROPERTY(ArrowPosition arrowPosition READ getArrowPosition WRITE setArrowPosition)

    Q_PROPERTY(bool showTriangle READ getShowTriangle WRITE setShowTriangle)
    Q_PROPERTY(int triangleLen READ getTriangleLen WRITE setTriangleLen)
    Q_PROPERTY(TrianglePosition trianglePosition READ getTrianglePosition WRITE setTrianglePosition)
    Q_PROPERTY(QColor triangleColor READ getTriangleColor WRITE setTriangleColor)

public:
    enum ArrowPosition {
        ArrowPosition_Left = 0,     
        ArrowPosition_Right = 1,    
        ArrowPosition_Top = 2,      
        ArrowPosition_Bottom = 3    
    };

    enum TrianglePosition {
        TrianglePosition_Left = 0,  
        TrianglePosition_Right = 1, 
        TrianglePosition_Top = 2,   
        TrianglePosition_Bottom = 3 
    };

    explicit SliderLabel(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);
    void drawTriangle(QPainter *painter);

private:
    int borderRadius;                   
    QColor background;                  
    QColor foreground;                  

    bool showArrow;                     
    int arrowSize;                      
    ArrowPosition arrowPosition;        

    bool showTriangle;                  
    int triangleLen;                    
    TrianglePosition trianglePosition;  
    QColor triangleColor;               

    QRect bgRect;                       

public:
    int getBorderRadius()               const;
    QColor getBackground()              const;
    QColor getForeground()              const;

    bool getShowArrow()                 const;
    int getArrowSize()                  const;
    ArrowPosition getArrowPosition()    const;

    bool getShowTriangle()              const;
    int getTriangleLen()                const;
    TrianglePosition getTrianglePosition()const;
    QColor getTriangleColor()           const;

    QSize sizeHint()                    const;
    QSize minimumSizeHint()             const;

public Q_SLOTS:
    
    void setBorderRadius(int borderRadius);
    
    void setBackground(const QColor &background);
    
    void setForeground(const QColor &foreground);

    
    void setShowArrow(bool showArrow);
    
    void setArrowSize(int arrowSize);
    
    void setArrowPosition(const ArrowPosition &arrowPosition);

    
    void setShowTriangle(bool showTriangle);
    
    void setTriangleLen(int triangleLen);
    
    void setTrianglePosition(const TrianglePosition &trianglePosition);
    
    void setTriangleColor(const QColor &triangleColor);

Q_SIGNALS:
    void clicked();
};

#endif // SLIDERLABEL_H
