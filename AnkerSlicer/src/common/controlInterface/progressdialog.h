#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMutex>
#include "progressbar.h"
#include "line.h"
class ProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProgressDialog(QWidget *parent = nullptr);

    
    void setText(const QString &text);
    
    void setAutoClosed(bool closed);
    
    void setCancelVisible(bool interrupt);
private:
    void init();
    void changeEvent(QEvent *e);
public :
signals:
    void cancel();
    void progressClosed();

public slots:
    void cancelButtonClick();
    
    void setValue(int value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout *m_mainLayout;
    ProgressBar *m_progressbar;
    QLabel *m_label;
    QPushButton *m_cancelButton;
    bool m_autoClosed;
    bool m_alreadyClosed;

};

#endif // PROGRESSDIALOG_H
