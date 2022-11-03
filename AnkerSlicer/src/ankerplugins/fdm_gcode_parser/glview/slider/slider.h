#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class SliderLabel;

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT Slider : public QSlider
#else
class Slider : public QSlider
#endif

{
    Q_OBJECT
    Q_ENUMS(ArrowStyle)
    Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius)
    Q_PROPERTY(int arrowSize READ getArrowSize WRITE setArrowSize)
    Q_PROPERTY(ArrowStyle arrowStyle READ getArrowStyle WRITE setArrowStyle)
    Q_PROPERTY(QColor background READ getBackground WRITE setBackground)
    Q_PROPERTY(QColor foreground READ getForeground WRITE setForeground)

    Q_PROPERTY(int labTipWidth READ getLabTipWidth WRITE setLabTipWidth)
    Q_PROPERTY(int labTipHeight READ getLabTipHeight WRITE setLabTipHeight)
    Q_PROPERTY(QFont labTipFont READ getLabTipFont WRITE setLabTipFont)

    Q_PROPERTY(bool showTime READ getShowTime WRITE setShowTime)
    Q_PROPERTY(bool clickEnable READ getClickEnable WRITE setClickEnable)
    Q_PROPERTY(QString unit READ getUnit WRITE setUnit)

public:
    enum ArrowStyle {
        ArrowStyle_Left = 0,    
        ArrowStyle_Right = 1,   
        ArrowStyle_Top = 2,     
        ArrowStyle_Bottom = 3   
    };

    explicit Slider(QWidget *parent = 0);
    ~Slider();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    int borderRadius;               
    int arrowSize;                  
    ArrowStyle arrowStyle;          

    QColor background;              
    QColor foreground;              

    int labTipWidth;                
    int labTipHeight;               
    QFont labTipFont;               

    bool showTime;                  
    bool clickEnable;               
    QString unit;                   

    SliderLabel *labTip;               

public:
    int getBorderRadius()           const;
    int getArrowSize()              const;
    ArrowStyle getArrowStyle()      const;

    QColor getBackground()          const;
    QColor getForeground()          const;

    int getLabTipWidth()            const;
    int getLabTipHeight()           const;
    QFont getLabTipFont()           const;

    bool getShowTime()              const;
    bool getClickEnable()           const;
    QString getUnit()               const;

public Q_SLOTS:
    
    void setBorderRadius(int borderRadius);
    
    void setArrowSize(int arrowSize);
    
    void setArrowStyle(const ArrowStyle &arrowStyle);

    
    void setBackground(const QColor &background);
    
    void setForeground(const QColor &foreground);

    
    void setLabTipWidth(int labTipWidth);
    
    void setLabTipHeight(int labTipHeight);
    
    void setLabTipFont(const QFont &labTipFont);

    
    void setShowTime(bool showTime);
    
    void setClickEnable(bool clickEnable);
    
    void setUnit(const QString &unit);

Q_SIGNALS:
    void clicked();
};

#endif // SLIDER_H
