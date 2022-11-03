#ifndef SLIDERRANGE_HORIZONOTAL_H
#define SLIDERRANGE_HORIZONOTAL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

class HSliderRang  : public	QWidget
{
    Q_OBJECT
    Q_ENUMS(SliderStyle)
    Q_ENUMS(SliderBgPercent)
    Q_ENUMS(SliderPercent)

    Q_PROPERTY(int minValue READ getMinValue WRITE setMinValue)
    Q_PROPERTY(int maxValue READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(int leftValue READ getLeftValue WRITE setLeftValue)
    Q_PROPERTY(int rightValue READ getRightValue WRITE setRightValue)

    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(bool horizontal READ getHorizontal WRITE setHorizontal)
    Q_PROPERTY(bool showText READ getShowText WRITE setShowText)

    Q_PROPERTY(QColor usedColor READ getUsedColor WRITE setUsedColor)
    Q_PROPERTY(QColor freeColor READ getFreeColor WRITE setFreeColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor rangeTextColor READ getRangeTextColor WRITE setRangeTextColor)
    Q_PROPERTY(QColor sliderColor READ getSliderColor WRITE setSliderColor)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

    Q_PROPERTY(SliderStyle sliderStyle READ getSliderStyle WRITE setSliderStyle)
    Q_PROPERTY(SliderBgPercent sliderBgPercent READ getSliderBgPercent WRITE setSliderBgPercent)
    Q_PROPERTY(SliderPercent sliderPercent READ getSliderPercent WRITE setSliderPercent)
public:

    enum SliderStyle
    {
        SliderStyle_Line = 0,
        SliderStyle_Circle = 1,
    };

    enum SliderBgPercent
    {
        SliderBgPercent_0_1 = 1,
        SliderBgPercent_0_2 = 2,
        SliderBgPercent_0_3 = 3,
        SliderBgPercent_0_4 = 4,
        SliderBgPercent_0_5 = 5,
        SliderBgPercent_0_6 = 6,
        SliderBgPercent_0_7 = 7,
        SliderBgPercent_0_8 = 8,
        SliderBgPercent_0_9 = 9,
        SliderBgPercent_1_0 = 10
    };

    enum SliderPercent
    {
        SliderPercent_0_1 = 1,
        SliderPercent_0_2 = 2,
        SliderPercent_0_3 = 3,
        SliderPercent_0_4 = 4,
        SliderPercent_0_5 = 5
    };
    enum LastPressPos {
        press_center = 0,
        press_right = 1,
    };

	explicit HSliderRang(QWidget *parent = NULL);
	~HSliderRang();


    int getMinValue()               const;
    int getMaxValue()               const;
    int getLeftValue()              const;
    int getRightValue()             const;

    int getBorderWidth()            const;
    bool getHorizontal()            const;
    bool getShowText()              const;

    QColor getUsedColor()           const;
    QColor getFreeColor()           const;
    QColor getTextColor()           const;
    QColor getRangeTextColor()      const;
    QColor getSliderColor()         const;
    QColor getBorderColor()         const;

    SliderStyle getSliderStyle()    const;
    SliderBgPercent getSliderBgPercent()const;
    SliderPercent getSliderPercent()const;

    QSize sizeHint()                const;
    QSize minimumSizeHint()         const;

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void drawSliderBg(QPainter* painter);
    void drawSliderLine(QPainter* painter);
    void drawSliderCircle(QPainter* painter);
    void drawValue(QPainter* painter);
    void focusOutEvent ( QFocusEvent * event ) Q_DECL_OVERRIDE;
    void focusInEvent ( QFocusEvent * event ) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;

public slots:
    
    void setRange(int minValue, int maxValue, bool isEmit = true);

    
    void setMinValue(int minValue);
    void setMaxValue(int maxValue);

    
    void setCurrentRange(int leftValue, int rightValue, bool isEmit = true);
    void setLeftValue(int leftValue);
    void setRightValue(int rightValue);

    
    void setBorderWidth(int borderWidth);

    
    void setHorizontal(bool horizontal);

    
    void setShowText(bool showText);

    
    void setUsedColor(const QColor& usedColor);
    
    void setFreeColor(const QColor& freeColor);
    
    void setTextColor(const QColor& textColor);
    
    void setRangeTextColor(const QColor& rangeTextColor);
    
    void setSliderColor(const QColor& sliderColor);
    
    void setBorderColor(const QColor& borderColor);

    
    void setSliderStyle(const SliderStyle& sliderStyle);
    
    void setSliderBgPercent(const SliderBgPercent& sliderBgPercent);
    
    void setSliderPercent(const SliderPercent& sliderPercent);

signals:
    void valueChanged(int leftValue, int rightValue);

private:
    int minValue;                   
    int maxValue;                   
    int leftValue;                  
    int rightValue;                 

    int borderWidth;                
    bool horizontal;                
    bool showText;                  

    QColor usedColor;               
    QColor freeColor;               
    QColor textColor;               
    QColor rangeTextColor;          
    QColor sliderColor;             
    QColor borderColor;             

    SliderStyle sliderStyle;        
    SliderBgPercent sliderBgPercent;
    SliderPercent sliderPercent;    

    bool leftPressed;               
    bool rightPressed;              

    int sliderLen;                  
    QRect leftSliderRect;           
    QRect rightSliderRect;          
    LastPressPos pressPos = press_center;
    double alf = 0.0;
};



#endif // !SLIDERRANGE_HORIZONOTAL_H
