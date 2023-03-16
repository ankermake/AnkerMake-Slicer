#ifndef MACHINEWIDGET_H
#define MACHINEWIDGET_H

#include <QWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QQuickView>
#include <QVBoxLayout>
#include "common/controlInterface/pagewidget.h"
#include "preferencesparamters.h"
#include "filemanage/fdmqmlsourcetree.h"
#include "filemanage/fdmqmlengine.h"
#include "service/fdmmachineprofileservice.h"
#include "common/controlInterface/messageDialog.h"
namespace control {
class MachineWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit MachineWidget(PageWidget *parent = nullptr);
    ~MachineWidget();

private:
    QVariantMap findParameters(FdmParamNode *root, QString keyNodeName, QStringList list);
     void getDefaultNewName(const QStringList &list, const QString &name);
    void changeEvent(QEvent *e);
signals:
    void machineRename( QString oldName, QString newName);

private slots:
    void renameCustomMachine(const QString &oldName);
    void textValid(int flag);

private:
    FdmMachineProfileService *m_service;
    QString m_oldName;
    MessageDialog *m_messageDialog;
    QString m_newName;
    QQuickView *m_quickView = nullptr;
};
}

#endif // MACHINEWIDGET_H
