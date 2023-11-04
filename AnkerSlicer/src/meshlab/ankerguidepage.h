#ifndef ANKERGUIDEPAGE_H
#define ANKERGUIDEPAGE_H
#include "ankermainwindow.h"
#include <QMainWindow>
#include <QtWidgets>

class ankerGuidePage : public QMainWindow
{
    Q_OBJECT
public:
    explicit ankerGuidePage(QMainWindow *parent = nullptr,shared_ptr<AnkerMainWindow> p = nullptr);
    void showNormalWin();
    void openFileList(QStringList argfileList);
signals:

private:
    void initUi();
    shared_ptr<AnkerMainWindow> m_window ;
#ifdef Q_OS_WIN
    customTitle *m_mainTitleBar = nullptr;
#endif
    void showMaximizedWin();
//    QFramelessHelper *m_framelessHelper = nullptr;
//    void paintEvent(QPaintEvent *e);

public slots:
    void openSlicer();
    void openPreview();
    void openDevice();
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();
    void openFileFromAppRaram(const QString &fileName);
protected:
//    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // ANKERGUIDEPAGE_H
