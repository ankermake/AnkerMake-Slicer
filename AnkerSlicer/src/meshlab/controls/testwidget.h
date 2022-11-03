#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include "common/controlInterface/pagewidget.h"
namespace  control{
class TestWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit TestWidget(PageWidget *parent = nullptr);

signals:

};
}
#endif // TESTWIDGET_H
