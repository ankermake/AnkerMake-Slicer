#ifndef PARAMLISTMODEL_H
#define PARAMLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVector>
#include <QDebug>
#include "filemanage/fdmparamdata.h"


namespace fdmsettings {

class ParamListModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(QSting  READ  WRITE set NOTIFY Changed)
public:
    enum datatype{
        label= Qt::UserRole +1,
        value,
        visible,
        level,
        type,
        unit,
        options,
        expand,
        icon,
        description,
        affects,
        affectedBy,
        minimumValue,
        maximumValue,
        minimumValueWarning,
        maximumValueWarning
    };
    explicit ParamListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    //qml
    Q_INVOKABLE void changeData(int row, QVariant text);
    Q_INVOKABLE void expandIndex(int row);
    Q_INVOKABLE void collapseIndex(int row);

    Q_INVOKABLE void updateList();

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setListDatas(const QList<FdmParamNode *> &datas);

private:
    QString roleToString(int role) const;
    void setChildNodeVisibility(int row, bool visbility);

signals:
    void updateFinished();

private slots:
    void onNodeValueChange(QVariant row);

private:
    QList<FdmParamNode *> m_datas;

};

}

#endif // PARAMLISTMODEL_H
