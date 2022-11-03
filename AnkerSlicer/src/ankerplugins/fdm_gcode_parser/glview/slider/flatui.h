#ifndef FLATUI_H
#define FLATUI_H

#include <QObject>

class QPushButton;
class QLineEdit;
class QProgressBar;
class QSlider;
class QRadioButton;
class QCheckBox;
class QScrollBar;

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT FlatUI : public QObject
#else
class FlatUI : public QObject
#endif

{
    Q_OBJECT
public:
    static FlatUI *Instance();
    explicit FlatUI(QObject *parent = 0);    

private:
    static QScopedPointer<FlatUI> self;

public:
    
    void setPushButtonQss(QPushButton *btn,                             
                          int radius = 5,                               
                          int padding = 8,                              
                          const QString &normalColor = "#34495E",       
                          const QString &normalTextColor = "#FFFFFF",   
                          const QString &hoverColor = "#4E6D8C",        
                          const QString &hoverTextColor = "#F0F0F0",    
                          const QString &pressedColor = "#2D3E50",      
                          const QString &pressedTextColor = "#B8C6D1"); 

    
    void setLineEditQss(QLineEdit *txt,                                 
                        int radius = 3,                                 
                        int borderWidth = 2,                            
                        const QString &normalColor = "#DCE4EC",         
                        const QString &focusColor = "#34495E");         

    
    void setProgressQss(QProgressBar *bar,
                           int barHeight = 8,                           
                           int barRadius = 5,                           
                           int fontSize = 9,                            
                           const QString &normalColor = "#E8EDF2",      
                           const QString &chunkColor = "#E74C3C");      

    
    void setSliderQss(QSlider *slider,                                  
                      int sliderHeight = 8,                             
                      const QString &normalColor = "#E8EDF2",           
                      const QString &grooveColor = "#1ABC9C",           
                      const QString &handleColor = "#1ABC9C");          

    
    void setRadioButtonQss(QRadioButton *rbtn,                          
                           int indicatorRadius = 8,                     
                           const QString &normalColor = "#D7DBDE",      
                           const QString &checkColor = "#34495E");      

    
    void setScrollBarQss(QWidget *scroll,                               
                         int radius = 6,                                
                         int min = 120,                                 
                         int max = 12,                                  
                         const QString &bgColor = "#606060",            
                         const QString &handleNormalColor = "#34495E",  
                         const QString &handleHoverColor = "#1ABC9C",   
                         const QString &handlePressedColor = "#E74C3C");
};

#endif // FLATUI_H
