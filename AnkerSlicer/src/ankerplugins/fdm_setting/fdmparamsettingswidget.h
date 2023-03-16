#ifndef FDMPARAMSETTINGSWIDGET_H
#define FDMPARAMSETTINGSWIDGET_H

#include <QWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QQuickView>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>
#include <QQuickWidget>
#include "paramlistmodel.h"
#include "fdmsettingstreemodel.h"
#include "filemanage/fdmqmlsourcetree.h"
#include "filemanage/fdmqmlengine.h"
#include "service/fdmrightparameterservice.h"
#include "common/controlInterface/messageDialog.h"
#include "tooltip.h"
//#include "common/controlInterface/controlInterface.h"
namespace fdmsettings {

using namespace control;
class FdmParamSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FdmParamSettingsWidget(QWidget *parent = nullptr);
    ~FdmParamSettingsWidget();

    Q_INVOKABLE void reload(const QString &fileName);

    Q_INVOKABLE void showToolTip(const QString &titlte, const QString &description, const QString &affects, const QString &affectedBy, QPoint point);
    Q_INVOKABLE void hideToolTip();
    void changeEvent(QEvent *event) override;
signals:
   void openPreferenceIndex(int index);
   void currentIndexClicked(AkConst::EWidgetType widgetType, QString name);
   void resertParameter();

private slots:
    void openMachinePreference(const QString &name);
    void openMaterialPreference(const QString &name);
    void openParameterPreference(const QString &name);
    void resertButtonClicked();
    void showBubbleTip();
private:
    ParamListModel *model;
    FdmSettingsTreeModel *treeModel;
    ToolTip *m_toolTip;
    QTimer *m_timer;
    QPoint m_point;
    QString m_titlte;
    QString m_description;
    QString m_affects;
    QString m_affectedBy;

    QQuickWidget *m_quickView = nullptr;
};


}
#endif // FDMPARAMSETTINGSWIDGET_H
