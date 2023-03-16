#ifndef RADIUSDIAOLG_H
#define RADIUSDIAOLG_H

#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT RadiusDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RadiusDialog(QWidget *parent = nullptr);
    void setRadius(int radius);
    void setBackgroundColor(QColor color);

protected:
   void paintEvent(QPaintEvent *event) override;

signals:

private:
   int m_radius;
   QColor m_color;
};


#endif // RADIUSDIAOLG_H
