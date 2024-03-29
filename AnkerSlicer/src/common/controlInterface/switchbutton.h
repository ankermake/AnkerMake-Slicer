#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    bool checkState() const;
    void setCheckState(bool state);

private:
    void drawBackground(QPainter *painter);
    void drawSlider(QPainter *painter);
    void updateState(bool state);

Q_SIGNALS:
    void stateChanged(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_isChecked;
    int m_startX;
    QColor m_backgroundColor;
};

#endif // SWITCHBUTTON_H
