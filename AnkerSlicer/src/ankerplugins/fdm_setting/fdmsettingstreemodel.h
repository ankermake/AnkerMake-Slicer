#ifndef FdmSettingsTreeModel_H
#define FdmSettingsTreeModel_H

#include <QAbstractItemModel>
#include <QDebug>
#include "filemanage/fdmparamdata.h"

namespace fdmsettings{

class FdmSettingsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum datatype{
        label= Qt::UserRole + 1,
        value,
        visible,
        level,
        type,
        unit,
    };
    explicit FdmSettingsTreeModel(QObject *parent = nullptr);
    ~FdmSettingsTreeModel();

    // Header:
   // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    //    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    //    bool canFetchMore(const QModelIndex &parent) const override;
    //    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //Qt::ItemFlag flags(const QModelIndex &index) const override;'
    QHash<int,QByteArray> roleNames() const override;

    void setRootNode(FdmParamNode *root);

private:
    QString roleToString(int role) const;
    FdmParamNode *indexToNode(const QModelIndex &index) const;
    QModelIndex nodeToIndex(FdmParamNode *node)const;

private:
    FdmParamNode *m_root;
};

}
#endif // FdmSettingsTreeModel_H
