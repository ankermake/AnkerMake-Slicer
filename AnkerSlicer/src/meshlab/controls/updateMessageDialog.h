#ifndef UPDATEMESSAGEBOXBOX_H
#define UPDATEMESSAGEBOXBOX_H

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
#include <QTimer>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextEdit>
#include "common/controlInterface/titlewidget.h"
namespace  control{

class updateMessageDialog : public QDialog
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

    
    explicit updateMessageDialog(const QString &title, const QString &description,
                        int buttons, QWidget *parent = nullptr);

    
    void setBottonText(BUTTONFLAG botton, const QString &string);
    // dhf 654321
    
    void setDetailTitleText(const QString &text);
    void setDetailText(const QString &text);
    
    void setOTAReleaseNoteVisible(bool show);
    

    void setEditMode(bool edit);
    
    void setValidator(const QValidator *validator);
    
    void setWarning(const QString &warning);
    
    void setEditText(const QString &string);
    QString editText() const;

    //aden add
    void setAutoLevelText(const QString& leftText,  const QString &rightText);
    void setDescriptionText(const QString &text);

    void setAutoClosed(bool ok);
   // void setCloseButtonIcon(QIcon icon);
signals:
   // void editText(const QString &text) ;
    void buttonClick(int flag);

private:
      void init();
      void setButton(int buttons);
      void setButtonValue(QPushButton *button, BUTTONFLAG flage);

      // dhf 654321
      QString setLineFeed(const QString &text, const QFont &font, int pxWidth);
protected:
   void paintEvent(QPaintEvent *event) override;

private slots:
   void buttonClicked();

private:
   QLabel *m_title;
   QLabel *m_description;

   QLabel *m_detailTitle = nullptr;  
   QTextEdit *m_releaseNote = nullptr;

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


/*
class TimerMessageDialog : public MessageDialog
{
public:
    TimerMessageDialog(const QString &title, const QString &description, int time,
                       int buttons, QWidget *parent = nullptr);

private:
    QTimer *m_timer;
    int m_totalSecs = 0;
    QString m_description;
};
*/

}
#endif // UPDATEMESSAGEBOXBOX_H
