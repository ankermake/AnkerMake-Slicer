#ifndef MESSAGEBOXBOX_H
#define MESSAGEBOXBOX_H

#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QValidator>
#include <QDebug>
#include "titlewidget.h"
namespace  control{
class MessageDialog : public QDialog
{
    Q_OBJECT
public:
    enum BUTTONFLAG
    {
        CANCEL = 0x00000400,
        DoNotSave = 0x00002000,
        SAVE = 0x00000800,
        YES = 0x00400000,
        OK = 0x00200000,
        NO = 0x00800000
    };

    
    explicit MessageDialog(const QString &title, const QString &description,
                        int buttons, QWidget *parent = nullptr);

    
    void setEditMode(bool edit);
    
    void setValidator(const QValidator *validator);
    
    void setWarning(const QString &warning);
    
    void setEditText(const QString &string);
    QString editText() const;

    void setAutoClosed(bool ok);
   // void setCloseButtonIcon(QIcon icon);
signals:
   // void editText(const QString &text) ;
    void buttonClick(int flag);

private:
      void init();
      void setButton(int buttons);
      void setButtonValue(QPushButton *button, BUTTONFLAG flage);

protected:
   void paintEvent(QPaintEvent *event) override;

private slots:
   void buttonClicked();

private:
   QLabel *m_title;
   QLabel *m_description;
   QLineEdit *m_edit;
   QLabel *m_warning;
   QPushButton *m_leftButton;
   QPushButton *m_middleButton;
   QPushButton *m_rightButton;
   QVBoxLayout *m_mainLayout;
   QHBoxLayout *m_pushLayout;
   bool m_isClosed;

   TitleWidget *m_titleWidget;

};
}
#endif // MESSAGEBOXBOX_H
