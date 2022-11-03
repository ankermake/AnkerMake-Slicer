#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>

namespace  control{
class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(QWidget *parent = nullptr);
    PageWidget(const PageWidget &widget) {
        this->m_displayName = widget.m_displayName;
    }

    virtual void closeWidget() { emit closeButtonClicked();};
    virtual void save() { emit saveButtonClicked();};
    virtual QString displayName() const { return m_displayName; }

signals:
    void saveButtonClicked();
    void closeButtonClicked();

protected:
    QString m_displayName;
};
}
#endif // PAGEWIDGET_H
