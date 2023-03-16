#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QFrame>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT Line : public QFrame
{
    Q_OBJECT
public:
   explicit Line( QWidget *parent = nullptr,QString backgroundColor = QString("#69696C"),QFrame::Shape shape = QFrame::HLine);

signals:

};

#endif // LINE_H
