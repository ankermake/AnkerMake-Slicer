#ifndef CUSTOMTITLE_H
#define CUSTOMTITLE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>
#include <QPainterPath>
enum ButtonType
{
    MIN_BUTTON = 0,         
    MIN_MAX_BUTTON ,        
    ONLY_CLOSE_BUTTON       
};

class customTitle : public QWidget
{
    Q_OBJECT

public:
    customTitle(QWidget *parent, QMenuBar * menuBar);
    
    ~customTitle();
    
    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint& point, QSize& size);
    void setMenuBar(QMenuBar *menuBar) { m_menuBar = menuBar; };
    QMenu *addMenu(QString name) { return m_menuBar->addMenu(name);};
    void addAction(QAction *action) {
        m_menuBar->addAction(action);
    }
private:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    
    void initControl();
    
    void initConnections();

signals:
    
    void signalButtonMinClicked();
    void signalButtonRestoreClicked();
    void signalButtonMaxClicked();
    void signalButtonCloseClicked();

private slots:
    
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonRestoreMaxClicked();
    void onButtonCloseClicked();

private:
    QLabel* m_pIcon;                    
    QLabel* m_pTitleContent;            
    QPushButton* m_pButtonMin;          
    QPushButton* m_pButtonRestoreMax;      
    
    QPushButton* m_pButtonClose;        
    QRect m_desktopRect;
    
    int m_colorR;
    int m_colorG;
    int m_colorB;

    
    QPoint m_restorePos;
    QSize m_restoreSize;
    
    bool m_isPressed;
    QPoint m_startMovePos;
    
    QTimer m_titleRollTimer;
    
    QString m_titleContent;
    
    ButtonType m_buttonType;
    
    int m_windowBorderWidth;
    
    bool m_isTransparent;

    QMenuBar *m_menuBar;
    bool m_maxScreen = true;
    int m_titleH;
};


#endif // CUSTOMTITLE_H
