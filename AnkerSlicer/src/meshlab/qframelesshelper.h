#ifndef QFRAMELESSHELPER_H
#define QFRAMELESSHELPER_H

#include <QObject>
#include <QWidget>

class QDrawShadowHelper;
class QFramelessHelper : public QObject
{
    Q_OBJECT
public:
    explicit QFramelessHelper(QWidget* w, bool resizeEnable, bool shadowBorder = true, bool winNativeEvent = true, QObject *parent = 0);

protected:
    
    void doShowEvent(QEvent *event);

    
    void doWindowStateChange(QEvent *event);
    void doResizeEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

    
    void updateDrawShadowState();
public:
    
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

    void paintEvent(QPaintEvent *e);

private:
    QWidget *m_widget;
    
    int m_padding;
    bool m_moveEnable;
    bool m_resizeEnable;
    bool m_shadowBorder;
    
    bool m_borderResizeEnable;

    
    QWidget *m_titleBar;

    
    bool m_mousePressed;
    QPoint m_mouseGlobalPoint;
    QRect m_mouseRect;

    
    
    QList<bool> m_pressedArea;
    QList<QRect> m_pressedRect;

    
    bool m_isMin;
    
    Qt::WindowFlags m_flags;

    
    bool m_winNativeEvent;

    
    QDrawShadowHelper* m_drawShadow;

public:
    
    void setPadding(int padding);
    void setMoveEnable(bool moveEnable);
    void setResizeEnable(bool resizeEnable);

    
    void setTitleBar(QWidget *titleBar);

    bool isMaximized();
public slots:
    void showMinimized();
    void showMaximized();
    void showNormal();
    void switchMaximizedNormal();
signals:
    void maximizedChanged(bool max);
};

#endif // QFRAMELESSHELPER_H
