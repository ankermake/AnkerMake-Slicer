#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

namespace  control{
class COMMONLIB_EXPORT PageWidget : public QWidget
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
