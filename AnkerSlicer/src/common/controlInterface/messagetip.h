#ifndef MESSAGETIP_H
#define MESSAGETIP_H

#include "bubbleWidget.h"
#include <QLabel>
#include <QCheckBox>
#include <QEvent>
#include <QPushButton>

namespace control {
class COMMONLIB_EXPORT MessageTipWidget : public BubbleWidget
{
    Q_OBJECT

public:
    MessageTipWidget(const QString&messageText, QWidget* parent = nullptr);
    ~MessageTipWidget();

    inline bool isDontRemindAgain() const{
        return m_isDontRemindAgain;
    }

    inline bool hasPressOK() const{
        return m_hasPressOk;
    }

    inline void setTipText(const QString& text){
        m_pLabel->setText(text);
    }

signals:
    void UserPressOkButton(bool dontRemindAgain);
    void LanguageUpdate();

protected:
    void changeEvent(QEvent * event);

private:
    QLabel* m_pLabel{nullptr};
    QCheckBox* m_pCheckBox{nullptr};
    QPushButton* m_pButton{nullptr};

    bool m_isDontRemindAgain{false};
    bool m_hasPressOk{false};
};
};

#endif // MESSAGETIP_H
