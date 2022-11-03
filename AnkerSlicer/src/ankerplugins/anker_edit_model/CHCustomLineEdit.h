/*



*/


#ifndef CHCustomLineEdit_H
#define CHCustomLineEdit_H


#include <QDoubleSpinBox>
#include "QLabel"


class CHCustomLineEdit :public QWidget
{
    Q_OBJECT

public:
    CHCustomLineEdit(QString text, QWidget* parent = nullptr, double radius = 8, QColor color = QColor(125, 125, 125));
    ~CHCustomLineEdit();

public:
    QString text() const;
    void setText(QString string);
    void setValidator(const QValidator* vd);


Q_SIGNALS:
    void editingFinished();

private:
    virtual void paintEvent(QPaintEvent* event);

private:
    QDoubleSpinBox* m_dSpinBox;
    QLabel* m_unitLabel;
    double m_radius;
    QColor m_color;
};


#endif
