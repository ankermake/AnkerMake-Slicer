#ifndef MESHINFOWIDGET_H
#define MESHINFOWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHeaderView>
#include <QAction>
#include <QToolBar>
#include <QWidget>
#include <QScrollBar>

#include <QGridLayout>
#include <QDebug>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFontMetrics>
#include "common/ml_document/mesh_document.h"
#include "common/controlInterface/bubbleWidget.h"
#include "common/GeoAndShow/CHScene.h"
namespace  control{

class ListWidgetItem;
class LineItem;
class MeshInfoWidget : public BubbleWidget
{
    Q_OBJECT
public:
    explicit MeshInfoWidget(QWidget *parent = nullptr);

    void addMeshModel();
    void addItem(QString name, bool visble, bool selected, int index);
    void init();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void currentChanged();

private slots:
    void showMesh();
    void deleteMesh();
    void hideList();
    //void clickCurrentIndex(const QModelIndex &index);
    void selectedChanged() ;

    void updateItem();

private:
   CH3DPrintModelPtr findCurrentModel(CHDocPtr docPtr,int index);
   bool docPtrStatusChanged(CHDocPtr docPtr);
    void changeEvent(QEvent *e) override;
private:
    QListWidget *m_listWidget;
    QToolButton *m_expandButton;

    bool flag = false;
    QLabel *m_title = nullptr;
};


}
#endif // MESHINFOWIDGET_H
