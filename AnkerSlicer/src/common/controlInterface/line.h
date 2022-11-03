#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QFrame>

class Line : public QFrame
{
    Q_OBJECT
public:
   explicit Line( QWidget *parent = nullptr,QString backgroundColor = QString("#69696C"),QFrame::Shape shape = QFrame::HLine);

signals:

};

#endif // LINE_H
