#ifndef AKPROGRESSBAR_H
#define AKPROGRESSBAR_H

#include <QWidget>

#include <QProgressBar>
#include <QPushButton>
#include <QHBoxLayout>

//namespace Ui {
//class AkProgressBar;
//}

class AkProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit AkProgressBar(QWidget *parent = nullptr);
    ~AkProgressBar();

    void paintEvent(QPaintEvent *event) override;


    void setValue(int value);
    void setFormat(const QString &format);

signals:
    
    void cancel();

private:
    //Ui::AkProgressBar *ui;
    QProgressBar *progressBar;
    QPushButton *pushButton;


};

#endif // AKPROGRESSBAR_H
