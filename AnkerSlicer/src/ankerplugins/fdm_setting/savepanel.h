#ifndef SAVEPANEL_H
#define SAVEPANEL_H

#include <QWidget>
#include <QPushButton>
#include "common/controlInterface/messageDialog.h"
#include "common/controlInterface/bubbleWidget.h"
class SavePanel : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit SavePanel(QWidget *parent = nullptr);
    
    void init(QString profileName);
signals:
    void save();
    void saveAs(QString profileName);

public slots:
    
    void doParameterChanged(QString name);
    void doPanelShow();
    void doPanelHide();
    
    void doSliceSuccess();
    void doSliceFailed();
protected:
    void changeEvent(QEvent *e);
private:
    QPushButton *saveBtn;
    QPushButton *saveAsBtn;
    QString currentProfileName;
    void doSaveAsClick();

    control::MessageDialog *m_messageDialog;
    void textValid(int flag);
};

#endif // SAVEPANEL_H
