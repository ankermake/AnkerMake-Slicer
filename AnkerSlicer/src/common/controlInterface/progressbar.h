#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>

#include <QPainter>
#include <QDebug>
#include <QProgressBar>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = nullptr);

    void setValue(int value);
    void setAutoClosed(bool closed);
    void setBackgroundColor(QColor color);
    void setProgressCheckedColor(QColor color);
    void setProgressUncheckedColor(QColor color);
    void setProgressValueTextColor(QColor color);
    void setValuePointSize(int pointSize);
   // void reset();

signals:
    void closeProgress(); 
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int m_angle;
    int m_value;
    int m_maxValue;
    int m_minValue;
    bool m_autoClosed;
    QColor m_backgroundColor;
    QColor m_progressCheckedColor;
    QColor m_progressUncheckedColor;
    QColor m_progressValueTextColor;
    int m_valuePointSize;
};

#endif // PROGRESSBAR_H
